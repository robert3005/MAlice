module.exports = (() ->
	rbtree = require './rbtree.coffee'

	analyzer =
		analyze: (parseTree) ->
			 return parseTree
		buildtree: () ->
		

	analyzer.SemanticError = (message, line, column) ->
    	@name = 'SemanticError'
    	@message = message
    	@line = line
    	@column = column

	analyzer.SemanticError.prototype = Error.prototype;

	return analyzer
)()