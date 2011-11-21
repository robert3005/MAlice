#include <string>
#include <map>
#include <utility>
#include <stack>
#include <cstdio>
#include <iostream>


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

#define DEBUG false

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

	while( rawData.length() > 1 && string::npos != ( pos = rawData.find( "|" ) )){
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

void makeLLVMModule( Node & ast ){
	theModule = new Module( "alice", getGlobalContext() );

	FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
	Main = Function::Create(FT, Function::ExternalLinkage, "main", theModule);
	BB = BasicBlock::Create( getGlobalContext(), "EntryBlock", Main );
	Builder.SetInsertPoint( BB );

  	Value * root = ast.codeGen( Builder );

  	
	vector<const Type *> args;
	args.push_back(Type::getInt32Ty( getGlobalContext() ));
  	FunctionType *ft = FunctionType::get(Type::getInt32Ty(getGlobalContext()), args, false); 
	Function *g = Function::Create(ft, Function::ExternalLinkage, Twine("exit"), theModule);
	
  	CallInst * ce = Builder.CreateCall( g, root );

  	Builder.CreateRet( ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), 0 ) );
}

int main(){
	CodeGenerator * codeGen = new CodeGenerator();
	string rawDataFromParser;
	cin >> rawDataFromParser;
	//string rawDataFromParser = "0#OP#ADD#1,2,|1#CONST#NONE#NUMBER,1,|2#CONST#NONE#NUBMER,2,|";
	//rawDataFromParser = "0#TYPE#NONE#NUMBER,y,|1#TYPE#NONE#NUMBER,x,|2#VAR#NONE#,x,3,|3#CONST#NONE#NUMBER,3,|4#VAR#NONE#,y,5,|5#OP#ADD#3,1,|6#RET#NONE#y,|";
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

	if(DEBUG) printf("Generate code...\n");

	makeLLVMModule( *ast );

	verifyModule( *theModule, PrintMessageAction );

	PassManager PM;
	PM.add( createPrintModulePass( &outs() ) );
	PM.run( *theModule );

	delete theModule;
	delete codeGen;
	delete ast;
	return 0;
}
