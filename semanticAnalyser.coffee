module.exports = (() ->
	[RBTree, RBTNode] = require './rbtree.coffee'
	Types = require './constants.coffee'
	util = require 'util'

	class Semantics

		constructor: ->
			@Dictionary = []
			@ASTParser = new AST
			@globalScopeName = 'top'
			@Dictionary[@globalScopeName] = [null, new RBTree]
			@initialise()

		analyse: (parseTree) ->
			@findDeclarations parseTree, @globalScopeName
			#console.log (util.inspect @Dictionary, false , 50)
			@check parseTree, @globalScopeName

		check: (node, scope) ->
			if node?
				switch node.type
					
					when Types.NODE_IO
						[varType, position] = @ASTParser.ioCall node
						
						if varType is Types.NODE_VAR
						
							if not (@totalLookup scope, node.children[0].value)?
								throw new SemanticError "Variable '" + node.children[0].value + "' hasn't been defined", position
						else if varType is Types.NODE_FUN_CALL
							@check node.children[0], scope
							functionNode = @totalLookup @globalScopeName, node.children[0].value
							
							if not functionNode?
								throw new SemanticError "call to undefined function: " + node.children[0].value, position
						else if varType is Types.NODE_OP
							@isNumerical node.children[0], scope
					
					when Types.NODE_FUN_CALL
						functionName = node.value
						functionNode = @lookup @globalScopeName, functionName
						
						if functionName is 'had' or functionName is 'was a' then return
												
						if not functionNode?
							throw new SemanticError "call to undefined function: " + functionName, node.position
				
						if functionName is 'became'
							[lhs, rhs, position] = @ASTParser.functionCallBecame node
							
							if lhs.type is Types.NODE_VAR_ARRAY_ELEM 
								varName = lhs.children[0].value
								varNode = @totalLookup scope, lhs.children[0].value
							else
								varName = lhs.value
								varNode = @totalLookup scope, lhs.value

							if not varNode?
								throw new SemanticError "Variable '" + varName + "' hasn't been defined" , position
							varType = varNode.returnType
						

							if rhs.type is Types.NODE_OP
								@isNumerical rhs, scope
								varReturnType = Types.TYPE_NUMBER
							else if rhs.type is Types.NODE_FUN_CALL
								@check rhs, scope
								varReturnType = (@lookup @globalScopeName, rhs.value).returnType
							else if rhs.type is Types.NODE_VAR
								varReturnType = (@totalLookup scope, rhs.value).returnType
							else if rhs.type is Types.NODE_VAR_ARRAY_ELEM
								varReturnType = (@totalLookup scope, rhs.children[0].value).returnType
							else 
								varReturnType = rhs.children[0].value
							
							if varType isnt varReturnType
								throw new SemanticError "both sides of assignment have to be of same type", position

						else if functionName is 'ate' or functionName is 'drank'
							[varName, position] = @ASTParser.functionCallUnary node
							varNode = @totalLookup scope, varName
							
							if not varNode?
								throw new SemanticError "Variable '" + varName + "' hasn't been declared", position

							if varNode.returnType isnt Types.TYPE_NUMBER
								throw new SemanticError "ate and drank functions are defined only for numbers", position
								 
						else 	
							typeCheck = functionNode.typeCheck
							callingTypes = node.children.map (node, index) => @computeExpressionType node, scope
							sameTypes = typeCheck callingTypes
								
							if not sameTypes
								throw new SemanticError "Wrong argument type for function call " + functionName, node.position

					when Types.NODE_LOOP
						[whileCondition, whileBody, position] = @ASTParser.whileBlock node
						whileScope = Types.NODE_LOOP + "@" + position.line
						@isBoolean whileCondition, scope
						@check child, whileScope for child in whileBody
					
					when Types.NODE_IF, Types.NODE_ELSE_IF
						[ifCondition, ifBody, position] = @ASTParser.ifBlock node
						@isBoolean ifCondition, scope
						@check child, scope for child in ifBody
					
					when Types.NODE_ELSE
						@check child, scope for child in node.children
					
					when Types.NODE_RETURN
						[returnNode, position] = @ASTParser.returnCall node
						if node.children[1]?
							throw new SemanticError "function can only return one value", position
						returnType = @computeExpressionType returnNode, scope
						enclosingFunction = @lookup @globalScopeName, scope
						if enclosingFunction? and returnType isnt enclosingFunction.returnType
							throw new SemanticError "return type doesn't agree with function definition", position
						@check returnNode, scope
					
					when Types.NODE_FUN_DEF
						[functionName, functionHeader, returnType, position] = @ASTParser.functionDefinition node
						[retType, header, functionBody...] = node.children
						@check child, functionName for child in functionBody
					
					when Types.NODE_LOOK
						[glassName, argName, position] = @ASTParser.lookingGlassCall node
						functionNode = @lookup @globalScopeName, glassName
						if not functionNode?
							throw new SemanticError	"call to undefined looking glass " + glassName, position
						varNode = @totalLookup scope, argName
						functionNode.typeCheck varNode.returnType
					
					when Types.NODE_LOOK_DEF
						[glassName, returnType, position] = @ASTParser.lookingGlassDefinition node
						[retType, lookBody...] = node.children
						@check child, glassName for child in lookBody
					else 
						@check child, scope for child in node.children

				
		isBoolean: (node, scope) ->
			booleanType = null

			condTypes = (exp) =>

				switch exp.type
					
					when Types.NODE_OP
						condTypes subExp for subExp in exp.children
						
						if exp.value in Types.boolOperators
							foundType = booleanType
						else
							foundType = Types.TYPE_NUMBER
					
					when Types.NODE_CONST
						foundType = exp.children[0].value

					when Types.NODE_VAR
						varNode = @totalLookup scope, exp.value
					
						if not varNode?
								throw new SemanticError "Variable '" + exp.value + "' hasn't been declared", exp.position
						foundType = varNode.returnType

					when Types.NODE_VAR_ARRAY_ELEM
						varArr = @totalLookup scope, exp.children[0].value
						
						if not varArr?
								throw new SemanticError "Array '" + exp.children[0].value + "' hasn't been declared", exp.position
						foundType = varArr.returnType

					when Types.NODE_FUN_CALL
						functionName = exp.value
						functionNode = @lookup @globalScopeName, functionName
						
						if not functionNode?
							throw new SemanticError "call to undefined function: " + functionName, exp.position
						foundType = functionNode.returnType


				if booleanType?
					if foundType isnt booleanType 
						throw new SemanticError "Wrong type in boolean expression", exp.position
				else
					booleanType = foundType

			condTypes node

		isNumerical: (node, scope) ->
		
			switch node.type
				
				when Types.NODE_OP
					@isNumerical subExp, scope for subExp in node.children
					foundType = Types.TYPE_NUMBER
				
				when Types.NODE_CONST
					foundType = node.children[0].value

				when Types.NODE_VAR
					varNode = @totalLookup scope, node.value
				
					if not varNode?
							throw new SemanticError "Variable '" + node.value + "' hasn't been declared", node.position
					foundType = varNode.returnType
				
				when Types.NODE_VAR_ARRAY_ELEM
					varArr = @totalLookup scope, node.children[0].value
					
					if not varArr?
							throw new SemanticError "Array '" + node.children[0].value + "' hasn't been declared", node.position
					foundType = varArr.returnType

				when Types.NODE_FUN_CALL
					functionName = node.value
					functionNode = @lookup @globalScopeName, functionName
					
					if not functionNode?
						throw new SemanticError "call to undefined function: " + functionName, node.position
					foundType = functionNode.returnType

			if foundType isnt Types.TYPE_NUMBER 
				throw new SemanticError "Wrong type in numerical expression", node.position

		findDeclarations: (node, scope) ->
			if node?
				switch node.type
					
					when Types.NODE_LOOK_DEF
						[glassName, returnType, position] = @ASTParser.lookingGlassDefinition node
						functionNode = @lookup @globalScopeName, glassName
						
						if functionNode?
							throw new SemanticError "Looking Glass '" + glassName + "' has already been declared", position
						(@getScope @globalScopeName).rbInsert new RBTNode glassName, position, returnType, @functionTypeCheck [returnType]
						newScope = new RBTree
						newScope.rbInsert new RBTNode 'it', position, returnType, @functionTypeCheck [returnType]
						@Dictionary[glassName] = [null, newScope]
						[retType, lookBody...] = node.children
						@findDeclarations child, glassName for child in lookBody
					
					when Types.NODE_FUN_DEF	
						[functionName, functionHeader, returnType, position] = @ASTParser.functionDefinition node
						functionNode = @lookup @globalScopeName, functionName
					
						if functionNode?
							throw new SemanticError "Function '" + functionName + "' has already been declared", position
						(@getScope @globalScopeName).rbInsert new RBTNode functionName, position, returnType, @functionTypeCheck functionHeader
						[retType, header, functionBody...] = node.children
						newScope = new RBTree
						header.children.forEach (argument, index, funcNode) =>
							[varName, varType, position] = @ASTParser.variableDeclaration argument
							newScope.rbInsert new RBTNode varName, position, varType, @functionTypeCheck [varType]
						@Dictionary[functionName] = [null, newScope]
						@findDeclarations child, functionName for child in functionBody
					
					when Types.NODE_LOOP
						[whileCondition, whileBody, position] = @ASTParser.whileBlock node
						whileScope = Types.NODE_LOOP + "@" + position.line
						@Dictionary[whileScope] = [scope, new RBTree]
						@findDeclarations child, whileScope for child in whileBody

					when Types.NODE_FUN_CALL
					
						if node.value is 'was a'
							[varName, varType, position] = @ASTParser.variableDeclaration node
							varNode = @lookup scope, varName
							if varNode?
								throw new SemanticError "Variable '" + varName + "' has already been declared", position
							(@getScope scope).rbInsert new RBTNode varName, position, varType, @functionTypeCheck [varType]
					
						if node.value is 'had'
							[arrName, arrType, position] = @ASTParser.arrayDeclaration node
							arrNode = @lookup scope, arrName
							if arrNode?
								throw new SemanticError "Array '" + arrName + "' has already been declared", position
							(@getScope scope).rbInsert new RBTNode arrName, position, arrType, @functionTypeCheck [arrType]
					else 
						@findDeclarations node, scope for node in node.children



		functionTypeCheck: (types) ->
			(runTimeArgs) =>
				unifiedTypes = types.map (typeVar, index) -> typeVar is runTimeArgs[index]
				unifiedTypes.reduce ((previous, current) -> previous and current), yes

		computeExpressionType: (node, scope) ->
			
			switch node.type
			
				when Types.NODE_VAR_ARRAY_ELEM
					varArr = @totalLookup scope, node.children[0].value
					if not varArr?
							throw new SemanticError "Array '" + node.children[0].value + "' hasn't been declared", node.position
					varArr.returnType

				when Types.NODE_VAR
					varNode = @totalLookup scope, node.value
					
					if not varNode?
						throw new SemanticError "Variable '" + node.value + "' hasn't been declared", node.position
					varNode.returnType
			
				when Types.NODE_CONST
					node.children[0].value
									
				when Types.NODE_OP
					@isNumerical node, scope
					Types.TYPE_NUMBER
				
				when Types.NODE_FUN_CALL
					functionNode = @lookup @globalScopeName, node.value
					@check child, scope for child in node.children

					if not functionNode?
						throw new SemanticError "call to undefined function: " + functionName, node.position

					functionNode.returnType					

		lookup: (scope, name) ->
			@getScope(scope).rbFind name

		totalLookup: (scope, name) ->

			if scope?
				lookupResult = @lookup scope, name
			
				if not lookupResult?
					return @totalLookup (@getOuterScope scope), name
				else
					return lookupResult
			else
				return null

		getScope: (scope) ->
			@Dictionary[scope][1]
		
		getOuterScope: (scope) ->
			@Dictionary[scope][0]

		initialise: () ->
			globalScope = @getScope(@globalScopeName)
			globalScope.rbInsert (new RBTNode 'was a', null, null)
			globalScope.rbInsert (new RBTNode 'became', null, null)
			globalScope.rbInsert (new RBTNode 'what was', null, null)
			globalScope.rbInsert (new RBTNode 'spoke', null, null)
			globalScope.rbInsert (new RBTNode 'said Alice', null, null)
			globalScope.rbInsert (new RBTNode 'ate', Types.TYPE_NUMBER, null)
			globalScope.rbInsert (new RBTNode 'drank', Types.TYPE_NUMBER, null)
			globalScope.rbInsert (new RBTNode 'had', null, null)
			globalScope.rbInsert (new RBTNode 'element', null, null) # not actually needed for now but needs testing
			globalScope.rbInsert (new RBTNode Types.NODE_OP, Types.TYPE_NUMBER, null)

		buildErrorMessage: (node) ->
			message = ''
			
			switch node.type
			
				when Types.NODE_VAR
					message = Types.NODE_VAR.toLowerCase() + ' "' + node.value + '" hasn\'t been defined'
			
			message
		
	class AST
		
		constructor: ->

		variableDeclaration: (node) ->
			varName = node.children[0].value
			varType = node.children[1].value
			position = node.position
			[varName, varType, position]

		arrayDeclaration: (node) ->
			arrName = node.children[0].value
			arrType = node.children[2].value
			position = node.position
			[arrName, arrType, position]

		functionDefinition: (node) ->
			returnType = node.children[0].value
			functionName = node.children[1].value
			functionHeader = node.children[1].children.map (argument, index) ->
				argument.children[1].value
			position = node.position
			[functionName, functionHeader, returnType, position]

		lookingGlassDefinition: (node) ->
			returnType = node.children[0].value
			glassName = node.value
			position = node.position
			[glassName, returnType, position]
		
		arrayElement: (node) ->
			arrName = node.children[0].value
			elementNumber = node.children[1].value
			position = node.position
			[arrName, elementNumber, position]

		functionCall: (node) ->
			functionName = node.value
			functionArgs = node.children
			position = node.position
			[functionName, functionArgs, position]

		functionCallUnary: (node) ->
			varName = node.children[0].value
			position = node.position
			[varName, position]

		functionCallBecame: (node) ->
			lhs = node.children[0]
			rhs = node.children[1]
			position = node.position
			[lhs, rhs, position]

		ioCall: (node) ->
			varType = node.children[0].type
			position = node.position
			[varType, position]
		
		lookingGlassCall: (node) ->
			glassName = node.value
			position = node.position
			argName = node.children[0].value
			[glassName, argName, position]

		returnCall: (node) ->
			returnNode = node.children[0]
			position = node.position
			[returnNode, position]

		ifBlock: (node) ->
			[ifCondition, ifBody...] = node.children
			position = node.position
			[ifCondition, ifBody, position]

		whileBlock: (node) ->
			[whileCondition, whileBody...] = node.children
			position = node.position
			[whileCondition, whileBody, position]

	class SemanticError extends Error
		
		constructor: (@message, position) ->
			@line = position.line
			@column = position.column
			@name = 'SemanticError'

	return Semantics
)()