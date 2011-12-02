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

	NODE_OP			= 0
	NODE_VAR		= 1
	NODE_CONST		= 2
	NODE_TYPE		= 3
	NODE_RETURN 	= 4
	NODE_IO			= 5
	NODE_LOOP 		= 6
	NODE_LOOP_END 	= 7
    NODE_FUN 		= 8
    NODE_FUN_DEF 	= 9
    NODE_IF			= 10
    NODE_ELSE       = 11
    NODE_FUN_CALL   = 12
    NODE_LOOK_DEF   = 13
    NODE_IF_BODY    = 14

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
	OP_LOR		= 11
	OP_LAND		= 12
	OP_EQ		= 13
	OP_UEQ		= 14
	OP_LT		= 15
	OP_GT		= 16
	OP_LET		= 17
	OP_GET		= 18

}

start
= root*

root
= loop_block
/ if_block
/ func_block
/ func_call
/ io_line
/ assignment_line

func_call
= space fun:function newLine {counter++; return createNode(NODE_FUN_CALL, "function" ,fun)}

if_block
= space name:ifFun space cond:condition space 'so' space [\n]* ife:ifelse space unsure newLine {counter++; return createNode(NODE_IF, name, cond, ife)}

ifelse
= space 'or' space [\n]* ifBody:start ife:ifelse {counter++; return createNode(NODE_ELSE, "else", ifBody, ife)}
/ space 'or maybe' space cond:condition space 'so' space [\n]* ifBody:start ife:ifelse {counter++; return createNode(NODE_ELSE, "maybe", cond, ifBody, ife)}
/ space ifBody:start {counter++; return createNode(NODE_IF_BODY, "body", ifBody)}

function
= identifier:id space '(' arglist:argument_list ')' {counter++; return createNode(NODE_FUN, identifier, arglist)}
/ arg:id space 'went through' space identifier:id {counter++; return createNode(NODE_FUN, arg, identifier)}

argument_list
= argument_fun*

argument_fun
= space arg:legalArgs separator* space { return arg }

func_block
= space 'The room' space def:function_def 'contained a' space type:typeName [\n]* funcB:start {counter++; return createNode( NODE_FUN_DEF, def, type, funcB)}
/ space 'The Looking-Glass' space identifier:id space 'changed a' space type:typeName [\n]* funcB:start {counter++; return createNode( NODE_LOOK_DEF, identifier, type, funcB)}

function_def
= identifier:id space '(' space arglist:argument_list_types space')' space {counter++; return createNode(NODE_FUN, identifier, arglist)}

argument_list_types
= argument_type*

argument_type
= space type:typeName space identifier:id [\,]* { return [identifier, type]}

loop_block
= space 'eventually' space cond:condition space 'because' space [\n]* loop:start space [\n]* space 'enough times' newLine {counter++; return createNode( NODE_LOOP, cond, loop )}

io_line
= single:io separator { return single }
/ single:io newLine { return single }

io
= space arg:legalArgs space name:function_output space 'Alice' {counter++; return createNode( NODE_IO, name, arg)}
/ space arg:legalArgs space name:function_output {counter++; return createNode(NODE_IO, name, arg)}
/ space 'Alice' space 'found' space arg:legalArgs space {counter++; return createNode(NODE_IO, 'found', arg)}
/ space name:function_input space arg:argument {counter++; return createNode(NODE_IO, name, arg)} 

legalArgs
= fun:function {return fun}
/ expr:expression {return expr}
/ arg:argument {return arg}
/ str:string {return str}

assignment_line 
= single:assignment separator { return single }
/ single:assignment newLine { return single }

assignment
= space identifier:id space name:function_name space type:typeName { counter++; return createNode( NODE_TYPE, name, identifier, type ) }
 / space arg:argument space name:function_name expr:legalArgs { counter++; return createNode( NODE_VAR, name, arg, expr ) }
 / space identifier:id space name:function_name { counter++; return createNode( NODE_VAR, name, identifier ) }
/ space identifier:id space 'had' space expr:expression space type:typeName { counter++; return createNode( NODE_TYPE, 'array', identifier, expr, type ) }

expression
= logical_or_exp

logical_or_exp
=  laExpr:logical_and_exp space '||' space loExpr:logical_or_exp { counter++; return createNode(NODE_OP, OP_LOR, laExpr, loExpr) }
/ logical_and_exp

logical_and_exp	
= orExpr:or_expression space '&&' space laExpr:logical_and_exp { counter++; return createNode(NODE_OP, OP_LAND, orExpr, laExpr) }
/ or_expression

or_expression
= xorExpr:xor_expression space '|' space orExpr:or_expression { counter++; return createNode( NODE_OP, OP_OR, xorExpr, orExpr ) }
/ xor_expression

xor_expression
= andExpr:and_expression space '^' space xorExpr:xor_expression { counter++; return createNode( NODE_OP, OP_XOR, andExpr, xorExpr ) }
/ and_expression

and_expression
= eqExpr:equality_exp space '&' space andExpr:and_expression { counter++; return createNode( NODE_OP, OP_AND, eqExpr, andExpr) }
/ equality_exp

equality_exp
= relExpr:relational_exp space '==' space eqExpr:equality_exp { counter++; return createNode(NODE_OP, OP_EQ, relExpr, eqExpr) }
/ relExpr:relational_exp space '!=' space eqExpr:equality_exp { counter++; return createNode(NODE_OP, OP_UEQ, relExpr, eqExpr) }
/ relational_exp

relational_exp
= addExpr:add_expression space '<' space relExpr:relational_exp { counter++; return createNode(NODE_OP, OP_LT, addExpr, relExpr) }
/ addExpr:add_expression space '>' space relExpr:relational_exp { counter++; return createNode(NODE_OP, OP_GT, addExpr, relExpr) }
/ addExpr:add_expression space '<=' space relExpr:relational_exp { counter++; return createNode(NODE_OP, OP_LET, addExpr, relExpr) }
/ addExpr:add_expression space '>=' space relExpr:relational_exp  { counter++; return createNode(NODE_OP, OP_GET, addExpr, relExpr) }
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
/ fun:function {return fun}
/ arg:argument {return arg}
/ str:string {return str}
/ '(' expr:expression ')'

typeName
= type:'letter'
/ type:'number'
/ type: 'sentence'

id
= identifier:[A-Za-z_]+ idd:[0-9A-Za-z_]* { return identifier.join("")+idd.join("") }

argument
= identifier:id "\'s" space expr:expression space 'piece' { return [identifier, expr]}
/ id

string
= ([\"] str:[^\"]* [\"]) { return str.join("") }

condition
= '(' space expr:expression space ')' { return expr }

function_name
= funcName:'was a ' { return funcName }
/ funcName:'became ' { return funcName }
/ funcName:'drank' { return funcName }
/ funcName:'ate' { return funcName }

function_input
= funcName:'what was' { return funcName }

function_output
= funcName:'said' { return funcName }
/ funcName:'thought' { return funcName }
/ funcName: 'spoke' { return funcName }

ifFun
= "either"
/ "perhaps"

unsure
= "Alice was unsure which"
/ "Alice was unsure"

newLine
= (space 'too' space [\.] space [\n]*) {return 1}
/ (space [\n]* space [\.\,\?] space [\n]*) {return 2 }

space
= [ \t]* { return }

separator
= ([\,] space [\n])
/ ([\,][ ]) {return 7}
/ ([?][ ])
/ ([ ]'then'[ ]) {return 8}
/ ([ ]'or'[ ]){return 9}
/ ([ ]'and'[ ]) {return 10}
/ ([ ]'too'[ ]){return 11}
/ ([ ]'but'[ ]){return 12}

not_op
= '~' { return OP_NOT }

neg_op
= '-' { return OP_NEG }