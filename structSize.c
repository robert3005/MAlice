#include <iostream>

using namespace std;

struct node {
	int id;
	int type;
	int value;
	node * children[];
};

typedef struct node * nodePtr;

int main(int argc, char* argv[]) {
	cout << sizeof(node) << endl;
}
