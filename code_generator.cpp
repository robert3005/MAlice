#include <string>
#include <map>
#include <utility>
#include <stack>
#include <cstdio>
#include <iostream>
#include <vector>

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/PassManager.h"
#include "llvm/CallingConv.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/System/DynamicLibrary.h"


#include "code_generator.hpp"
#include "code_ast.hpp"

int main(){
	CodeGenerator * codeGen = new CodeGenerator();
	string rawDataFromParser;
	//cin >> rawDataFromParser;
	//string rawDataFromParser = "0#OP#ADD#1,2,|1#CONST#NONE#NUMBER,1,|2#CONST#NONE#NUBMER,2,|";
	rawDataFromParser = "0#TYPE#NONE#NUMBER,x,|1#VAR#NONE#,x,2,|2#CONST#NONE#NUMBER,5,|3#WHILE#NONE#4,6,|4#OP#S#1,5,|5#CONST#NONE#NUMBER,8,|6#VAR#NONE#,x,7,|7#OP#SUB#x,8,|8#CONST#NONE#NUMBER,1,|9#RET#NONE#x,|";
	map<int, SimpleNode*> dataFromParser;

	//read data input

	if( DEBUG ) printf("Generating nodes from: %s ...\n", rawDataFromParser.c_str());

	dataFromParser = codeGen -> parse( rawDataFromParser );

	if( DEBUG ){
		map<int, SimpleNode*>::iterator it;
		for( it = dataFromParser.begin(); it != dataFromParser.end(); it++ ){
			(*it).second -> debug();
		}
	}

	map< int, Node * > ast = Node::createAST( dataFromParser );


	if(DEBUG) printf("Generate code...\n");

	makeLLVMModule( ast );

	verifyModule( *theModule, PrintMessageAction );

	PassManager PM;
	PM.add( createPrintModulePass( &outs() ) );
	PM.run( *theModule );

	delete theModule;
	delete codeGen;
	//delete ast;
	return 0;
}
