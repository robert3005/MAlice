#include <iostream>
#include <iomanip>
#include "structSize.h"

using namespace std;

#ifdef  __cplusplus
extern "C" {
#endif

void print_struct(Node node, int indent) {
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