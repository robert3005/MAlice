{

	counter = 0

	function NODE() {
		var id;
		var type;
		var value;
		var children;
	};

	//Management functions
	function createNode( type, value, children ) {
		var n = new NODE();
		n.id = counter;
		n.type = type;
		n.value = value;	
		n.children = new Array();
		
		for( var i = 2; i < arguments.length; i++ )
			n.children.push( arguments[i] );
			
		return n;
	};

	NODE_OP		= 0
	NODE_VAR	= 1
	NODE_CONST	= 2
	NODE_TYPE	= 3
	NODE_RETURN = 4

	OP_NONE		= -1
	OP_ADD      = 1
	OP_OR		= 2
	OP_XOR		= 3
	OP_AND		= 4
	OP_SUB		= 5
	OP_MUL		= 6
	OP_DIV		= 7
	OP_MOD		= 8
	OP_NOT		= 9
	OP_NEG		= 10

}

start
= root*

root
= single:assignment separator { return single }
/ single:assignment newLine { return single }

assignment
= space identifier:id space name:function_name space type:typeName { counter++; return createNode( NODE_TYPE, name, identifier, type ) }
 / space identifier:id space name:function_name expr:expression { counter++; return createNode( NODE_VAR, name, identifier, expr ) }
 / space identifier:id space name:function_name { counter++; return createNode( NODE_VAR, name, identifier ) }
/ space identifier:id space 'spoke' { counter++; return createNode( NODE_RETURN, 'spoke', identifier) }


expression
= or_expression

or_expression
= xorExpr:xor_expression space '|' space orExpr:or_expression { counter++; return createNode( NODE_OP, OP_OR, xorExpr, orExpr ) }
/ xor_expression

xor_expression
= andExpr:and_expression space '^' space xorExpr:xor_expression { counter++; return createNode( NODE_OP, OP_XOR, andExpr, xorExpr ) }
/ and_expression

and_expression
= addExpr:add_expression space '&' space andExpr:and_expression { counter++; return createNode( NODE_OP, OP_AND, addExpr, andExpr) }
/ add_expression

add_expression
= notExpr:not_expression space '+' space addExpr:add_expression { counter++; return createNode(NODE_OP, OP_ADD, notExpr, addExpr) }
/ notExpr:not_expression space '-' space addExpr:add_expression { counter++; return createNode(NODE_OP, OP_SUB, notExpr, addExpr ) }
/ not_expression

not_expression
= notExpr:not_op space mulExpr:mul_expression {counter++; return createNode(NODE_OP, notExpr, mulExpr)}
/ mul_expression

mul_expression
= unExpr:unary_expression space '*'  space mulExpr:mul_expression  { counter++; return createNode(NODE_OP, OP_MUL, unExpr, mulExpr) }
/ unExpr:unary_expression space '/' space mulExpr:mul_expression { counter++; return createNode(NODE_OP, OP_DIV, unExpr, mulExpr) }
/ unExpr:unary_expression space '%' space mulExpr:mul_expression  { counter++; return createNode(NODE_OP, OP_MOD, unExpr, mulExpr) }
/ unary_expression

unary_expression
= unOP:neg_op primExpr:primitive_expression { counter++; return createNode(NODE_OP, unOP, primExpr ) }
/ primExpr:primitive_expression

primitive_expression
= num:[0-9]+ { counter++; return createNode( NODE_CONST, num.join(""), 'number' ) }
/ letter:([\'][^\'][\']) {counter++; return createNode( NODE_CONST, letter[1], 'letter' ) }
/ identifier:id
/ '(' expr:expression ')'

typeName
= type:'letter'
/ type:'number'

id
= identifier:([A-Za-z_]+) { return identifier.join("") }

function_name
= funcName:'was a ' { return funcName }
/ funcName:'became ' { return funcName }
/ funcName:'drank' { return funcName }
/ funcName:'ate' { return funcName }

newLine
= (space 'too' space [\.] space [\n]*) {return 1}
/ (space [\n]* space [\.\,] space [\n]*) {return 2 }

space
= ' '* { return }

separator
= ([\,][ ]) {return 7}
/ ([ ]'then'[ ]) {return 8}
/ ([ ]'or'[ ]){return 9}
/ ([ ]'and'[ ]) {return 10}
/ ([ ]'too'[ ]){return 11}
/ ([ ]'but'[ ]){return 12}

not_op
= '~' { return OP_NOT }

neg_op
= '-' { return OP_NEG }