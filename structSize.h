#ifndef STRUCT_SIZE_H
#define STRUCT_SIZE_H

#ifdef  __cplusplus
extern "C" {
#endif

using namespace std;

typedef struct Node * NodePtr;

struct Node {
	int id;
	char * type;
	char * value;
	int numberOfChildren;
	NodePtr * children;
};

void print_struct(NodePtr node, int indent);

#ifdef  __cplusplus
}
#endif

#endif