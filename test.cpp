#include "UnitTest++/src/UnitTest++.h"

#include <cstdio>
#include <iostream>
#include <vector>

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
/*
g++ -g test.cpp code_ast.cpp  UnitTest++/libUnitTest++.a `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native jit` -o test

*/

#include "llvm/Instructions.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/PassManager.h"
#include "llvm/CallingConv.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/System/DynamicLibrary.h"


#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

#include "llvm/Support/MemoryBuffer.h"

#include "code_ast.hpp"
#include "code_generator.hpp"

using namespace std;
using namespace llvm;

static ExecutionEngine *TheExecutionEngine;

pair<int, string> execute( string input ){
	out = "";
	
	CodeGenerator * codeGen = new CodeGenerator();
	map<int, SimpleNode*> dataFromParser;
	dataFromParser = codeGen -> parse( input );
	map< int, Node * > ast = Node::createAST( dataFromParser );
	makeLLVMModule( ast );
	verifyModule( *theModule, PrintMessageAction );

	ExecutionEngine *ee = EngineBuilder( theModule ).create();
    Function* func = ee -> FindFunctionNamed("main");

    typedef int (*PFN)();
    PFN pfn = reinterpret_cast<PFN>(ee->getPointerToFunction(func));
    int result = pfn();

    //printf( "%s\n", out.c_str() );
    delete theModule;

    return make_pair( result, out );
}


TEST( constant_string ){
	//pair<int, string> ret = execute( "1#CONST#NONE#STRING,piesek leszek jest kurwa zajebisty,|2#RET#NONE#,1," );
	string expected_output = "piesek leszek jest kurwa zajebisty";
	//CHECK_EQUAL( expected_output, ret.second );
}

TEST( constant_letter ){
	pair<int, string> ret = execute( "3#CONST#NONE#LETTER,a,|4#RET#NONE#,3,|" );
	string expected_result = "a";
	CHECK_EQUAL( expected_result, ret.second );	
}

TEST( constant_number ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,3,|4#RET#NONE#,3,|" );
	int expected_result = 3;
	CHECK( expected_result == ret.first );	
}

TEST( constant_array ){
	CHECK(false);	
}

TEST( variable ){
	pair<int, string> ret = execute( "0#TYPE#NONE#NUMBER,y,|4#VAR#NONE#,y,5,|5#CONST#NONE#NUMBER,5,|6#RET#NONE#y,|" );
	int expected_result = 5;
	CHECK_EQUAL( expected_result, ret.first );			
}

TEST( if_statement ){
	pair<int, string> ret = execute( "0#CONST#NONE#NUMBER,5,|1#CONST#NONE#NUMBER,1,|2#CONST#NONE#NUMBER,0,|6#IF#NONE#2,0,1,|9#RET#NONE#,6,|" );
	int expected_result = 1;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( while_statement ){
	pair<int, string> ret = execute( "0#TYPE#NONE#NUMBER,x,|1#VAR#NONE#,x,2,|2#CONST#NONE#NUMBER,5,|3#WHILE#NONE#4,6,|4#OP#S#x,5,|5#CONST#NONE#NUMBER,8,|6#VAR#NONE#,x,7,|7#OP#SUB#x,8,|8#CONST#NONE#NUMBER,1,|9#RET#NONE#x,|" );
	int expected_result = 5;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( printf_statement ){
	CHECK(false);	
}

TEST( add_op ){
	pair<int, string> ret = execute( "0#TYPE#NONE#NUMBER,y,|1#TYPE#NONE#NUMBER,x,|2#VAR#NONE#,x,3,|3#CONST#NONE#NUMBER,5,|4#VAR#NONE#,y,5,|5#OP#ADD#3,1,|6#RET#NONE#y,|" );
	int expected_result = 10;
	CHECK_EQUAL( expected_result, ret.first );			
}

TEST( sub_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,5,|5#CONST#NONE#NUMBER,1,|6#OP#SUB#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 4;
	CHECK_EQUAL( expected_result, ret.first );		
}

TEST( mul_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#MUL#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 14;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( div_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#DIV#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 3;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( mod_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,5,|6#OP#MOD#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 2;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( neg_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#NOT#3,|7#RET#NONE#,6,|" );
	int expected_result = -8;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( and_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#AND#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 2;
	CHECK_EQUAL( expected_result, ret.first );		
}

TEST( or_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#OR#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 7;
	CHECK_EQUAL( expected_result, ret.first );			
}

TEST( xor_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#XOR#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 5;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( bool_g_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#G#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 1;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( bool_e_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#E#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 0;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( bool_e_true_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,7,|6#OP#E#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 1;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( bool_ge_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#GOE#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 1;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( bool_s_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#S#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 0;
	CHECK_EQUAL( expected_result, ret.first );	
}

TEST( bool_se_op ){
	pair<int, string> ret = execute( "3#CONST#NONE#NUMBER,7,|5#CONST#NONE#NUMBER,2,|6#OP#SOE#3,5,|7#RET#NONE#,6,|" );
	int expected_result = 0;
	CHECK_EQUAL( expected_result, ret.first );		
}

TEST( function1 ){
	CHECK(false);	
}

TEST( function2 ){
	CHECK(false);	
}

TEST( function3 ){
	CHECK(false);	
}

int main(int, char const *[])
{
	InitializeNativeTarget();
    return UnitTest::RunAllTests();
}