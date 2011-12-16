[RBTree, RBTNode] = require './rbtree.coffee'
Types = require './constants.coffee'

class Semantics

	constructor: ->
		@Dictionary = []
		@ASTParser = new AST
		@globalScopeName = 'top'
		@Dictionary[@globalScopeName] = [null, new RBTree]
		@initialise()

	# builds label tree and performs semantic checking 
	analyse: (parseTree) ->
		@findDeclarations parseTree, @globalScopeName
		@check parseTree, @globalScopeName

	# Actually performs semantic checking of the program
	# for IO calls the values given need just to be defined
	# for became both sides of the operation have to be of same type
	# for ate and drank checks whether argument is a number
	# for any other function call definition types and run time types need to be the same
	# for loops and ifs checks whether condition evaluates to boolean type
	# for return statements if there is enclosing function it tries to match function return type and type of return statement
	check: (node, scope) ->
		if node?
			switch node.type
				
				when Types.NODE_IO
					[varType, position] = @ASTParser.ioCall node
					
					if varType is Types.NODE_VAR
						@isDefined node.children[0], scope

					else if varType is Types.NODE_FUN_CALL
						@check node.children[0], scope
						@isDefined node.children[0], @globalScopeName

					else if varType is Types.NODE_OP
						@isOfType node.children[0], scope, Types.TYPE_NUMBER
				
				when Types.NODE_FUN_CALL
					functionName = node.value
					functionNode = 	@isDefined node, @globalScopeName			
					
					if functionName is 'had' or functionName is 'was a' then return
					
					if functionName is 'became'
						[lhs, rhs, position] = @ASTParser.functionCallBecame node

						varType = (@isDefined lhs, scope).returnType

						if rhs.type is Types.NODE_OP
							@isOfType rhs, scope, Types.TYPE_NUMBER 
							varReturnType = Types.TYPE_NUMBER
						else if rhs.type is Types.NODE_FUN_CALL
							@check rhs, scope
							varReturnType = (@isDefined rhs, @globalScopeName).returnType
						else if rhs.type is Types.NODE_VAR
							varReturnType = (@isDefined rhs, scope).returnType
						else if rhs.type is Types.NODE_VAR_ARRAY_ELEM
							varReturnType = (@isDefined rhs, scope).returnType
						else 
							varReturnType = rhs.children[0].value
						
						if varType isnt varReturnType
								throw new SemanticError "Both sides of assignment have to be of same type\n" + "lhs is of type '" + varType + "'\n rhs is of type '" + varReturnType + "'", position

					else if functionName is 'ate' or functionName is 'drank'

						varReturnType = (@isDefined node.children[0], scope).returnType
						if varReturnType isnt Types.TYPE_NUMBER
							throw new SemanticError "Trying to pass variable of type '" + varReturnType + "'.\n" + "Ate and drank operators are defined only for numbers", position
							 
					else 	
						typeCheck = functionNode.typeCheck
						callingTypes = node.children.map (node, index) => @computeExpressionType node, scope
						sameTypes = typeCheck callingTypes

						if not sameTypes
							throw new SemanticError "Could not find matching function for call '" + functionName + "'' with types '" + callingTypes.join "', '" + "'", node.position

				when Types.NODE_LOOP
					[whileCondition, whileBody, position] = @ASTParser.whileBlock node
					# loops can have their own scope so we need to check variable existence in new scope
					whileScope = Types.NODE_LOOP + "@" + position.line
					@isOfType whileCondition, scope, null
					@check child, whileScope for child in whileBody
				
				when Types.NODE_IF, Types.NODE_ELSE_IF
					[ifCondition, ifBody, position] = @ASTParser.ifBlock node
					@isOfType ifCondition, scope, null
					@check child, scope for child in ifBody
				
				when Types.NODE_ELSE
					@check child, scope for child in node.children
				
				when Types.NODE_RETURN
					[returnNode, position] = @ASTParser.returnCall node
					
					if node.children[1]?
						throw new SemanticError "Functions can only return one value", position
					
					returnType = @computeExpressionType returnNode, scope
					enclosingFunction = @lookup @globalScopeName, scope
					# return statement doesn't have to have enclosing function
					if enclosingFunction? and returnType isnt enclosingFunction.returnType
						throw new SemanticError "Return type: '" + returnType + "' doesn't agree with function '" + enclosingFunction.key + "' return type '" + enclosingFunction.returnType + "'", position
					
					@check returnNode, scope
				
				when Types.NODE_FUN_DEF
					[functionName, functionHeader, returnType, position] = @ASTParser.functionDefinition node
					[retType, header, functionBody...] = node.children
					@check child, functionName for child in functionBody
				
				when Types.NODE_LOOK
					functionNode = @isDefined node, @globalScopeName
					
					varReturnType = (@isDefined node.children[0], scope).returnType
					functionNode.typeCheck varReturnType
				
				when Types.NODE_LOOK_DEF
					[glassName, returnType, position] = @ASTParser.lookingGlassDefinition node
					[retType, lookBody...] = node.children
					@check child, glassName for child in lookBody
				
				else 
					@check child, scope for child in node.children

	# checks whether node in the current scope is of given type
	isOfType: (node, scope, type) ->
		expectedType = type

		condTypes = (exp) =>

			switch exp.type
				
				when Types.NODE_OP
					condTypes subExp for subExp in exp.children
					
					if exp.value in Types.boolOperators
						foundType = expectedType
					else
						foundType = Types.TYPE_NUMBER
				
				when Types.NODE_CONST
					foundType = exp.children[0].value

				when Types.NODE_VAR
					foundType = (@isDefined exp, scope).returnType

				when Types.NODE_VAR_ARRAY_ELEM
					foundType = (@isDefined exp, scope).returnType
		
				when Types.NODE_FUN_CALL
					@check child, scope for child in exp.children
					foundType = (@isDefined exp, @globalScopeName).returnType

			if expectedType?
				if foundType isnt expectedType 
					throw new SemanticError "Wrong type in expression.\n" + "Expected: " + expectedType + "\n Found: " + foundType, exp.position
			else
				expectedType = foundType

		condTypes node

	# function that defines how to perform type checking on user defined functions
	functionTypeCheck: (types) ->
		(runTimeArgs) =>
			unifiedTypes = types.map (typeVar, index) -> typeVar is runTimeArgs[index]
			unifiedTypes.reduce ((previous, current) -> previous and current), yes

	# returns type of give node in its scope
	computeExpressionType: (node, scope) ->

		switch node.type

			when Types.NODE_VAR_ARRAY_ELEM
				(@isDefined node, scope).returnType

			when Types.NODE_VAR
				(@isDefined node, scope).returnType

			when Types.NODE_CONST
				node.children[0].value
								
			when Types.NODE_OP
				@isOfType node, scope, Types.TYPE_NUMBERO
				Types.TYPE_NUMBER
			
			when Types.NODE_FUN_CALL
				@check child, scope for child in node.children
				(@isDefined node, @globalScopeName).returnType

	# walks parse tree and populates label tree with definitions of functions and variables from parse tree
	findDeclarations: (node, scope) ->
		if node?
			switch node.type
				
				when Types.NODE_LOOK_DEF
					[glassName, returnType, position] = @ASTParser.lookingGlassDefinition node
					functionNode = @lookup @globalScopeName, glassName
					
					if functionNode?
						throw new SemanticError "Function '" + glassName + "' has already been declared.\n Trying to redeclare", position
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
						throw new SemanticError "Function '" + functionName + "' has already been declared.\n Trying to redeclare", position
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
							throw new SemanticError "Variable '" + varName + "' has already been declared as '" + varNode.returnType + "'.\n Trying to redeclare as '" + varType + "'", position
						(@getScope scope).rbInsert new RBTNode varName, position, varType, @functionTypeCheck [varType]
				
					if node.value is 'had'
						[arrName, arrType, position] = @ASTParser.arrayDeclaration node
						arrNode = @lookup scope, arrName
						if arrNode?
							throw new SemanticError "Array '" + arrName + "' has already been declared as '" + arrNode.returnType + "'.\n Trying to redeclare as '" + arrType + "'", position
						(@getScope scope).rbInsert new RBTNode arrName, position, arrType, @functionTypeCheck [arrType]
				else 
					@findDeclarations node, scope for node in node.children
	
	# perform lookup of name within scope
	lookup: (scope, name) ->
		@getScope(scope).rbFind name

	# performs lookup of name but bubbles up until no enclosing scope exists
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

	# Populates label tree with language defined functions to prevent overwriting
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
		globalScope.rbInsert (new RBTNode Types.NODE_OP, Types.TYPE_NUMBER, null)

	# Checks whether node is defiend with the scope.
	# If yes returns the label tree node associated with this parse tree node.
	isDefined: (node, scope) ->
		
		switch node.type
			
			when Types.NODE_FUN_CALL
				functionNode = @lookup scope, node.value

				if not functionNode?
					throw new SemanticError "Call to undefined function: " + node.value, node.position
				functionNode	
								
			when Types.NODE_VAR
				varNode = @totalLookup scope, node.value
				
				if not varNode?
					throw new SemanticError "Variable '" + node.value + "' hasn't been declared", node.position
				
				varNode

			when Types.NODE_VAR_ARRAY_ELEM
				varArr = @totalLookup scope, node.children[0].value
				
				if not varArr?
						throw new SemanticError "Array '" + node.children[0].value + "' hasn't been declared", node.position
				varArr

			when Types.NODE_LOOK
				glassNode = @lookup @globalScopeName, node.value
				
				if not glassNode?
					throw new SemanticError	"Call to undefined function: " + node.value, node.position

				glassNode

	# Class representing error of semantic analyser
	SemanticError: class SemanticError extends Error
	
			constructor: (@message, position) ->
				@line = position.line
				@column = position.column
				@name = 'SemanticError'
	
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
	
	functionCallBecame: (node) ->
		lhs = node.children[0]
		rhs = node.children[1]
		position = node.position
		[lhs, rhs, position]

	ioCall: (node) ->
		varType = node.children[0].type
		position = node.position
		[varType, position]
	
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

module.exports = Semantics