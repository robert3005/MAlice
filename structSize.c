#include <iostream>
#include <iomanip>
#include <fstream>


#include "structSize.h"
#include "code_ast.hpp"
#include "code_generator.hpp"

using namespace std;

#ifdef  __cplusplus
extern "C" {
#endif

void compile(SNode node, string s){
	CodeGenerator * codeGen = new CodeGenerator();
	
	Node * ast = Node::createAST( *node );
	makeLLVMModule( *ast );
	verifyModule( *theModule, PrintMessageAction );

	//ExecutionEngine *ee = EngineBuilder( theModule ).create();
    //Function* func = ee -> FindFunctionNamed("main");

    ofstream myfile;
  	myfile.open ( s.c_str() );
  	myfile << theModule;
  	myfile.close();

  	theModule -> dump(); //print theModule to the output
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