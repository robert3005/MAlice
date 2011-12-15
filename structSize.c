#include <iostream>
#include <iomanip>
#include <fstream>

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

#include "structSize.h"
#include "code_ast.hpp"
#include "code_generator.hpp"

using namespace std;
using namespace llvm;

#ifdef  __cplusplus
extern "C" {
#endif

void compile(SNode node, char * s){
	cout << "Create AST\n";
	Node * ast = Node::createAST( *node );
	cout << "Make module\n";
	Module * theModule = makeLLVMModule( *ast );
	verifyModule( *theModule, PrintMessageAction );

	//ExecutionEngine *ee = EngineBuilder( theModule ).create();
    //Function* func = ee -> FindFunctionNamed("main");

    ofstream myfile;
  	myfile.open ( "hello.c.ll" );
  	myfile << theModule;
  	myfile.close();

  	cout << theModule;
  	//theModule -> dump(); //print theModule to the output

  	PassManager PM;
	PM.add( createPrintModulePass( &outs() ) );
	PM.run( *theModule );
}

void print_struct(SNode node, int indent) {
	cout << setw(indent) << " " << "id: " << node->id << endl;
	cout << setw(indent) << " " << "type: " << node->type << endl;
	cout << setw(indent) << " " << "value: " << node->value << endl;
	cout << setw(indent) << " " << "position: " << node->position[0] << ", " << node->position[1] << endl;
	cout << setw(indent) << " " << "childrenNumber: " << node->numberOfChildren << endl;
	if(node->numberOfChildren != 0 ) {
		cout << setw(indent) << " " << "children: " << endl << endl;
		for(int i = 0; i < node->numberOfChildren; ++i) {
			print_struct(node->children[i], indent+2);
		}
	} else {
		cout << endl;
	} 
}

#ifdef  __cplusplus
}
#endif