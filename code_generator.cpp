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

using namespace std;
using namespace llvm;

Module * makeLLVMModule( Node & ast ){
	llvm::Module * theModule = new Module( "alice", getGlobalContext() );

	FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
	Main = Function::Create(FT, Function::ExternalLinkage, "main", theModule);
	BB = BasicBlock::Create( getGlobalContext(), "EntryBlock", Main );
	Builder.SetInsertPoint( BB );

	Environment<Node> * env = new Environment<Node>();
	env -> addScope("main");

	for(std::vector< Node * >::iterator it = ast.children.begin(); it != ast.children.end(); ++it) {
		if( ( *it ) -> getType() == FUNC_DEF )
		( *it ) -> codeGen( Builder, *env, theModule );
   	}  	  	
	
	for(std::vector< Node * >::iterator it = ast.children.begin(); it != ast.children.end(); ++it) {
		if( ( *it ) -> getType() != FUNC_DEF )
		( *it ) -> codeGen( Builder, *env, theModule );
   	}

	if( Builder.GetInsertBlock() -> getTerminator() == 0 ){
		Builder.CreateRet( ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), 0 ));
	}
    //________________________________________________________________

  	//theModule -> dump();
  	return theModule;
}

/**
 * Converts string representation on a node to SimpleNode object
 */
map<int, SimpleNode* > parse( string rawData ){
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
