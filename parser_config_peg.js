{
function NODE()
{
	var type;
	var value;
	var children;
}

//Management functions
function createNode( type, value, children )
{
	var n = new NODE();
	n.type = type;
	n.value = value;	
	n.children = new Array();
	
	for( var i = 2; i < arguments.length; i++ )
		n.children.push( arguments[i] );
		
	return n;
}


NODE_OP		= 0
NODE_VAR	= 1
NODE_CONST	= 2
NODE_TYPE	= 3

OP_NONE		= -1
OP_RETURN       =11
OP_ADD          = 1
OP_OR		= 2
OP_XOR		= 3
OP_AND		= 4
OP_SUB		= 5
OP_MUL		= 6
OP_DIV		= 7
OP_MOD		= 8
OP_UNR		= 9
OP_NEG		= 10
}

start
= root*

root
= single:assignment sep:separator {return single}
/ single:assignment enter:newLine {return single}

assignment
= sp:space identifier:id sp:space name:function_name sp:space type:typeName { return createNode( NODE_OP, name, identifier, type ) }
 / sp:space identifier:id sp:space name:function_name expr:expression { return createNode( NODE_OP, name, identifier, expr ) }
/ sp:space identifier:id sp:space 'spoke' { return createNode( NODE_OP, OP_RETURN, identifier) }


expression
= orExpr:or_expression

or_expression
= xorExpr:xor_expression sp:space '|' sp:space orExpr:or_expression { return createNode( NODE_OP, OP_OR, orExpr, xorExpr ) }
/ xorExpr:xor_expression

xor_expression
= andExpr:and_expression sp:space '^' sp:space xorExpr:xor_expression { return createNode( NODE_OP, OP_XOR, xorExpr, andExpr ) }
/ andExpr:and_expression

and_expression
= addExpr:add_expression sp:space '&' sp:space andExpr:and_expression { return createNode( NODE_OP, OP_AND, andExpr, addExpr) }
/ addExpr:add_expression

add_expression
= mulExpr:mul_expression sp:space '+' sp:space addExpr:add_expression { return createNode(NODE_OP, OP_ADD, addExpr, mulExpr) }
/ mulExpr:mul_expression sp:space '-' sp:space addExpr:add_expression { return createNode(NODE_OP, OP_SUB, addExpr, mulExpr) }
/ mulExpr:mul_expression

mul_expression
= unExpr:unary_expression sp:space '*'  sp:space mulExpr:mul_expression  { return createNode(NODE_OP, OP_MUL, mulExpr, unExpr) }
/ unExpr:unary_expression sp:space '/' sp:space mulExpr:mul_expression { return createNode(NODE_OP, OP_DIV, mulExpr, unExpr) }
/ unExpr:unary_expression sp:space '%' sp:space mulExpr:mul_expression  { return createNode(NODE_OP, OP_MOD, mulExpr, unExpr) }
/ unExpr:unary_expression

unary_expression
= unOP:un_op primExpr:primitive_expression { return createNode(NODE_OP, OP_UNR, unOP, primExpr ) }
/ primExpr:primitive_expression

primitive_expression
= num:[0-9]+ { return createNode( NODE_CONST, num.join("") ) }
/ letter:([\'][^\'][\']) { return createNode( NODE_CONST, letter[1] ) }
/ identifier:id
/ ''
/ '(' expr:expression ')'

typeName
= type:'letter' { return createNode( NODE_TYPE, type) }
/ type:'number' { return createNode( NODE_TYPE, type) }

id
= identifier:([A-Za-z_]+) { return identifier.join("") }

function_name
= funcName:'was a ' { return funcName }
/ funcName:'became ' { return funcName }
/ funcName:'drank' { return funcName }
/ funcName:'ate' { return funcName }

newLine
= sep:([ ]'too'[\.] sp:space) {return 1}
/ sep:([\n] [\.] sp:space) {return 2}
/ sep:([\.] [\n]) {return 3 }
/ sep:([\.]) {return 4 }
/ sep:([\.] sp:space [\n]) {return 5 }
/ sep:([\,] sp:space [\n]) {return 6 }

space
= ' '* { return }

separator
= sep:([\,][ ]) {return 7}
/ sep:([ ]'then'[ ]) {return 8}
/ sep:([ ]'or'[ ]){return 9}
/ sep:([ ]'and'[ ]) {return 10}
/ sep:([ ]'too'[ ]){return 11}
/ sep:([ ]'but'[ ]){return 12}

un_op
= unOP:'~' { return createNode( NODE_OP, OP_NEG)  }
