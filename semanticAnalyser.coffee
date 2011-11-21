###
Fix regural expressions matching instead of comparison
node.value trailing whitespace

###

module.exports = (() ->
	[RBTree, RBTNode] = require './rbtree.coffee'
	analyser =
		analyse: (parseTree) ->
			@checkTree = new RBTree
			@check node for node in parseTree 
			@checkTree
		# TODO - somehow check the unary operators, so far it doesnt support this operation
		check : (node) ->
			console.log node
			switch node.value
				when 'was a '
					if @checkTree.rbFind node.children[0] isnt null
					then throw new analyser.SemanticError 'variable has been already declared'
					else @checkTree.rbInsert new RBTNode node.children[0], node.children[1]
				when 'became '
					@checkIfInTree node.children[0]
					console.log node.children[1]
					if node.children[1]?.type is 2 and node.children[1]?.children[0] is 'number' and (@checkTree.rbFind node.children[0]).argumentsType is 'letter'
					then throw new analyser.SemanticError 'variable has been declared as another type' 
					if node.children[1]?.type is 2 and node.children[1]?.children[0] is 'letter' and (@checkTree.rbFind node.children[0]).argumentsType is 'number'
					then throw new analyser.SemanticError 'variable has been declared as another type'
					@checkExpression node
				when 'drank', 'ate'
					@checkIfInTree node.children[0]
					@checkTypeNum node.children[0]
				when 'spoke'
					@checkIfInTree node.children[0]
				else throw new analyser.SemanticError 'I do not recognise this function'

		checkIfANumber: (strNum) ->
			if strNum isnt 'number'
				throw new analyser.SemanticError 'This operation supports only numbers'

		checkExpression: (node) ->
			while node?.children[1]?.type is 0
				@checkVarConst node.children[1].children[0]
				node = node.children[1]
			@checkVarConst node?.children[1]
	
		checkVarConst: (node) ->
			if node?.type is 2
				@checkIfANumber node.children[0]
			else
				@checkIfInTree node
				@checkTypeNum node

		checkIfInTree: (variable) ->
			if @checkTree.rbFind variable is null
			then throw new analyser.SemanticError 'variable has not been declared'

		checkTypeNum: (variable) ->
			if (@checkTree.rbFind variable).argumentsType isnt 'number'
			then throw new analyser.SemanticError 'this function works only with numbers'

		###
		x was a number counter#TYPE#NONE#NUMBER,x,|
		x became 42    counter#VAR#NONE#,x,1,|1#CONST#NONE#NUMBER,42,|
		x became 'a'   counter#VAR#NONE#,x,1,|1#CONST#NONE#LETTER,a,|
		x became y	   couunter#VAR#NONE#,x,1,|1#VAR#NONE#NUMBER,y,|
		x became 1 + 2 counter#VAR#NONE#,x,1,|1#OP#ADD#2,3,|2#CONST#NONE#NUMBER,1,|3#CONST#NONE#NUBMER,2,|
		x became y + z counter#VAR#NONE#,x,1,|1#OP#ADD#2,3,|2#VAR#NONE#NUMBER,y,|3#VAR#NONE#NUBMER,z,|
		x became ~5	   counter#VAR#NONE#,x,1,|1#OP#NEG#2,|2#CONST#NONE#NUMBER,5,|
		x drank		   counter#VAR#NONE#,x,1,|1#OP#ADD#2,3,|2#VAR#NONE#NUMBER,x,|3#CONST#NONE#NUBMER,1,|
		x spoke		   counter#RET#NONE#,x,|
		this became 4 + 6 + 8 + 10 counter#VAR#NONE#this,1,|1#OP#ADD#2,3,|2#CONST#NONE#NUMBER,4,|3#OP#ADD#4,5|4#CONST#NONE#NUMBER,6,|5#OP#ADD#6,7...
		###	
		# TODO Bruteforce checking of the type of the variable, when we have ex x became 5 or x became 'a', how to check consts type
		# Update - now every const has a type in parsetree, the concern above has been resolved
		buildtree: () ->
			counter = 0
			stringTree = @changeToString node counter for node in @parseTree
			stringTree.join
		changeToString: (node, counter) ->
			toString = ''
			console.log node
			switch node?.type
				# was a only way to declare a type so node.type = 3, we are done
				when 3 then toString += "#{counter}##{@nodeType node}##{@opType node}##{@varType node.children[1]},#{node.children[0]},|"
				# ok, so we have a VAR it might be either became or drank, ate
				when 1 
					if node.value is "ate" or node.value is "drank"
						toString += "#{counter}##{@nodeType node}##{@opType node}#,#{node.children[0]},#{++counter},|#{@drankAte node.children[0] node.value counter}"
					else
						toString += "#{counter}##{@nodeType node}##{@opType node}#,#{node.children[0]},#{++counter},|#{@changeToString node.children[1], counter}"
				# const value
				when 2 
					console.log 2
					toString += "#{counter}#CONST#NONE##{@varType node.children[0]},#{node.value},|"
				# operations /node.value 10 is for the neg operation
				when 0 
					if node.value is 10
						toString += "#{counter}##{@nodeType node}##{@opType node}##{++counter},|#{@changeToString node.children[0] counter}"
					else toString += "#{counter}##{@nodeType node}##{@opType node}##{++counter},#{++counter},|#{@changeToString node.children[0] counter-1}|#{@changeToString node.children[1] counter}"
				# spoke, return statement
				when 4 then toString += "#{counter}##{@nodeType node}##{@opType node}#,#{node.children[0]},|"
				# ok, so else case is when we have no object just a variable reference i assume node.type returns undefined and it actually works
				else @getElementCommand node counter
			toString

		drankAte: (variable, func, counter) ->
			switch func
				when "drank" then "#{counter}#OP#SUB##{++counter},#{++counter},|#{@changeToString variable counter-1}|#{counter}#CONST#NONE#NUBMER,1,|"
				when "ate" then "#{counter}#OP#ADD#{++counter},#{++counter},|#{@changeToString variable counter-1}|#{counter}#CONST#NONE#NUBMER,1,|"
				else "MOTHER OF GOD WE HAVE AN ERROR"

		# TODO We can do the lookup in rbtree to check the type
		# UPDATE - done
		getElementCommand: (variable, counter) ->
				rbnode = @checkTree.rbFind variable
				if rbnode isnt null
					"#{counter}#VAR#NONE##{rbnode.argumentsType},#{variable},|"
				else "MOTHER OF GOD WE HAVE AN ERROR"
			
		nodeType: (node) ->
			switch node.type
				when 0 then "OP"
				when 1 then "VAR"
				when 2 then "CONST"
				when 3 then "TYPE"
				when 4 then "RET"

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