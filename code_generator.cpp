#include <String>

#include "llvm/Module.h"
#include "llvm/Function.h"
#include "llvm/PassManager.h"
#include "llvm/CallingConv.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

#include "code_generator.hpp"

#include "code_ast.cpp"

using namespace llvm;

Module* makeLLVMModule( Node ast ){
	Module* mod = new Module( "alice", getGlobalContext() );

	ast.codeGen( *mod );

	return mod;
}

int main(){
	CodeGenerator = new CodeGenerator();
	std::String rawDataFromParser;
	SimpleNode dataFromParser;

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

}