#include <string>
#include <map>
#include <utility>
#include <stack>

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"


#include "code_generator.hpp"
#include "code_ast.hpp"

using namespace llvm;
using namespace std;
/**
 * Converts string representation on a node to SimpleNode object
 */
map<int, SimpleNode* > CodeGenerator::parse( string rawData ){
	map<int, SimpleNode* > mapOfNodes;

	size_t pos;
	int last = 0;

	string node;
	string nodeIdNumber;
	string nodeType;
	string opType;
	string args;

	size_t fieldPos;
	int lastFieldPos = 0;

	while( string::npos != ( pos = rawData.find( "#" ) ) ){
		node = rawData.substr( last + 1, ( int( pos ) - last - 1 ) );

		lastFieldPos = 0;
		fieldPos = node.find( "|" );
		nodeIdNumber = node.substr( 0, fieldPos );

		lastFieldPos = int( fieldPos );
		fieldPos = node.find( "|" );
		nodeType = node.substr( lastFieldPos + 1, ( int( fieldPos ) - lastFieldPos - 1 ) );

		lastFieldPos = int( fieldPos );
		fieldPos = node.find( "|" );
		opType = node.substr( lastFieldPos + 1, ( int( fieldPos ) - lastFieldPos - 1 ) );

		lastFieldPos = int( fieldPos );
		fieldPos = node.find( "|" );
		args = node.substr( lastFieldPos + 1, ( int( fieldPos ) - lastFieldPos - 1 ) );

		mapOfNodes[atoi( nodeIdNumber.c_str() )] = new SimpleNode( nodeIdNumber, nodeType, opType, args );

		last = int(pos);
	}
}
/*
Module* makeLLVMModule( Node ast ){
	Module* mod = new Module( "alice", getGlobalContext() );

	ast.codeGen( *mod );

	return mod;
}

int main(){
	CodeGenerator = new CodeGenerator();
	std::string rawDataFromParser = "0#OP#ADD#1,2#1,2|1#CONST#NONE##1|2#CONST#NONE##2";
	std::map<int, SimpleNode> dataFromParser;

	//read data input

	dataFromParser = CodeGenerator.parse( rawDataFromParser );
	Node ast = Node::generateAST( dataFromParser );

	Module* Mod = makeLLVMModule( ast );

	verifyModule( *Mod, PrintMessageAction );

	PassManager PM;
	PM.add( createPrintModulePass( &outs() ) );
	PM.run( *Mod );

	delete Mod;
	return 0;

}*/

int main(){
	CodeGenerator * codeGen = new CodeGenerator();
	std::string rawDataFromParser = "0#OP#ADD#1,2#1,2|1#CONST#NONE##1|2#CONST#NONE##2";
	std::map<int, SimpleNode*> dataFromParser;

	//read data input

	dataFromParser = codeGen -> parse( rawDataFromParser );
	//Node ast = Node::generateAST( dataFromParser );

	delete codeGen;
	return 0;
}