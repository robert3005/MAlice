/*
We used a parser generator Pegjs. In the top of the file there are three
functions, which are used to build a parse tree. computePosition is used
to include in the parse tree the position of particular tokens. Every
node has a unique id and type, value, position in the file. There is also
variable children where additional values are kept and the number of
children.  
*/
{

	Array.prototype.isArray = true;

	var counter = 0;

	function NODE() {
		var id;
		var type;
		var value;
		var numberOfChildren;
		var position;
		var children;
	};

      function computePosition(position) {
        var line = 1;
        var column = 1;
        var seenCR = false;
        
        for (var i = 0; i < position; i++) {
          var ch = input.charAt(i);
          if (ch === '\n') {
            if (!seenCR) { line++; }
            column = 1;
            seenCR = false;
          } else if (ch === '\r' | ch === '\u2028' || ch === '\u2029') {
            line++;
            column = 1;
            seenCR = true;
          } else {
            column++;
            seenCR = false;
          }
        }
        
        return { line: line, column: column };
      };

	//Management functions
	function createNode( tagKey, type, value, children ) {
		var n = new NODE();
		n.id = counter++;
		n.type = type;
		n.value = value;	
		n.children = new Array();
		
		for( var i = 3; i < arguments.length; i++ ) {
			if(arguments[i].isArray) {
				n.children = n.children.concat(arguments[i]);	
			} else {	
				if(arguments[i] !== '' ) {
					n.children.push( arguments[i] );
				}
			}
		}
		var position = tagKey.split('@');
		n.position = computePosition(position[1]);
		n.numberOfChildren = n.children.length;
		return n;
	};
/*
Below there is a list of node types and a list of operator types. Every time
a node is created, a type has to be specified. The same process concerns
the operations, every operation has a different type.
*/
	NODE_ROOT		= "ROOT"
	NODE_OP			= "OPERATOR"
	NODE_VAR 		= "VARIABLE"
	NODE_CONST		= "CONSTANT"
	NODE_TYPE		= "TYPE"
	NODE_RETURN		= "RETURN"
	NODE_IO			= "IO"
	NODE_LOOP 		= "WHILE"
	NODE_FUN 		= "FUNCTION"
	NODE_FUN_DEF	= "FUNCTION_DEFINITION"
	NODE_IF			= "IF"
	NODE_ELSE		= "ELSE"
	NODE_ELSE_IF	= "ELSE_IF"
	NODE_FUN_CALL	= "FUNCTION_CALL"
	NODE_LOOK_DEF	= "LOOKING_GLASS"
	NODE_END_IF		= "END_IF"
	NODE_VAR_ARRAY	= "ARRAY"
	NODE_STRING		= "STRING"

	OP_NONE		= "NO_OP"
	OP_ADD		= "ADD"
	OP_OR		= "OR"
	OP_XOR		= "XOR"
	OP_AND		= "AND"
	OP_SUB		= "SUBTRACT"
	OP_MUL		= "MULTIPLY"
	OP_DIV		= "DIVIDE"
	OP_MOD		= "MOD"
	OP_NOT		= "NOT"
	OP_NEG		= "NEGATE"
	OP_LOR		= "LOGICAL_OR"
	OP_LAND		= "LOGICAL_AND"
	OP_EQ		= "EQUAL"
	OP_UEQ		= "NOT_EQUAL"
	OP_LT		= "LESS_THAN"
	OP_GT		= "GREATER_THAN"
	OP_LTE		= "LESS_THAN_EQUAL"
	OP_GTE		= "GREATER_THAN_EQUAL"

}

/*
Pegjs looks for a particular patterns. We created 6 main types of instructions:
a loop, an if block, a function block, a function call, input/output statement
and an assignment. Each instruction has a different behaviour. Patterns occur
in the rules in the particular order, because they are matched from top to bottom.
*/

start
= program:root* { return createNode( cacheKey, NODE_ROOT, "root", program);}

root
= loop_block
/ if_block
/ func_block
/ func_call
/ io_line
/ assignment_line

/*
This part concerns functions. In the functions every type may occur except for another function definition.
*/

function_body
= instructions*

instructions
= loop_block
/ if_block
/ func_call
/ io_line
/ assignment_line

/*
If block. In the if block body every type may occur except for a function definition.
In the else block there is: another reference to function_body and an another
else case or a line which signalises the end of an if block.
*/

if_block
= space name:ifFun space cond:condition space 'so' space [\n]* space ifB:function_body ifE:ifelse { return createNode( cacheKey, NODE_IF, "if", cond, ifB, ifE)}

ifelse
= space 'or' space [\n]* ifBody:function_body ife:ifelse { return createNode( cacheKey, NODE_ELSE, "else", ifBody, ife)}
/ space 'or maybe' space cond:condition space 'so' space [\n]* ifBody:function_body ife:ifelse { return createNode( cacheKey, NODE_ELSE_IF, "else if", cond, ifBody, ife)}
/ space unsure newLine { return createNode( cacheKey, NODE_END_IF, "endif" ) }

/*
Function call finds a pattern of a type function(type var_name, type var_name ...).
argument_fun is a rule which matches all the possible legal arguments for the
functions.
*/

func_call
= fun:function newLine { return fun; }
/ fun:function separator { return fun; }

function
= space identifier:id space '(' arglist:argument_fun* ')' space { return createNode( cacheKey, NODE_FUN_CALL, "function_call", identifier, arglist)}
/ space arg:id space 'went through' space identifier:id { return createNode( cacheKey, NODE_FUN_CALL, "function_call_by_reference", arg, identifier)}

argument_fun
= space arg:legalArgs separator? space { return arg }

/*
Function block is the rule for matching two types of functions: room and looking-glass.
In the function block there might be anything besides another function definition.
Function_type matches the type which is returned by a function and argument_type matches
all the types of function's arguments.
*/

func_block
= space 'The room' space def:function_type 'contained a' space type:typeName [\n]* funcB:function_body { return createNode( cacheKey, NODE_FUN_DEF, "function_definition", type, def, funcB) }
/ space 'The Looking-Glass' space identifier:id space 'changed a' space type:typeName [\n]* funcB:function_body { return createNode( cacheKey, NODE_LOOK_DEF, identifier, type, funcB)}

function_type
= identifier:id space '(' arglist:argument_type* ')' space { return createNode( cacheKey, NODE_FUN, "function", identifier, arglist)}

argument_type
= space stype:spiderType? space type:typeName space identifier:id separator? space {  return createNode( cacheKey, NODE_TYPE, "argument", identifier, type, stype ) }

/*
Loop block contains anything besides a function definition.
*/

loop_block
= space 'eventually' space cond:condition space 'because' space [\n]* loop:function_body space [\n]* space 'enough' space 'times' newLine { return createNode( cacheKey, NODE_LOOP, "while", createNode( cackeKey, NODE_OP, OP_NOT, cond), loop )}

/*
Io block matches all the input/output statements and creates appropriate nodes.
Any of the legal arguments can be directed to standard output. Values of variables and
elements of the array can be prompted from the user. Comment segements are also
included in the io block.
*/

io_line
= single:io separator { return single }
/ single:io newLine { return single }

io
= space arg:legalArgs space name:function_output { return createNode( cacheKey, NODE_IO, name, arg)}
/ space 'Alice' space 'found' space arg:legalArgs space { return createNode( cacheKey, NODE_IO, 'found', arg)}
/ space name:function_input space arg:argument { return createNode( cacheKey, NODE_IO, name, arg)} 
/ space string space name:comment_function space 'Alice' { return createNode( cacheKey, NODE_IO, name, arg) }

/*
All the legal arguments. They are used in ouput operations, assignments and function calls.
*/

legalArgs
= fun:function {return fun}
/ expr:expression {return expr}
/ arg:argument {return arg}
/ str:string {return str}

/*
Assignments include functions 'was a', 'became', 'had', 'drank' and 'ate'.
*/

assignment_line 
= single:assignment separator { return single }
/ single:assignment newLine { return single }

assignment
= space arg:id space name:type_operator space type:typeName {  return createNode( cacheKey, NODE_FUN_CALL, name, arg, type ) }
/ space arg:argument space name:binary_operators space expr:legalArgs {  return createNode( cacheKey, NODE_FUN_CALL, name, arg, expr ) }
/ space identifier:id space name:unary_operators {  return createNode( cacheKey, NODE_FUN_CALL, name, identifier ) }
/ space identifier:id space 'had' space expr:expression space type:typeName {  return createNode( cacheKey, NODE_VAR_ARRAY, "array", identifier, expr, type ) }

/*
All the possible expressions are covered below. They are sorted in order
from the weakest to the strongest operator.
*/

expression
= logical_or_exp

logical_or_exp
=  laExpr:logical_and_exp space '||' space loExpr:logical_or_exp {  return createNode( cacheKey, NODE_OP, OP_LOR, laExpr, loExpr) }
/ logical_and_exp

logical_and_exp	
= orExpr:or_expression space '&&' space laExpr:logical_and_exp {  return createNode( cacheKey, NODE_OP, OP_LAND, orExpr, laExpr) }
/ or_expression

or_expression
= xorExpr:xor_expression space '|' space orExpr:or_expression {  return createNode( cacheKey, NODE_OP, OP_OR, xorExpr, orExpr ) }
/ xor_expression

xor_expression
= andExpr:and_expression space '^' space xorExpr:xor_expression {  return createNode( cacheKey, NODE_OP, OP_XOR, andExpr, xorExpr ) }
/ and_expression

and_expression
= eqExpr:equality_exp space '&' space andExpr:and_expression {  return createNode( cacheKey, NODE_OP, OP_AND, eqExpr, andExpr) }
/ equality_exp

equality_exp
= relExpr:relational_exp space '==' space eqExpr:equality_exp {  return createNode( cacheKey, NODE_OP, OP_EQ, relExpr, eqExpr) }
/ relExpr:relational_exp space '!=' space eqExpr:equality_exp {  return createNode( cacheKey, NODE_OP, OP_UEQ, relExpr, eqExpr) }
/ relational_exp

relational_exp
= addExpr:add_expression space '<' space relExpr:relational_exp {  return createNode( cacheKey, NODE_OP, OP_LT, addExpr, relExpr) }
/ addExpr:add_expression space '>' space relExpr:relational_exp {  return createNode( cacheKey, NODE_OP, OP_GT, addExpr, relExpr) }
/ addExpr:add_expression space '<=' space relExpr:relational_exp {  return createNode( cacheKey, NODE_OP, OP_LTE, addExpr, relExpr) }
/ addExpr:add_expression space '>=' space relExpr:relational_exp  {  return createNode( cacheKey, NODE_OP, OP_GTE, addExpr, relExpr) }
/ add_expression

add_expression
= notExpr:not_expression space '+' space addExpr:add_expression {  return createNode( cacheKey, NODE_OP, OP_ADD, notExpr, addExpr) }
/ notExpr:not_expression space '-' space addExpr:add_expression {  return createNode( cacheKey, NODE_OP, OP_SUB, notExpr, addExpr ) }
/ not_expression

not_expression
= notExpr:not_op space mulExpr:mul_expression { return createNode( cacheKey, NODE_OP, notExpr, mulExpr)}
/ mul_expression

mul_expression
= unExpr:unary_expression space '*'  space mulExpr:mul_expression  {  return createNode( cacheKey, NODE_OP, OP_MUL, unExpr, mulExpr) }
/ unExpr:unary_expression space '/' space mulExpr:mul_expression {  return createNode( cacheKey, NODE_OP, OP_DIV, unExpr, mulExpr) }
/ unExpr:unary_expression space '%' space mulExpr:mul_expression  {  return createNode( cacheKey, NODE_OP, OP_MOD, unExpr, mulExpr) }
/ unary_expression

unary_expression
= unOP:neg_op primExpr:primitive_expression {  return createNode( cacheKey, NODE_OP, unOP, primExpr ) }
/ primExpr:primitive_expression

primitive_expression
= num:[0-9]+ {  return createNode( cacheKey, NODE_CONST, num.join(""), createNode( cacheKey, NODE_TYPE, "number" ) ) }
/ letter:([\'][^\'][\']) { return createNode( cacheKey, NODE_CONST, letter[1], createNode( cacheKey, NODE_TYPE, "letter" ) ) }
/ fun:function {return fun}
/ arg:argument {return arg}
/ str:string {return str}
/ space '(' space expr:expression space ')' space { return expr; }

/*
Legal types used in declarations.
*/

typeName
= type:'letter' { return createNode( cacheKey, NODE_TYPE, type ); }
/ type:'number' { return createNode( cacheKey, NODE_TYPE, type ); }
/ type:'sentence' { return createNode( cacheKey, NODE_TYPE, type ); }
 
spiderType
= type:'spider' { return createNode( cacheKey, NODE_TYPE, type ); }

id
= identifierFirst:[A-Za-z] identifierRest:[0-9A-Za-z_]* { return createNode( cacheKey, NODE_VAR, identifierFirst + identifierRest.join("") ) }

/*
Arguments are identifiers and array's pieces.
*/

argument
= identifier:id "\'s" space expr:expression space 'piece' { return createNode( cacheKey, NODE_VAR_ARRAY, "array_element", identifier, expr ) }
/ id

string
= ([\"] str:[^\"]* [\"]) { return createNode( cacheKey, NODE_CONST, str.join(""), createNode( cacheKey, NODE_TYPE, "sentence" ) ) }

condition
= '(' space expr:expression space ')' { return expr }

binary_operators
= funcName:'became ' { return funcName }

type_operator
= funcName:'was a ' { return funcName }

unary_operators
= funcName:'drank' { return funcName }
/ funcName:'ate' { return funcName }

function_input
= funcName:'what was' { return funcName }

function_output
= funcName:'said Alice' { return funcName }
/ funcName:'spoke' { return funcName }

comment_function
= funcName:'thought' { return funcName }

ifFun
= "either"
/ "perhaps"

unsure
= "Alice was unsure which"
/ "Alice was unsure"

/*
newLine, space and separators are used to indicate white spaces and various separators.
*/

newLine
= (space [\n\t]* space [\.\,\?] space [\n\t]*) { return }
/ (space 'too' space [\.] space [\n]*) { return }
/ (space 'and' space [\n]*) { return }
/ (space 'but' space [\n]*) { return }

space
= [ \t]* { return }

separator
= ([\,] space [\n]) { return }
/ ([\,]) { return }
/ ([?][ ]) { return }
/ ([ ]'then'[ ]) { return }
/ ([ ]'and'[ ]) { return }
/ ([ ]'too'[ ]) { return }
/ ([ ]'but'[ ]) { return }

not_op
= '~' { return OP_NOT }

neg_op
= '-' { return OP_NEG }