#include <String>

enum NodeType {OP = 0, VAR, CONST, TYPE}
enum OPType {NONE = 0, ADD, OR, XOR, AND, SUB, MUL, DIV, MOD, UNR, NEG}
enum VarType {STRING = 0, NUMBER}

typedef Value* (*OPGenFunction)( Module*, Builder* );
typedef Node* (*NodeGenFunction)( NodeType, OPType, const char* Format, ... );

class SymbolTable{
	
};

class SimpleNode{
	protected:
		//general
		NodeType type;
		OPType op;
		std::String id;
		VarType varType;
		
		//values
		int valueInt;
		std::String valueString;

		//children
		SimpleNode children;

	public:
		SimpleNode();
};

class Node : public SimpleNode{
	public:
		virtual Value *codeGen( Module*, Builder* ) = 0;
		static Node createAST( SimpleNode ); //Factory
	
	protected:
		NodeGenFunction codeGenFunctions[11] = { NULL };
};

class OPNode : public Node{
	public:
		Value *codeGen( Module*, Builder* );

	protected:

		Value *lhs;
		Value *rhs;

		Value *codeGenAdd();
		Value *codeGenOR();
		Value *codeGenXOR();
		Value *codeGenAND();
		Value *codeGenSUB(); 
		Value *codeGenMUL();
		Value *codeGenDIV();
		Value *codeGenUNR();
		Value *codeGenNEG();
};

class VARNode : public Node{
		public:
		Value *codeGen( Module*, Builder* );
};

class CONSTNode : public Node{
	public:
		Value *codeGen( Module*, Builder* );	

	protected:

		Value *codeGenSTRING( Module*, Builder* );
		Value *codeGenNUMBER( Module*, Builder* );
}

class TYPENode : public Node{
	public:
		Value *codeGen( Module*, Builder* );	
}