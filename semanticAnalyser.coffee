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
					then throw new analyser.SemanticError 'variable has been already declared'
					else checkTree.rbInsert new rbtree.RBTNode node.children[0] node.children[1].value
				when 'became'
					checkIfInTree node.children[0]
					if node.children[1].value is ///[0-9]+/// and (checkTree.rbFind node.children[0]).argumentsType is 'letter'
					then throw new analyser.SemanticError 'variable has been declared as another type' 
					if node.children[1].value is ///([\'][^\'][\'])/// and (checkTree.rbFind node.children[0]).argumentsType is 'number'
					then throw new analyser.SemanticError 'variable has been declared as another type' 
					if node.children[1].value <= 10 and node.children[1].value >= 1
						checkIfInTree node.children[1].children[0]
						checkTypeNum node.children[1].children[0]
						checkIfInTree node.children[1].children[1]
						checkTypeNum node.children[1].children[1]
				when 'drank', 'ate'
					checkIfInTree node.children[0]
					checkTypeNum node.children[0]
				when 11
					checkIfInTree node.children[0]
				else throw new analyser.SemanticError 'I do not recognise this function'
	
		checkIfInTree: (variable) ->
			if (checkTree.rbFind variable).key is null
			then throw new analyser.SemanticError 'variable has not been declared'
		checkTypeNum: (variable) ->
			if (checkTree.rbFind variable).argumentsType isnt 'number'
			then throw new analyser.SemanticError 'this function works only with numbers'
		buildtree: () ->
			counter = 0
			stringTree = changeToString node for node in parseTree
			stringTree.join
		changeToString: (node) ->
			# was a
			str = "#{counter}##{nodeType node}##{opType node}##{varType node},#{node.children[0]},"
			(checkTree.rbFind node.children[0]).counter = counter
			# became
			str = "#{counter}##{nodeType node}##{opType node}##{node.children[0]},#{++counter},|#{changeToString node.children[1]}"
			# became expr
			str = "#{counter}##{nodeType node}##{opType node}##{++counter},#{++counter},"
				rbnode = checkTree.rbFind node.children[0]
				if rbnode.key is null
					"#{counter-1}#CONST#NONE#NUMBER,#{node.children[0]}"
				else
					"#{counter-1}#CONST#NONE#NUMBER,#{node.children[0]}"
			
		###
		x was a number counter#TYPE#NONE#NUMBER,x
		x became 42    counter#VAR#NONE#x,1,|1#CONST#NONE#NUMBER,42,
		x became 'a'   counter#VAR#NONE#x,1,|1#CONST#NONE#LETTER,a,
		x became 1 + 2 counter#VAR#NONE#x,1,|1#OP#ADD#2,3,|2#CONST#NONE#NUMBER,1,|3#CONST#NONE#NUBMER,2,
		x became y + z counter#VAR#NONE#x,1,|1#OP#ADD#2,3,|2#VAR#NONE#NUMBER,y,|3#VAR#NONE#NUBMER,z,
		x drank		   counter#OP#ADD#x,1,2,|1#VAR#NONE#x,|2#CONST#NONE#NUMBER,1
		x spoke		   counter#OP#RETURN#x,
		###
		nodeType: (node) ->
			switch node.type
				when 0 then "OP"
				when 1 then "VAR"
				when 2 then "CONST"
				when 3 then "TYPE"

		opType: (node) ->
			switch node.value
				when -1 then "NONE"
				when 1 then "ADD"
				when 2 then "OR"
				when 3 then "XOR"
				when 4 then "AND"
				when 5 then "SUB"
				when 6 then "MUL"
				when 7 then "DIV"
				when 8 then "MOD"
				when 9 then "UNR"
				when 10 then "NEG"
				when 11 then "RETURN"
				else "NONE"
		varType: (node) ->
			switch node.children[1]
				when "number" then "NUMBER"
				when "letter" then "LETTER"

	analyser.SemanticError = (message, line, column) ->
    	@name = 'SemanticError'
    	@message = message
    	@line = line
    	@column = column

	analyser.SemanticError.prototype = Error.prototype;

	return analyser
)()