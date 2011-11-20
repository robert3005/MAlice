module.exports = (() ->
	rbtree = require './rbtree.coffee'

	analyser =
		analyse: (parseTree) ->
			@checkTree = new rbtree.RBTree
			check node for node in parseTree 
		check : (node) ->
			switch node.value
				when 'was a'
					if (checkTree.rbFind node.children[0]).key isnt null
					then throw new SemanticError 'variable has been already declared'
					else checkTree.rbInsert new rbtree.RBTNode node.children[0] node.children[1].value
				when 'became'
					checkIfInTree node.children[0]
					if node.children[1].value is [0-9]+ and (checkTree.rbFind node.children[0]).argumentsType is 'letter'
					then throw new SemanticError 'variable has been declared as another type' 
					if node.children[1].value is '([\'][^\'][\'])' and (checkTree.rbFind node.children[0]).argumentsType is 'number'
					then throw new SemanticError 'variable has been declared as another type' 
					if node.children[1].value <= 10 and node.children[1].value >= 1
					then 
						checkIfInTree node.children[1].children[0]
						checkTypeNum node.children[1].children[0]
						checkIfInTree node.children[1].children[1]
						checkTypeNum node.children[1].children[1]
				when 'drank', 'ate'
					checkIfInTree node.children[0]
					checkTypeNum node.children[0]
				when 11
					checkIfInTree node.children[0]
				else throw new SemanticError 'I do not recognise this function'
		checkIfInTree: (var) ->
			if (checkTree.rbFind var).key is null
			then throw new SemanticError 'variable has not been declared'
		checkTypeNum: (var) ->
			if (checkTree.rbFind node.children[0]).argumentsType isnt 'number'
			then throw new SemanticError 'this function works only with numbers'
		buildtree: () ->
			stringTree = changeToString node for node in parseTree
			stringTree.join

	analyser.SemanticError = (message, line, column) ->
    	@name = 'SemanticError'
    	@message = message
    	@line = line
    	@column = column

	analyser.SemanticError.prototype = Error.prototype;

	return analyser
)()