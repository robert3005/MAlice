#include <string>
#include <map>
#include <utility>
#include <stack>
#include <cstdio>

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"


#include "code_generator.hpp"
#include "code_ast.hpp"

#define DEBUG true

using namespace llvm;
using namespace std;
/**
 * Converts string representation on a node to SimpleNode object
 */
map<int, SimpleNode* > CodeGenerator::parse( string rawData ){
	map<int, SimpleNode* > mapOfNodes;

	size_t pos;

	string node;
	string nodeIdNumber;
	string nodeType;
	string opType;
	string args;

	size_t fieldPos;

	int i = 0;

	while( string::npos != ( pos = rawData.find( "|" ) ) && i < 3 ){
		node = rawData.substr( 0, pos );
	
		fieldPos = node.find( "#" );
		nodeIdNumber = node.substr( 0, fieldPos );
		node = node.substr( fieldPos + 1 );
		
		fieldPos = node.find( "#" );
		nodeType = node.substr( 0, fieldPos );
		node = node.substr( fieldPos + 1 );

		fieldPos = node.find( "#" );
		opType = node.substr( 0, fieldPos );
		node = node.substr( fieldPos + 1 );

		args = node;
		
		mapOfNodes[atoi( nodeIdNumber.c_str() )] = new SimpleNode( nodeIdNumber, nodeType, opType, args );
		
		rawData = rawData.substr( pos + 1 );
		i++;
	}

	return mapOfNodes;
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
	string rawDataFromParser = "0#OP#ADD#1,2,|1#CONST#NONE#NUMBER,1,|2#CONST#NONE#NUBMER,2,|";
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

	Node * ast = Node::createAST( dataFromParser );

	if( DEBUG ){
		map<int, SimpleNode*>::iterator it;
		for( it = dataFromParser.begin(); it != dataFromParser.end(); it++ ){
			(Node)(*it).second -> debug();
		}
	}

	delete codeGen;
	return 0;
}