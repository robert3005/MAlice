#ifndef AST_H_GUARD
#define AST_H_GUARD

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <list>

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"

enum NodeType {OP = 0, VAR, CONST, TYPE};
enum OPType {NONE = 0, ADD, OR, XOR, AND, SUB, MUL, DIV, MOD, UNR, NEG};
enum VarType {STRING = 0, NUMBER};
/*
typedef Value* Valueptr
typedef Value * (*OPGenFunction)( *Module, *Builder );

class SymbolTable{
	
};
*/
class SimpleNode{
	public:
		SimpleNode( std::string, std::string, std::string, std::string );

		NodeType getType();
		int getId();
		std::string getData();

	protected:
		int uniqueId; 

		//general
		NodeType type;
		OPType op;
		
		//data left to parse
		std::string data;


};

class Node : public SimpleNode{
	public:
		virtual Value *codeGen( Module&, Builder& ) = 0;
		void addChild( Node* );

		static Node createAST( std::map<int, SimpleNode>& ); //Factory
	
	protected:
		Node( SimpleNode );
		Value * (*OPGenFunction)( *Module, *Builder ) codeGenFunctions[11] = { NULL };

		static Node * createOPNode( const SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createVARNode( const SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createCONSTNode( const SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createTYPENode( const SimpleNode&, std::list<std::pair<int, int> >& );

		//TODO: decide which fields to leave here, and which to move down
		std::string id;
		VarType varType;
		
		//values
		int valueInt;
		std::string valueString;

		//children
		std::vector<SimpleNode> children;

};

class OPNode : public Node{
	public:
		Value *codeGen( Module&, Builder& );

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
		Value *codeGen( Module&, Builder& );
};

class CONSTNode : public Node{
	public:
		Value *codeGen( Module&, Builder& );	

	protected:

		Value *codeGenSTRING( Module&, Builder& );
		Value *codeGenNUMBER( Module&, Builder& );
};

class TYPENode : public Node{
	public:
		Value *codeGen( Module&, Builder& );	
};

#endif