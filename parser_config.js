[*

//Structs
function NODE()
{
	var type;
	var value;
	var children;
}

//Management functions
function createNode( type, value, childs )
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
OP_OR		= 2
OP_XOR		= 3
OP_AND		= 4
OP_SUB		= 5
OP_MUL		= 6
OP_DIV		= 7
OP_MOD		= 8
OP_UNR		= 9
OP_NEG		= 10

*]

!	' |\r|\n|\t'

	';'
	'='
	'\+'
	'\-'
	'/'
	'\*'
	'\('
	'\)'
	'\|'
	'\^'
	'\&'
	'\%'
	'#'
	'.'
	','
	'then'
	'and'
	'or'
	'too'
	'but'
	'~'
	'[A-Za-z]*'					function_name
	'letter'					number
	'number'					letter
	'[A-Za-z_][A-Za-z0-9_]*'	id
	'\'([^\']|\'\')\''			letter
	'[0-9]+'					numeral
	;

##

assignment_list:		assignment_list assignment				[* %% = createNode( NODE_OP, OP_NONE, %1, %2 ) *]
						|
						;


assignment:				id function_name type separator         [* %% = createNode( NODE_OP, %2, %1, %3) *]       
						| id function_name expression			[* %% = createNode( NODE_OP, %2, %1, %3 ) *]
						;

expression:				expression 								[* %% = createNode( NODE_OP, %1 ) *]
						| or_expression					
						;

or_expression:			xor_expression					
						| or_expression '|' xor_expression		[* %% = createNode( NODE_OP, OP_OR, %1, %3 ) *]
						;

xor_expression: 		and_expression					
						| xor_expression '^' and_expression		[* %% = createNode( NODE_OP, OP_XOR, %1, %3 ) *]
						;
 
and_expression: 		add_expression					
						| and_expression '&' add_expression		[* %% = createNode( NODE_OP, OP_AND, %1, %3 ) *]
						;

add_expression:			mul_expression					
						| add_expression '+' mul_expression		[* %% = createNode( NODE_OP, OP_ADD, %1, %3 ) *]
						| add_expression '-' mul_expression		[* %% = createNode( NODE_OP, OP_SUB, %1, %3 ) *]
						;	

mul_expression:			unary_expression
						| mul_expression '*' unary_expression	[* %% = createNode( NODE_OP, OP_MUL, %1, %3 ) *]
						| mul_expression '/' unary_expression	[* %% = createNode( NODE_OP, OP_DIV, %1, %3 ) *]
						| mul_expression '%' unary_expression	[* %% = createNode( NODE_OP, OP_MOD, %1, %3 ) *]
						;

unary_expression:		primitive_expression				
						| un_op primitive_expression			[* %% = createNode( NODE_OP, OP_UNR, %1, %2 ) *]
						;

primitive_expression:	id 										[* %% = createNode( NODE_VAR, %1 ) *]
						| numeral 								[* %% = createNode( NODE_CONST, %1 ) *]
						| letter 								[* %% = createNode( NODE_CONST, %1 ) *]
						| '(' expression ')' 					[* %% = %2; *]
						|
						;

type:					| 'letter' 								[* %% = createNode( NODE_TYPE, %1 ) *]
						| 'number'								[* %% = createNode( NODE_TYPE, %1 ) *]
						;

separator:				'.' 									[* %% = createNode( NODE_OP, OP_NONE ) *]
						| ',' 									[* %% = createNode( NODE_OP, OP_NONE ) *]
						| 'then' 								[* %% = createNode( NODE_OP, OP_NONE ) *]
						| 'or' 									[* %% = createNode( NODE_OP, OP_NONE ) *]
						| 'and' 								[* %% = createNode( NODE_OP, OP_NONE ) *]
						| 'too' 								[* %% = createNode( NODE_OP, OP_NONE ) *]
						| 'but'									[* %% = createNode( NODE_OP, OP_NONE ) *]
						;

un_op:					'~'										[* %% = createNode( NODE_OP, OP_NEG) *]
						;

[*

var str = prompt( "Please enter an XPL-script to be executed:",
	"say \'Hello World\'; i = 0; do { write i; i = i + 1; } while i < 10;" );
var error_cnt 	= 0;
var error_off	= new Array();
var error_la	= new Array();

if( ( error_cnt = __NODEJS_parse( str, error_off, error_la ) ) > 0 )
{
	for( i = 0; i < error_cnt; i++ )
		alert( "Parse error near >" 
			+ str.substr( error_off[i], 30 ) + "<, expecting \"" + error_la[i].join() + "\"" );
}
*]