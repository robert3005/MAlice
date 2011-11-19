module.exports = () ->
	rbtree = require 'rbtree.coffee'

	analyzer =
		analyze: (parseTree) ->
			 return parseTree

	return analyzer