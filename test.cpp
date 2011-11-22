#include "UnitTest++/src/UnitTest++.h"

#include <cstdio>

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

#include "code_ast.hpp"
#include "code_generator.hpp"

using namespace std;
using namespace llvm;

static ExecutionEngine *TheExecutionEngine;

inline pair<int, string> execute( string input ){
	char outputBuf[20002];
	fflush(stdout);
	setvbuf(stdout, outputBuf, _IOFBF, 20000);
	CodeGenerator * codeGen = new CodeGenerator();
	map<int, SimpleNode*> dataFromParser;
	dataFromParser = codeGen -> parse( input );
	Node * ast = Node::createAST( dataFromParser );
	makeLLVMModule( *ast );
	TheExecutionEngine = EngineBuilder::createJIT( theModule );

	Function* func = TheExecutionEngine -> FindFunctionNamed("main");

    typedef int ( *PFN )();
    PFN pfn = reinterpret_cast<PFN>( TheExecutionEngine -> getPointerToFunction( func ) );
    int result  = pfn();

    return make_pair( result, string(outputBuf) );
}


TEST( constant_string ){
	pair<int, string> ret = execute( "1#CONST#NONE#STRING,piesek leszek jest kurwa zajebisty,|2#RET#NONE#,1," );
	string expected_output = "piesek leszek jest kurwa zajebisty";
	CHECK_EQUAL(expected_output, ret.second);
}

TEST( constant_letter ){
	CHECK(false);
}

TEST( constant_number ){
	pair<int, string> ret = execute( "1#CONST#NONE#NUMBER,3,|2#RET#NONE#,1," );
	int expected_result = 3;
	CHECK( expected_result == ret.first );	
}

TEST( constant_array ){
	CHECK(false);	
}

TEST( variable ){
	CHECK(false);	
}

TEST( if_statement ){
	CHECK(false);	
}

TEST( while_statement ){
	CHECK(false);	
}

TEST( return_statement ){
	CHECK(false);	
}

TEST( printf_statement ){
	CHECK(false);	
}

TEST( add_op ){
	CHECK(false);	
}

TEST( sub_op ){
	CHECK(false);	
}

TEST( mul_op ){
	CHECK(false);	
}

TEST( div_op ){
	CHECK(false);	
}

TEST( mod_op ){
	CHECK(false);	
}

TEST( neg_op ){
	CHECK(false);	
}

TEST( and_op ){
		CHECK(false);
}

TEST( or_op ){
	CHECK(false);	
}

TEST( xor_op ){
	CHECK(false);	
}

TEST( bool_g_op ){
	CHECK(false);	
}

TEST( bool_e_op ){
	CHECK(false);	
}

TEST( bool_ge_op ){
	CHECK(false);	
}

TEST( bool_s_op ){
	CHECK(false);	
}

TEST( bool_se_op ){
	CHECK(false);
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
    return UnitTest::RunAllTests();
}