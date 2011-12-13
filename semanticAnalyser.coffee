module.exports = (() ->
	[RBTree, RBTNode] = require './rbtree.coffee'
	Types = require './constants.coffee'

	analyser =
		analyse: (parseTree) ->
			@checkTree = new RBTree
			@initialise()
			@check parseTree 
			@checkTree

		check : (node) ->
			@check child for child in node.children

		functionTypeCheck: (types...) ->
			functionHeader = types
			(runTimeArgs...) =>
				console.log functionHeader
				functionHeader.map ((type, index ) ->
					unify type, runTimeArgs[index]
				).reduce ((previous, current) -> 
					previous and current)
					, yes

		unify: (typeA, typeB) ->
			if typeA is 'Type' then yes else typeA is typeB

		variableTypeCheck: (name) ->
			@checkTree.rbFind name

		###
		NODE_TYPE represents generic type so we need to try to unify them
		###
		initialise: () ->
			@checkTree.rbInsert (new RBTNode 'was a', null, @functionTypeCheck(Types.NODE_VAR, Types.NODE_TYPE))
			@checkTree.rbInsert (new RBTNode 'became', null, @functionTypeCheck(Types.NODE_TYPE, Types.NODE_TYPE))
			@checkTree.rbInsert (new RBTNode 'what was', null, @functionTypeCheck(Types.NODE_VAR))
			@checkTree.rbInsert (new RBTNode 'spoke', null, @functionTypeCheck(Types.NODE_TYPE))
			@checkTree.rbInsert (new RBTNode 'said Alice', null, @functionTypeCheck(Types.NODE_TYPE))
			@checkTree.rbInsert (new RBTNode 'ate', Types.TYPE_NUMBER, @functionTypeCheck(Types.TYPE_NUMBER))
			@checkTree.rbInsert (new RBTNode 'drank', Types.TYPE_NUMBER, @functionTypeCheck(Types.TYPE_NUMBER))
			@checkTree.rbInsert (new RBTNode 'had', null, @functionTypeCheck(Types.NODE_VAR, Types.TYPE_NUMBER, Types.NODE_TYPE))
			@checkTree.rbInsert (new RBTNode 'element', Types.NODE_TYPE, @functionTypeCheck(Types.NODE_TYPE, Types.TYPE_NUMBER))
			@checkTree.rbInsert (new RBTNode Types.NODE_OP, Types.TYPE_NUMBER, @functionTypeCheck(Types.TYPE_NUMBER, Types.TYPE_NUMBER))

		SemanticError: (@message, @line, @column) ->
    		@name = 'SemanticError'

	analyser.SemanticError.prototype = Error.prototype;

	return analyser
)()
