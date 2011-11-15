

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


/*
	Default template driver for JS/CC generated parsers running as
	browser-based JavaScript/ECMAScript applications.
	
	WARNING: 	This parser template will not run as console and has lesser
				features for debugging than the console derivates for the
				various JavaScript platforms.
	
	Features:
	- Parser trace messages
	- Integrated panic-mode error recovery
	
	Written 2007, 2008 by Jan Max Meyer, J.M.K S.F. Software Technologies
	
	This is in the public domain.
*/

var NODEJS__dbg_withtrace		= true;
var NODEJS__dbg_string			= new String();
if(NODEJS__dbg_withtrace){
	var fd = require("fs").openSync("NODEJS__dbg_withtrace.log", "w+");
	require('fs').writeSync(fd, new Date );
}
function __NODEJS_dbg_print( text )
{
	NODEJS__dbg_string += text + "\n";
}

function __NODEJS_lex( info )
{
	var state		= 0;
	var match		= -1;
	var match_pos	= 0;
	var start		= 0;
	var pos			= info.offset + 1;

	do
	{
		pos--;
		state = 0;
		match = -2;
		start = pos;

		if( info.src.length <= start )
			return 41;

		do
		{

switch( state )
{
	case 0:
		if( info.src.charCodeAt( pos ) == 39 ) state = 20;
		else state = -1;
		break;

	case 1:
		if( ( info.src.charCodeAt( pos ) >= 0 && info.src.charCodeAt( pos ) <= 8 ) || ( info.src.charCodeAt( pos ) >= 11 && info.src.charCodeAt( pos ) <= 12 ) || ( info.src.charCodeAt( pos ) >= 14 && info.src.charCodeAt( pos ) <= 31 ) || ( info.src.charCodeAt( pos ) >= 33 && info.src.charCodeAt( pos ) <= 34 ) || info.src.charCodeAt( pos ) == 36 || info.src.charCodeAt( pos ) == 44 || info.src.charCodeAt( pos ) == 46 || info.src.charCodeAt( pos ) == 58 || info.src.charCodeAt( pos ) == 60 || ( info.src.charCodeAt( pos ) >= 62 && info.src.charCodeAt( pos ) <= 64 ) || ( info.src.charCodeAt( pos ) >= 91 && info.src.charCodeAt( pos ) <= 93 ) || info.src.charCodeAt( pos ) == 96 || info.src.charCodeAt( pos ) == 123 || ( info.src.charCodeAt( pos ) >= 125 && info.src.charCodeAt( pos ) <= 254 ) ) state = 2;
		else if( ( info.src.charCodeAt( pos ) >= 9 && info.src.charCodeAt( pos ) <= 10 ) || info.src.charCodeAt( pos ) == 13 || info.src.charCodeAt( pos ) == 32 ) state = 3;
		else if( info.src.charCodeAt( pos ) == 35 ) state = 4;
		else if( info.src.charCodeAt( pos ) == 37 ) state = 5;
		else if( info.src.charCodeAt( pos ) == 38 ) state = 6;
		else if( info.src.charCodeAt( pos ) == 40 ) state = 7;
		else if( info.src.charCodeAt( pos ) == 41 ) state = 8;
		else if( info.src.charCodeAt( pos ) == 42 ) state = 9;
		else if( info.src.charCodeAt( pos ) == 43 ) state = 10;
		else if( info.src.charCodeAt( pos ) == 45 ) state = 11;
		else if( info.src.charCodeAt( pos ) == 47 ) state = 12;
		else if( info.src.charCodeAt( pos ) == 59 ) state = 13;
		else if( info.src.charCodeAt( pos ) == 61 ) state = 14;
		else if( info.src.charCodeAt( pos ) == 94 ) state = 15;
		else if( info.src.charCodeAt( pos ) == 124 ) state = 16;
		else if( info.src.charCodeAt( pos ) == 39 ) state = 27;
		else if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) ) state = 29;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 99 && info.src.charCodeAt( pos ) <= 110 ) || ( info.src.charCodeAt( pos ) >= 112 && info.src.charCodeAt( pos ) <= 115 ) || ( info.src.charCodeAt( pos ) >= 117 && info.src.charCodeAt( pos ) <= 122 ) ) state = 31;
		else if( info.src.charCodeAt( pos ) == 95 ) state = 33;
		else if( info.src.charCodeAt( pos ) == 97 ) state = 35;
		else if( info.src.charCodeAt( pos ) == 98 ) state = 36;
		else if( info.src.charCodeAt( pos ) == 111 ) state = 37;
		else if( info.src.charCodeAt( pos ) == 116 ) state = 38;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 2:
		state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 3:
		state = -1;
		match = 1;
		match_pos = pos;
		break;

	case 4:
		state = -1;
		match = 14;
		match_pos = pos;
		break;

	case 5:
		state = -1;
		match = 13;
		match_pos = pos;
		break;

	case 6:
		state = -1;
		match = 12;
		match_pos = pos;
		break;

	case 7:
		state = -1;
		match = 8;
		match_pos = pos;
		break;

	case 8:
		state = -1;
		match = 9;
		match_pos = pos;
		break;

	case 9:
		state = -1;
		match = 7;
		match_pos = pos;
		break;

	case 10:
		state = -1;
		match = 4;
		match_pos = pos;
		break;

	case 11:
		state = -1;
		match = 5;
		match_pos = pos;
		break;

	case 12:
		state = -1;
		match = 6;
		match_pos = pos;
		break;

	case 13:
		state = -1;
		match = 2;
		match_pos = pos;
		break;

	case 14:
		state = -1;
		match = 3;
		match_pos = pos;
		break;

	case 15:
		state = -1;
		match = 11;
		match_pos = pos;
		break;

	case 16:
		state = -1;
		match = 10;
		match_pos = pos;
		break;

	case 17:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) ) state = 17;
		else state = -1;
		match = 27;
		match_pos = pos;
		break;

	case 18:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || info.src.charCodeAt( pos ) == 95 || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 18;
		else state = -1;
		match = 26;
		match_pos = pos;
		break;

	case 19:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 19;
		match_pos = pos;
		break;

	case 20:
		state = -1;
		match = 25;
		match_pos = pos;
		break;

	case 21:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 18;
		match_pos = pos;
		break;

	case 22:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 21;
		match_pos = pos;
		break;

	case 23:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 20;
		match_pos = pos;
		break;

	case 24:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 17;
		match_pos = pos;
		break;

	case 25:
		if( info.src.charCodeAt( pos ) == 39 ) state = 0;
		else state = -1;
		break;

	case 26:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 27:
		if( ( info.src.charCodeAt( pos ) >= 0 && info.src.charCodeAt( pos ) <= 38 ) || ( info.src.charCodeAt( pos ) >= 40 && info.src.charCodeAt( pos ) <= 254 ) ) state = 0;
		else if( info.src.charCodeAt( pos ) == 39 ) state = 25;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 28:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( info.src.charCodeAt( pos ) == 100 ) state = 21;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 99 ) || ( info.src.charCodeAt( pos ) >= 101 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 29:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) ) state = 17;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 30:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( info.src.charCodeAt( pos ) == 116 ) state = 22;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 115 ) || ( info.src.charCodeAt( pos ) >= 117 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 31:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 32:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( info.src.charCodeAt( pos ) == 111 ) state = 23;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 110 ) || ( info.src.charCodeAt( pos ) >= 112 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 33:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || info.src.charCodeAt( pos ) == 95 || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 122 ) ) state = 18;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 34:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( info.src.charCodeAt( pos ) == 110 ) state = 24;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 109 ) || ( info.src.charCodeAt( pos ) >= 111 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 35:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 109 ) || ( info.src.charCodeAt( pos ) >= 111 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else if( info.src.charCodeAt( pos ) == 110 ) state = 28;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 36:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 116 ) || ( info.src.charCodeAt( pos ) >= 118 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else if( info.src.charCodeAt( pos ) == 117 ) state = 30;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 37:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( info.src.charCodeAt( pos ) == 114 ) state = 19;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 113 ) || ( info.src.charCodeAt( pos ) >= 115 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 38:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 103 ) || ( info.src.charCodeAt( pos ) >= 105 && info.src.charCodeAt( pos ) <= 110 ) || ( info.src.charCodeAt( pos ) >= 112 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else if( info.src.charCodeAt( pos ) == 111 ) state = 32;
		else if( info.src.charCodeAt( pos ) == 104 ) state = 39;
		else state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 39:
		if( ( info.src.charCodeAt( pos ) >= 48 && info.src.charCodeAt( pos ) <= 57 ) || info.src.charCodeAt( pos ) == 95 ) state = 18;
		else if( ( info.src.charCodeAt( pos ) >= 65 && info.src.charCodeAt( pos ) <= 90 ) || ( info.src.charCodeAt( pos ) >= 97 && info.src.charCodeAt( pos ) <= 100 ) || ( info.src.charCodeAt( pos ) >= 102 && info.src.charCodeAt( pos ) <= 122 ) ) state = 26;
		else if( info.src.charCodeAt( pos ) == 101 ) state = 34;
		else state = -1;
		match = 23;
		match_pos = pos;
		break;

}


			pos++;

		}
		while( state > -1 );

	}
	while( 1 > -1 && match == 1 );

	if( match > -1 )
	{
		info.att = info.src.substr( start, match_pos - start );
		info.offset = match_pos;
		

	}
	else
	{
		info.att = new String();
		match = -1;
	}

	return match;
}


function __NODEJS_parse( src, err_off, err_la )
{
	var		sstack			= new Array();
	var		vstack			= new Array();
	var 	err_cnt			= 0;
	var		act;
	var		go;
	var		la;
	var		rval;
	var 	parseinfo		= new Function( "", "var offset; var src; var att;" );
	var		info			= new parseinfo();
	
/* Pop-Table */
var pop_tab = new Array(
	new Array( 0/* assignment_list' */, 1 ),
	new Array( 28/* assignment_list */, 2 ),
	new Array( 28/* assignment_list */, 0 ),
	new Array( 29/* assignment */, 4 ),
	new Array( 29/* assignment */, 3 ),
	new Array( 32/* expression */, 1 ),
	new Array( 32/* expression */, 1 ),
	new Array( 33/* or_expression */, 1 ),
	new Array( 33/* or_expression */, 3 ),
	new Array( 34/* xor_expression */, 1 ),
	new Array( 34/* xor_expression */, 3 ),
	new Array( 35/* and_expression */, 1 ),
	new Array( 35/* and_expression */, 3 ),
	new Array( 36/* add_expression */, 1 ),
	new Array( 36/* add_expression */, 3 ),
	new Array( 36/* add_expression */, 3 ),
	new Array( 37/* mul_expression */, 1 ),
	new Array( 37/* mul_expression */, 3 ),
	new Array( 37/* mul_expression */, 3 ),
	new Array( 37/* mul_expression */, 3 ),
	new Array( 38/* unary_expression */, 1 ),
	new Array( 38/* unary_expression */, 2 ),
	new Array( 39/* primitive_expression */, 1 ),
	new Array( 39/* primitive_expression */, 1 ),
	new Array( 39/* primitive_expression */, 1 ),
	new Array( 39/* primitive_expression */, 3 ),
	new Array( 39/* primitive_expression */, 0 ),
	new Array( 30/* type */, 0 ),
	new Array( 30/* type */, 1 ),
	new Array( 30/* type */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 31/* separator */, 1 ),
	new Array( 40/* un_op */, 1 )
);

/* Action-Table */
var act_tab = new Array(
	/* State 0 */ new Array( 41/* "$" */,-2 , 26/* "id" */,-2 ),
	/* State 1 */ new Array( 26/* "id" */,3 , 41/* "$" */,0 ),
	/* State 2 */ new Array( 41/* "$" */,-1 , 26/* "id" */,-1 ),
	/* State 3 */ new Array( 23/* "function_name" */,4 ),
	/* State 4 */ new Array( 25/* "letter" */,8 , 24/* "number" */,9 , 26/* "id" */,17 , 27/* "numeral" */,18 , 8/* "(" */,19 , 22/* "~" */,20 , 15/* "." */,-27 , 16/* "," */,-27 , 17/* "then" */,-27 , 19/* "or" */,-27 , 18/* "and" */,-27 , 20/* "too" */,-27 , 21/* "but" */,-27 , 41/* "$" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 ),
	/* State 5 */ new Array( 15/* "." */,22 , 16/* "," */,23 , 17/* "then" */,24 , 19/* "or" */,25 , 18/* "and" */,26 , 20/* "too" */,27 , 21/* "but" */,28 ),
	/* State 6 */ new Array( 41/* "$" */,-4 , 26/* "id" */,-4 ),
	/* State 7 */ new Array( 10/* "|" */,29 , 41/* "$" */,-6 , 26/* "id" */,-6 , 9/* ")" */,-6 ),
	/* State 8 */ new Array( 15/* "." */,-28 , 16/* "," */,-28 , 17/* "then" */,-28 , 19/* "or" */,-28 , 18/* "and" */,-28 , 20/* "too" */,-28 , 21/* "but" */,-28 , 41/* "$" */,-24 , 26/* "id" */,-24 , 7/* "*" */,-24 , 6/* "/" */,-24 , 13/* "%" */,-24 , 4/* "+" */,-24 , 5/* "-" */,-24 , 12/* "&" */,-24 , 11/* "^" */,-24 , 10/* "|" */,-24 ),
	/* State 9 */ new Array( 15/* "." */,-29 , 16/* "," */,-29 , 17/* "then" */,-29 , 19/* "or" */,-29 , 18/* "and" */,-29 , 20/* "too" */,-29 , 21/* "but" */,-29 ),
	/* State 10 */ new Array( 11/* "^" */,30 , 41/* "$" */,-7 , 26/* "id" */,-7 , 10/* "|" */,-7 , 9/* ")" */,-7 ),
	/* State 11 */ new Array( 12/* "&" */,31 , 41/* "$" */,-9 , 26/* "id" */,-9 , 11/* "^" */,-9 , 10/* "|" */,-9 , 9/* ")" */,-9 ),
	/* State 12 */ new Array( 5/* "-" */,32 , 4/* "+" */,33 , 41/* "$" */,-11 , 26/* "id" */,-11 , 12/* "&" */,-11 , 11/* "^" */,-11 , 10/* "|" */,-11 , 9/* ")" */,-11 ),
	/* State 13 */ new Array( 13/* "%" */,34 , 6/* "/" */,35 , 7/* "*" */,36 , 41/* "$" */,-13 , 26/* "id" */,-13 , 4/* "+" */,-13 , 5/* "-" */,-13 , 12/* "&" */,-13 , 11/* "^" */,-13 , 10/* "|" */,-13 , 9/* ")" */,-13 ),
	/* State 14 */ new Array( 41/* "$" */,-16 , 26/* "id" */,-16 , 7/* "*" */,-16 , 6/* "/" */,-16 , 13/* "%" */,-16 , 4/* "+" */,-16 , 5/* "-" */,-16 , 12/* "&" */,-16 , 11/* "^" */,-16 , 10/* "|" */,-16 , 9/* ")" */,-16 ),
	/* State 15 */ new Array( 41/* "$" */,-20 , 26/* "id" */,-20 , 7/* "*" */,-20 , 6/* "/" */,-20 , 13/* "%" */,-20 , 4/* "+" */,-20 , 5/* "-" */,-20 , 12/* "&" */,-20 , 11/* "^" */,-20 , 10/* "|" */,-20 , 9/* ")" */,-20 ),
	/* State 16 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 41/* "$" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 ),
	/* State 17 */ new Array( 41/* "$" */,-22 , 26/* "id" */,-22 , 7/* "*" */,-22 , 6/* "/" */,-22 , 13/* "%" */,-22 , 4/* "+" */,-22 , 5/* "-" */,-22 , 12/* "&" */,-22 , 11/* "^" */,-22 , 10/* "|" */,-22 , 9/* ")" */,-22 ),
	/* State 18 */ new Array( 41/* "$" */,-23 , 26/* "id" */,-23 , 7/* "*" */,-23 , 6/* "/" */,-23 , 13/* "%" */,-23 , 4/* "+" */,-23 , 5/* "-" */,-23 , 12/* "&" */,-23 , 11/* "^" */,-23 , 10/* "|" */,-23 , 9/* ")" */,-23 ),
	/* State 19 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 9/* ")" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 ),
	/* State 20 */ new Array( 26/* "id" */,-37 , 27/* "numeral" */,-37 , 25/* "letter" */,-37 , 8/* "(" */,-37 , 41/* "$" */,-37 , 7/* "*" */,-37 , 6/* "/" */,-37 , 13/* "%" */,-37 , 4/* "+" */,-37 , 5/* "-" */,-37 , 12/* "&" */,-37 , 11/* "^" */,-37 , 10/* "|" */,-37 , 9/* ")" */,-37 ),
	/* State 21 */ new Array( 41/* "$" */,-3 , 26/* "id" */,-3 ),
	/* State 22 */ new Array( 41/* "$" */,-30 , 26/* "id" */,-30 ),
	/* State 23 */ new Array( 41/* "$" */,-31 , 26/* "id" */,-31 ),
	/* State 24 */ new Array( 41/* "$" */,-32 , 26/* "id" */,-32 ),
	/* State 25 */ new Array( 41/* "$" */,-33 , 26/* "id" */,-33 ),
	/* State 26 */ new Array( 41/* "$" */,-34 , 26/* "id" */,-34 ),
	/* State 27 */ new Array( 41/* "$" */,-35 , 26/* "id" */,-35 ),
	/* State 28 */ new Array( 41/* "$" */,-36 , 26/* "id" */,-36 ),
	/* State 29 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 ),
	/* State 30 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 ),
	/* State 31 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 ),
	/* State 32 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 ),
	/* State 33 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 ),
	/* State 34 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 ),
	/* State 35 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 ),
	/* State 36 */ new Array( 26/* "id" */,17 , 27/* "numeral" */,18 , 25/* "letter" */,38 , 8/* "(" */,19 , 22/* "~" */,20 , 41/* "$" */,-26 , 7/* "*" */,-26 , 6/* "/" */,-26 , 13/* "%" */,-26 , 4/* "+" */,-26 , 5/* "-" */,-26 , 12/* "&" */,-26 , 11/* "^" */,-26 , 10/* "|" */,-26 , 9/* ")" */,-26 ),
	/* State 37 */ new Array( 41/* "$" */,-21 , 26/* "id" */,-21 , 7/* "*" */,-21 , 6/* "/" */,-21 , 13/* "%" */,-21 , 4/* "+" */,-21 , 5/* "-" */,-21 , 12/* "&" */,-21 , 11/* "^" */,-21 , 10/* "|" */,-21 , 9/* ")" */,-21 ),
	/* State 38 */ new Array( 41/* "$" */,-24 , 26/* "id" */,-24 , 7/* "*" */,-24 , 6/* "/" */,-24 , 13/* "%" */,-24 , 4/* "+" */,-24 , 5/* "-" */,-24 , 12/* "&" */,-24 , 11/* "^" */,-24 , 10/* "|" */,-24 , 9/* ")" */,-24 ),
	/* State 39 */ new Array( 9/* ")" */,48 ),
	/* State 40 */ new Array( 11/* "^" */,30 , 41/* "$" */,-8 , 26/* "id" */,-8 , 10/* "|" */,-8 , 9/* ")" */,-8 ),
	/* State 41 */ new Array( 12/* "&" */,31 , 41/* "$" */,-10 , 26/* "id" */,-10 , 11/* "^" */,-10 , 10/* "|" */,-10 , 9/* ")" */,-10 ),
	/* State 42 */ new Array( 5/* "-" */,32 , 4/* "+" */,33 , 41/* "$" */,-12 , 26/* "id" */,-12 , 12/* "&" */,-12 , 11/* "^" */,-12 , 10/* "|" */,-12 , 9/* ")" */,-12 ),
	/* State 43 */ new Array( 13/* "%" */,34 , 6/* "/" */,35 , 7/* "*" */,36 , 41/* "$" */,-15 , 26/* "id" */,-15 , 4/* "+" */,-15 , 5/* "-" */,-15 , 12/* "&" */,-15 , 11/* "^" */,-15 , 10/* "|" */,-15 , 9/* ")" */,-15 ),
	/* State 44 */ new Array( 13/* "%" */,34 , 6/* "/" */,35 , 7/* "*" */,36 , 41/* "$" */,-14 , 26/* "id" */,-14 , 4/* "+" */,-14 , 5/* "-" */,-14 , 12/* "&" */,-14 , 11/* "^" */,-14 , 10/* "|" */,-14 , 9/* ")" */,-14 ),
	/* State 45 */ new Array( 41/* "$" */,-19 , 26/* "id" */,-19 , 7/* "*" */,-19 , 6/* "/" */,-19 , 13/* "%" */,-19 , 4/* "+" */,-19 , 5/* "-" */,-19 , 12/* "&" */,-19 , 11/* "^" */,-19 , 10/* "|" */,-19 , 9/* ")" */,-19 ),
	/* State 46 */ new Array( 41/* "$" */,-18 , 26/* "id" */,-18 , 7/* "*" */,-18 , 6/* "/" */,-18 , 13/* "%" */,-18 , 4/* "+" */,-18 , 5/* "-" */,-18 , 12/* "&" */,-18 , 11/* "^" */,-18 , 10/* "|" */,-18 , 9/* ")" */,-18 ),
	/* State 47 */ new Array( 41/* "$" */,-17 , 26/* "id" */,-17 , 7/* "*" */,-17 , 6/* "/" */,-17 , 13/* "%" */,-17 , 4/* "+" */,-17 , 5/* "-" */,-17 , 12/* "&" */,-17 , 11/* "^" */,-17 , 10/* "|" */,-17 , 9/* ")" */,-17 ),
	/* State 48 */ new Array( 41/* "$" */,-25 , 26/* "id" */,-25 , 7/* "*" */,-25 , 6/* "/" */,-25 , 13/* "%" */,-25 , 4/* "+" */,-25 , 5/* "-" */,-25 , 12/* "&" */,-25 , 11/* "^" */,-25 , 10/* "|" */,-25 , 9/* ")" */,-25 )
);

/* Goto-Table */
var goto_tab = new Array(
	/* State 0 */ new Array( 28/* assignment_list */,1 ),
	/* State 1 */ new Array( 29/* assignment */,2 ),
	/* State 2 */ new Array(  ),
	/* State 3 */ new Array(  ),
	/* State 4 */ new Array( 30/* type */,5 , 32/* expression */,6 , 33/* or_expression */,7 , 34/* xor_expression */,10 , 35/* and_expression */,11 , 36/* add_expression */,12 , 37/* mul_expression */,13 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 5 */ new Array( 31/* separator */,21 ),
	/* State 6 */ new Array(  ),
	/* State 7 */ new Array(  ),
	/* State 8 */ new Array(  ),
	/* State 9 */ new Array(  ),
	/* State 10 */ new Array(  ),
	/* State 11 */ new Array(  ),
	/* State 12 */ new Array(  ),
	/* State 13 */ new Array(  ),
	/* State 14 */ new Array(  ),
	/* State 15 */ new Array(  ),
	/* State 16 */ new Array( 39/* primitive_expression */,37 ),
	/* State 17 */ new Array(  ),
	/* State 18 */ new Array(  ),
	/* State 19 */ new Array( 32/* expression */,39 , 33/* or_expression */,7 , 34/* xor_expression */,10 , 35/* and_expression */,11 , 36/* add_expression */,12 , 37/* mul_expression */,13 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 20 */ new Array(  ),
	/* State 21 */ new Array(  ),
	/* State 22 */ new Array(  ),
	/* State 23 */ new Array(  ),
	/* State 24 */ new Array(  ),
	/* State 25 */ new Array(  ),
	/* State 26 */ new Array(  ),
	/* State 27 */ new Array(  ),
	/* State 28 */ new Array(  ),
	/* State 29 */ new Array( 34/* xor_expression */,40 , 35/* and_expression */,11 , 36/* add_expression */,12 , 37/* mul_expression */,13 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 30 */ new Array( 35/* and_expression */,41 , 36/* add_expression */,12 , 37/* mul_expression */,13 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 31 */ new Array( 36/* add_expression */,42 , 37/* mul_expression */,13 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 32 */ new Array( 37/* mul_expression */,43 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 33 */ new Array( 37/* mul_expression */,44 , 38/* unary_expression */,14 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 34 */ new Array( 38/* unary_expression */,45 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 35 */ new Array( 38/* unary_expression */,46 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 36 */ new Array( 38/* unary_expression */,47 , 39/* primitive_expression */,15 , 40/* un_op */,16 ),
	/* State 37 */ new Array(  ),
	/* State 38 */ new Array(  ),
	/* State 39 */ new Array(  ),
	/* State 40 */ new Array(  ),
	/* State 41 */ new Array(  ),
	/* State 42 */ new Array(  ),
	/* State 43 */ new Array(  ),
	/* State 44 */ new Array(  ),
	/* State 45 */ new Array(  ),
	/* State 46 */ new Array(  ),
	/* State 47 */ new Array(  ),
	/* State 48 */ new Array(  )
);



/* Symbol labels */
var labels = new Array(
	"assignment_list'" /* Non-terminal symbol */,
	"WHITESPACE" /* Terminal symbol */,
	";" /* Terminal symbol */,
	"=" /* Terminal symbol */,
	"+" /* Terminal symbol */,
	"-" /* Terminal symbol */,
	"/" /* Terminal symbol */,
	"*" /* Terminal symbol */,
	"(" /* Terminal symbol */,
	")" /* Terminal symbol */,
	"|" /* Terminal symbol */,
	"^" /* Terminal symbol */,
	"&" /* Terminal symbol */,
	"%" /* Terminal symbol */,
	"#" /* Terminal symbol */,
	"." /* Terminal symbol */,
	"," /* Terminal symbol */,
	"then" /* Terminal symbol */,
	"and" /* Terminal symbol */,
	"or" /* Terminal symbol */,
	"too" /* Terminal symbol */,
	"but" /* Terminal symbol */,
	"~" /* Terminal symbol */,
	"function_name" /* Terminal symbol */,
	"number" /* Terminal symbol */,
	"letter" /* Terminal symbol */,
	"id" /* Terminal symbol */,
	"numeral" /* Terminal symbol */,
	"assignment_list" /* Non-terminal symbol */,
	"assignment" /* Non-terminal symbol */,
	"type" /* Non-terminal symbol */,
	"separator" /* Non-terminal symbol */,
	"expression" /* Non-terminal symbol */,
	"or_expression" /* Non-terminal symbol */,
	"xor_expression" /* Non-terminal symbol */,
	"and_expression" /* Non-terminal symbol */,
	"add_expression" /* Non-terminal symbol */,
	"mul_expression" /* Non-terminal symbol */,
	"unary_expression" /* Non-terminal symbol */,
	"primitive_expression" /* Non-terminal symbol */,
	"un_op" /* Non-terminal symbol */,
	"$" /* Terminal symbol */
);


	
	info.offset = 0;
	info.src = src;
	info.att = new String();
	
	if( !err_off )
		err_off	= new Array();
	if( !err_la )
	err_la = new Array();
	
	sstack.push( 0 );
	vstack.push( 0 );
	
	la = __NODEJS_lex( info );
	while( true )
	{
		act = 50;
		for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
		{
			if( act_tab[sstack[sstack.length-1]][i] == la )
			{
				act = act_tab[sstack[sstack.length-1]][i+1];
				break;
			}
		}

		if( NODEJS__dbg_withtrace && sstack.length > 0 )
		{
			__NODEJS_dbg_print( "\nState " + sstack[sstack.length-1] + "\n" +
							"\tLookahead: " + labels[la] + " (\"" + info.att + "\")\n" +
							"\tAction: " + act + "\n" + 
							"\tSource: \"" + info.src.substr( info.offset, 30 ) + ( ( info.offset + 30 < info.src.length ) ?
									"..." : "" ) + "\"\n" +
							"\tStack: " + sstack.join() + "\n" +
							"\tValue stack: " + vstack.join() + "\n" );
		}
		
			
		//Panic-mode: Try recovery when parse-error occurs!
		if( act == 50 )
		{
			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "Error detected: There is no reduce or shift on the symbol " + labels[la] );
			
			err_cnt++;
			err_off.push( info.offset - info.att.length );			
			err_la.push( new Array() );
			for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
				err_la[err_la.length-1].push( labels[act_tab[sstack[sstack.length-1]][i]] );
			
			//Remember the original stack!
			var rsstack = new Array();
			var rvstack = new Array();
			for( var i = 0; i < sstack.length; i++ )
			{
				rsstack[i] = sstack[i];
				rvstack[i] = vstack[i];
			}
			
			while( act == 50 && la != 41 )
			{
				if( NODEJS__dbg_withtrace )
					__NODEJS_dbg_print( "\tError recovery\n" +
									"Current lookahead: " + labels[la] + " (" + info.att + ")\n" +
									"Action: " + act + "\n\n" );
				if( la == -1 )
					info.offset++;
					
				while( act == 50 && sstack.length > 0 )
				{
					sstack.pop();
					vstack.pop();
					
					if( sstack.length == 0 )
						break;
						
					act = 50;
					for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
					{
						if( act_tab[sstack[sstack.length-1]][i] == la )
						{
							act = act_tab[sstack[sstack.length-1]][i+1];
							break;
						}
					}
				}
				
				if( act != 50 )
					break;
				
				for( var i = 0; i < rsstack.length; i++ )
				{
					sstack.push( rsstack[i] );
					vstack.push( rvstack[i] );
				}
				
				la = __NODEJS_lex( info );
			}
			
			if( act == 50 )
			{
				if( NODEJS__dbg_withtrace )
					__NODEJS_dbg_print( "\tError recovery failed, terminating parse process..." );
				break;
			}


			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "\tError recovery succeeded, continuing" );
		}
		
		/*
		if( act == 50 )
			break;
		*/
		
		
		//Shift
		if( act > 0 )
		{			
			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "Shifting symbol: " + labels[la] + " (" + info.att + ")" );
		
			sstack.push( act );
			vstack.push( info.att );
			
			la = __NODEJS_lex( info );
			
			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "\tNew lookahead symbol: " + labels[la] + " (" + info.att + ")" );
		}
		//Reduce
		else
		{		
			act *= -1;
			
			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "Reducing by producution: " + act );
			
			rval = void(0);
			
			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "\tPerforming semantic action..." );
			
switch( act )
{
	case 0:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 1:
	{
		 rval = createNode( NODE_OP, OP_NONE, vstack[ vstack.length - 2 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 2:
	{
		rval = vstack[ vstack.length - 0 ];
	}
	break;
	case 3:
	{
		 rval = createNode( NODE_OP, vstack[ vstack.length - 3 ], vstack[ vstack.length - 4 ], vstack[ vstack.length - 2 ]) 
	}
	break;
	case 4:
	{
		 rval = createNode( NODE_OP, vstack[ vstack.length - 2 ], vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 5:
	{
		 rval = createNode( NODE_OP, vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 6:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 7:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 8:
	{
		 rval = createNode( NODE_OP, OP_OR, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 9:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 10:
	{
		 rval = createNode( NODE_OP, OP_XOR, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 11:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 12:
	{
		 rval = createNode( NODE_OP, OP_AND, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 13:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 14:
	{
		 rval = createNode( NODE_OP, OP_ADD, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 15:
	{
		 rval = createNode( NODE_OP, OP_SUB, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 16:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 17:
	{
		 rval = createNode( NODE_OP, OP_MUL, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 18:
	{
		 rval = createNode( NODE_OP, OP_DIV, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 19:
	{
		 rval = createNode( NODE_OP, OP_MOD, vstack[ vstack.length - 3 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 20:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 21:
	{
		 rval = createNode( NODE_OP, OP_UNR, vstack[ vstack.length - 2 ], vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 22:
	{
		 rval = createNode( NODE_VAR, vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 23:
	{
		 rval = createNode( NODE_CONST, vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 24:
	{
		 rval = createNode( NODE_CONST, vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 25:
	{
		 rval = vstack[ vstack.length - 2 ]; 
	}
	break;
	case 26:
	{
		rval = vstack[ vstack.length - 0 ];
	}
	break;
	case 27:
	{
		rval = vstack[ vstack.length - 0 ];
	}
	break;
	case 28:
	{
		 rval = createNode( NODE_TYPE, vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 29:
	{
		 rval = createNode( NODE_TYPE, vstack[ vstack.length - 1 ] ) 
	}
	break;
	case 30:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 31:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 32:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 33:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 34:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 35:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 36:
	{
		 rval = createNode( NODE_OP, OP_NONE ) 
	}
	break;
	case 37:
	{
		 rval = createNode( NODE_OP, OP_NEG) 
	}
	break;
}



			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "\tPopping " + pop_tab[act][1] + " off the stack..." );
				
			for( var i = 0; i < pop_tab[act][1]; i++ )
			{
				sstack.pop();
				vstack.pop();
			}
									
			go = -1;
			for( var i = 0; i < goto_tab[sstack[sstack.length-1]].length; i+=2 )
			{
				if( goto_tab[sstack[sstack.length-1]][i] == pop_tab[act][0] )
				{
					go = goto_tab[sstack[sstack.length-1]][i+1];
					break;
				}
			}
			
			if( act == 0 )
				break;
				
			if( NODEJS__dbg_withtrace )
				__NODEJS_dbg_print( "\tPushing non-terminal " + labels[ pop_tab[act][0] ] );
				
			sstack.push( go );
			vstack.push( rval );			
		}
		
		if( NODEJS__dbg_withtrace )
		{	
				
			require('fs').writeSync(fd, NODEJS__dbg_string );
			NODEJS__dbg_string = new String();
		}
	}

	if( NODEJS__dbg_withtrace )
	{
		__NODEJS_dbg_print( "\nParse complete." );
		require('fs').writeSync(fd, NODEJS__dbg_string );
		NODEJS__dbg_string = new String();
	}
	
	return err_cnt;
}




var str =  "x was a number and x became 42. y was a number, y became 30. z was a number but z became x + y. z spoke."
var error_cnt 	= 0;
var error_off	= new Array();
var error_la	= new Array();

if( ( error_cnt = __NODEJS_parse( str, error_off, error_la ) ) > 0 )
{
	for( i = 0; i < error_cnt; i++ )
		console.log( "Parse error near >" 
			+ str.substr( error_off[i], 30 ) + "<, expecting \"" + error_la[i].join() + "\"" );
}

