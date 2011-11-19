module.exports = (() ->
	rbtree = require './rbtree.coffee'

	analyser =
		analyse: (parseTree) ->
			 return parseTree
		buildtree: () ->
		

	analyser.SemanticError = (message, line, column) ->
    	@name = 'SemanticError'
    	@message = message
    	@line = line
    	@column = column

	analyser.SemanticError.prototype = Error.prototype;

	return analyser
)()