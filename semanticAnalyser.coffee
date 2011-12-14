module.exports = (() ->
	[RBTree, RBTNode] = require './rbtree.coffee'
	Types = require './constants.coffee'

	class Semantics
		Dictionary: []
		globalScopeName: 'top'

		analyse: (parseTree) ->
			@Dictionary[@globalScopeName] = [null, new RBTree]
			@initialise()
			@check parseTree, @globalScopeName

		check: (node, scope) ->
			#console.log node.type
			switch node.type
				when Types.NODE_ROOT
					@check child, scope for child in node.children
				when Types.NODE_VAR
					console.log node.value
				when Types.NODE_IO
					if node.children[0].type is Types.NODE_VAR
						if not (@lookup scope, node.children[0].value)?
							throw new SemanticError node.children[0]
					else if node.children[0].type is Types.NODE_FUN_CALL
						@check node.children[0], scope
						functionNode = @lookup node.children[0].children[0].value, scope
						if not functionNode?
							throw new SemanticError node.children[0]
				when Types.NODE_FUN_CALL
					functionNode = @lookup scope, node.value
					if not functionNode?
						throw new SemanticError node
			
					if node.value is 'was a'
						varNode = @lookup scope, node.children[0].value
						if varNode?
							throw new SemanticError varNode
						(@getScope scope).rbInsert (new RBTNode node.children[0].value, node.children[1].value, null)
					else if node.value is 'became'
						if node.children[0].type is Types.NODE_VAR_ARRAY_ELEM 
							if node.children[0].children[1].type is Types.NODE_CONSTANT and not node.children[0].children[1].children[0].type is Types.TYPE_NUMBER
								throw new SemanticError node.children[0]
							varNode = @lookup scope, node.children[0].children[0].value
						else 
							varNode = @lookup scope, node.children[0].value

						if not varNode?
							throw new SemanticError node.children[0]
						varType = varNode.returnType
						if node.children[1].type is Types.NODE_OP
							@check node.children[1]
							varType is Types.TYPE_NUMBER
						else if node.children[1].type is Types.NODE_FUN_CALL
							@check node.children[1]
							valueReturnType = (@lookup scope, node.children[1].children[0].value).returnType
							varType is valueReturnType
						else 
							varType is node.children[1].children[0].value
					else if node.value is 'ate' or node.value is 'drank'
						varNode = @lookup scope, node.children[0].value
						varNode.returnType is Types.TYPE_NUMBER
					else if node.value is 'had'
						varNode = @lookup scope, node.children[0].value
						if varNode?
							throw new SemanticError varNode
						(@getScope scope).rbInsert (new RBTNode node.children[0].value, node.children[2].value, null)
					else 				
						# NEEDS WORK
						typeCheck = functionNode.typeCheck
						callingTypes = node.children.map (node, index) -> node.type
						#console.log "function " + node.value + " called with " + callingTypes
						typeCheck callingTypes
				when Types.NODE_WHILE
					@isBoolean node.children[0]
					@check child, scope for child in node.children.splice 1
				when Types.NODE_IF, Types.NODE_ELSE_IF
					@isBoolean node.children[0]
					@check child, scope for child in node.children.splice 1
				when Types.NODE_RETURN
					if not node.children[1]?
						throw new SemanticError node
					@check node.children[0], scope
				when Types.NODE_FUN_DEF
					returnType = node.children[0].type
					functionName = node.children[1].value
					functionHeader = (node.children[1].children.splice 1).map (argument, index) ->
						argument.children[1].value
					#console.log functionHeader
					(@getScope scope).rbInsert (new RBTNode functionName, returnType, @functionTypeCheck(functionHeader))
					@Dictionary[functionName] = [scope, new RBTree]
					@check child, functionName for child in node.children.splice 2
				when Types.NODE_LOOK
					functionNode = @lookup scope, node.value
					if not functionNode?
						throw new SemanticError node
					varNode = @lookup scope, node.children[0].value
					functionNode.typeCheck varNode.returnType
				when Types.NODE_LOOK_DEF
					returnType = node.children[0].type
					functionName = node.value
					(@getScope scope).rbInsert (new RBTNode functionName, returnType, @functionTypeCheck(returnType))
					@Dictionary[functionName] = [scope, new RBTree]
					(@getScope functionName).rbInsert (new RBTNode 'it', returnType, null)
					@check child, functionName for child in node.children.splice 1

		isBoolean: (node) ->

		isNumerical: (node) ->

		functionTypeCheck: (types) ->
			(runTimeArgs) =>
				#console.log "Checking " + types + " with run time params " + runTimeArgs
				unifiedTypes = @unify runTimeArgs, types, []
				#unifiedTypes.reduce ((previous, current) => 
				#	previous and current)
				#	, yes

		unify: (header, runtime, defined) ->
			if header.length isnt runtime.length
				no
				#console.log 'massive error'
			#console.log "unifying types " + header + " and " + runtime

		lookupInScope: (scope, name) ->
			@getScope(scope).rbFind name
		
		lookup: (scope, name) ->
			if not scope? then return null
			labelNode = @lookupInScope scope, name
			if labelNode?
				labelNode
			else
				@lookup @getOuterScope(scope), name

		getScope: (scope) ->
			@Dictionary[scope][1]
		
		getOuterScope: (scope) ->
			@Dictionary[scope][0]

		# NODE_TYPE represents generic type so we need to try to unify them
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
		
	class SemanticError extends Error
		constructor: (node) ->
			@line = node.position.line
			@column = node.position.column
			@message = @buildErrorMessage node
			@name = 'SemanticError'
		
		buildErrorMessage: (node) ->
			switch node.type
				when Types.NODE_VAR
					message = Types.NODE_VAR.toLowerCase() + ' "' + node.value + '" hasn\'t been defined'
			
			message

	return new Semantics
)()
