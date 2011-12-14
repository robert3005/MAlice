#ifndef STRUCT_SIZE_H
#define STRUCT_SIZE_H

#ifdef  __cplusplus
extern "C" {
#endif

using namespace std;

typedef struct node_struct * SNode;

struct node_struct {
	int id;
	char * type;
	char * value;
	int * position;
	int numberOfChildren;
	SNode * children;
};

void print_struct(SNode node, int indent);

#ifdef  __cplusplus
}
#endif

#endif