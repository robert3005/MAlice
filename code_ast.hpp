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
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"

enum NodeType {OP = 0, VAR, CONST, TYPE};
enum OPType {NONE = 0, ADD, OR, XOR, AND, SUB, MUL, DIV, MOD, UNR, NEG};
enum VarType {STRING = 0, NUMBER};

static llvm::Module *TheModule;
static llvm::IRBuilder<> Builder( llvm::getGlobalContext() );
/*
typedef Value* Valueptr
typedef Value * (*OPGenFunction)( *Module, *Builder );

class SymbolTable{
	
};
*/
class SimpleNode{
	public:
		SimpleNode();
		SimpleNode( std::string, std::string, std::string, std::string );

		NodeType getType();
		int getId();
		std::string getData();

		void debug();
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
		Node();
		virtual llvm::Value *codeGen() = 0;
		void addChild( Node* );

		static Node * createAST( std::map<int, SimpleNode*> & ); //Factory
	
	protected:
		Node( SimpleNode );

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
		std::vector<Node> children;

};

class OPNode : public Node{
	public:
		OPNode();
		OPNode( const SimpleNode& s){};
		llvm::Value *codeGen();

	protected:

		llvm::Value *lhs;
		llvm::Value *rhs;

		static llvm::Value *codeGenADD( const OPNode& );
		static llvm::Value *codeGenOR( const OPNode& );
		static llvm::Value *codeGenXOR( const OPNode& );
		static llvm::Value *codeGenAND( const OPNode& );
		static llvm::Value *codeGenSUB( const OPNode& ); 
		static llvm::Value *codeGenMUL( const OPNode& );
		static llvm::Value *codeGenDIV( const OPNode& );
		static llvm::Value *codeGenUNR( const OPNode& );
		static llvm::Value *codeGenNEG( const OPNode& );
};

class VARNode : public Node{
		public:
		VARNode();
		VARNode( const SimpleNode& s){};
		llvm::Value *codeGen();
};

class CONSTNode : public Node{
	public:
		CONSTNode();
		CONSTNode( const SimpleNode& s){};
		llvm::Value *codeGen();	

	protected:

		static llvm::Value *codeGenSTRING( const OPNode& ){};
		static llvm::Value *codeGenNUMBER( const OPNode& ){};
};

class TYPENode : public Node{
	public:
		TYPENode();
		TYPENode( const SimpleNode& s){};
		llvm::Value *codeGen();	
};

#endif