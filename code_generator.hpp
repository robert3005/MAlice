#ifndef GENERATOR_H_GUARD
#define GENERATOR_H_GUARD

#include <string>
#include <map>

#include "code_ast.hpp"

llvm::Function * makeLLVMModule( Node & );

class CodeGenerator{
	public:
			std::map<int, SimpleNode* > parse( std::string );
			
};

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

Function * makeLLVMModule( Node & ast ){
	theModule = new Module( "alice", getGlobalContext() );

	FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
	Main = Function::Create(FT, Function::ExternalLinkage, "main", theModule);
	BB = BasicBlock::Create( getGlobalContext(), "EntryBlock", Main );
	Builder.SetInsertPoint( BB );

	Environment<Node> * env = new Environment<Node>();
	//env -> add("main", Main);

	for(std::vector< Node * >::iterator it = ast.children.begin(); it != ast.children.end(); ++it) {
	    ( *it ) -> debug();
		( *it ) -> codeGen( Builder, *env );
   	}  	  	
	
	if( Builder.GetInsertBlock() -> getTerminator() == 0 ){
		Builder.CreateRet( ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), 0 ));
	}
    //________________________________________________________________

  	//theModule -> dump();
  	return Main;
}


#endif