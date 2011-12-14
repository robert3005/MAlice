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
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"

#include "structSize.h"

enum NodeType {OP = 0, VAR, CONST, TYPE, RET, WHILE, IF, FUNC, ARRAY, ARRAY_ELEM, IO, FUNC_DEF, FUNC_CALL, LOOK_DEF};
enum OPType {NONE = 0, ADD, OR, XOR, AND, SUB, MUL, DIV, MOD, UNR, NEG, G, GOE, S, SOE, E, BOOL_OR, BOOL_AND, NOT, NE};
enum VarType {STRING = 0, NUMBER, LETTER, T_ARRAY, T_NONE};

static llvm::Module * theModule;
static llvm::IRBuilder<> Builder( llvm::getGlobalContext() );
static llvm::Function * Main;
static llvm::BasicBlock * BB;

static std::string out; //output stream :P


/*
typedef Value* Valueptr
typedef Value * (*OPGenFunction)( *Module, *Builder );

class SymbolTable{
	
};
*/
template <class T>
class Environment{
	public:
		Environment(){
			parent = 0;
		}

		Environment( Environment<T> * p ){
			parent = p;
		}

		Environment<T> * getScope( std::string key ){
			typename std::map<std::string, Environment<T> * >::iterator it;
			it = scopes.find( key );

			if( it != scopes.end() ){
				return (*it).second;
			} else {
				return 0;
			}	
		}

		void addScope( std::string key ){
			scopes[key] = new Environment<T>( this );
		}

		void add( std::string key, T * n ){
			typename std::map<std::string, T* >::iterator it;
			it = elements.find( key );

			if( it != elements.end() ){
				elements[key] = n;
			} else {
				elements[key] = n;
			}
		}
		 
		T * get( std::string key ){
			typename std::map<std::string, T* >::iterator it;
			it = elements.find( key );

			if( it != elements.end() ){
				return (*it).second;
			} else if( parent != 0 ) {
				return parent -> get( key );
			} else {
				return 0;
			}
		}

		bool is( std::string key ){
			return (elements.find( key ) != elements.end());
		}

	protected:
		std::map< std::string, Environment<T> * > scopes;
		std::map< std::string, T * > elements;
		Environment<T> * parent;
};

class SimpleNode : public node_struct{
	public:
		SimpleNode();
		SimpleNode( node_struct & );
		SimpleNode( std::string, std::string, std::string, std::string );

		NodeType getType();
		OPType getOP();
		int getId();
		std::string getData();

		void debug();

		llvm::IRBuilder<> * builder;
		std::vector<SimpleNode* > children;

		std::string value;
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
		Node(){};
		virtual ~Node(){};
		Node( SimpleNode& );
		virtual llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&) = 0;
		void addChild( Node* );

		static Node * createAST( node_struct & ); //Factory
		static Node * createNode( SimpleNode & ); 
		
		VarType getVarType();
		void setVarType( VarType );
		
		std::string getVarId();
		void setVarId( std::string );

		std::string getValueString();
		void setValueString( std::string );
		
		char getValueLetter();
		void setValueLetter( char );

		int getValueNumber();
		void setValueNumber( int );
			
		llvm::Value * getValue();
		void setValue( llvm::Value * );
				
		void debug();
		
		llvm::AllocaInst * alloca;

		//children
		std::vector<Node*> children;

		Node * varNodeWithValue;
		bool allocated;

		bool wasGenerated();

		//printf
		//virtual void say(); 

	protected:
		static Node * createOPNode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createVARNode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createCONSTNode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createTYPENode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createRETNode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createIFNode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createWHILENode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createARRAYNode( SimpleNode&, std::list<std::pair<int, int> >& );
		static Node * createARRAYELEMNode( SimpleNode&, std::list<std::pair<int, int> >& );

		//TODO: decide which fields to leave here, and which to move down
		std::string id;
		VarType varType;
		
		//values
		llvm::Value * value;
		
		int valueNumber;
		std::string valueString;
		char valueLetter;

		bool generated;

		int set;

		Node * tn;

		int arrayType, arraySize; 
};

static std::map<std::string, VarType> mapOfTypes;

class OPNode : public Node{
	public:
		OPNode(){};
		OPNode( SimpleNode& );
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);

	protected:

		llvm::Value *lhs;
		llvm::Value *rhs;

		static llvm::Value *codeGenADD( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenOR( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenXOR( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenAND( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenSUB( llvm::IRBuilder<> &, OPNode& ); 
		static llvm::Value *codeGenMUL( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenDIV( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenMOD( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenNEG( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenS( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenG( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenE( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenGOE( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenSOE( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenNOT( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenNE( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenBOR( llvm::IRBuilder<> &, OPNode& );
		static llvm::Value *codeGenBAND( llvm::IRBuilder<> &, OPNode& );

};

class VARNode : public Node{
	public:
		VARNode();
		VARNode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);

		void setAlloca( llvm::AllocaInst * );

	protected:
		llvm::Value *lhs;
};		
class RETNode : public Node{
	public:
		RETNode();
		RETNode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);

		llvm::Value *lhs;
};

class CONSTNode : public Node{
	public:
		CONSTNode();
		CONSTNode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	

		llvm::Value *lhs;

	protected:

		static llvm::Value *codeGenSTRING( CONSTNode& );
		static llvm::Value *codeGenNUMBER( CONSTNode& );
		static llvm::Value *codeGenLETTER( CONSTNode& );
};

class ARRAYELEMNode : public Node{
	public:
		ARRAYELEMNode();
		ARRAYELEMNode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	

	protected:

		VarType arrayType;
};

class TYPENode : public Node{
	public:
		TYPENode();
		TYPENode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	

		llvm::Type * getLlvmType();
		llvm::Type * getLlvmArgType();
		VarType getArgType();

	protected:
		static llvm::Value *codeGenSTRING( TYPENode & , llvm::IRBuilder<> &  );
		static llvm::Value *codeGenNUMBER( TYPENode & , llvm::IRBuilder<> &  );
		static llvm::Value *codeGenLETTER( TYPENode & , llvm::IRBuilder<> &  );
};

class IFNode : public Node{
	public:
		IFNode();
		IFNode( SimpleNode& s);
		void setCondId( std::string );
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	
		Node * getCondNode();

	protected:
		Node * cond;
		std::string cond_id;

		Node * else_node;
		std::list< Node * > body;	
};

class WHILENode : public IFNode {
	public:
		WHILENode();
		WHILENode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	
	
};

class FUNCTIONNode : public Node {
	public:
		FUNCTIONNode();
		FUNCTIONNode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	
		
		std::string getFunName();
		std::vector<llvm::Type*> getArgsTypes();
		std::vector<std::string> getArgs();

		std::string funName;

	protected:
		std::vector<llvm::Type*> args;
		std::vector<std::string> argsNames;
};

class FUNCTIONDEFNode : public Node {
	public:
		FUNCTIONDEFNode();
		FUNCTIONDEFNode( SimpleNode& s);
		llvm::Function *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	
		
		llvm::FunctionType * FT;
		llvm::Function * F;

		FUNCTIONNode * Fn;

		Environment<Node> * Fenv;

	protected:

};



class FUNCTIONCALLNode : public Node {
	public:
		FUNCTIONCALLNode();
		FUNCTIONCALLNode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	
		
		llvm::Value *callWas(llvm::IRBuilder<> &, Environment<Node>&);
		llvm::Value *callBecame(llvm::IRBuilder<> &, Environment<Node>&);
		llvm::Value *callDrank(llvm::IRBuilder<> &, Environment<Node>&);
		llvm::Value *callAte(llvm::IRBuilder<> &, Environment<Node>&);
		llvm::Value *callHad(llvm::IRBuilder<> &, Environment<Node>&);

		std::string funName;

		std::string getFunName();
		std::vector<llvm::Type*> getArgs();

	protected:
		std::vector<llvm::Type*> args;
};

class IONode : public Node{
	public:
		IONode();
		IONode( SimpleNode& s);
		llvm::Value *codeGen(llvm::IRBuilder<> &, Environment<Node>&);	
		
		llvm::Value *callWhatWas(llvm::IRBuilder<> &, Environment<Node>&);
		llvm::Value *callSaidAlice(llvm::IRBuilder<> &, Environment<Node>&);
		llvm::Value *callSpoke(llvm::IRBuilder<> &, Environment<Node>&);

	protected:
};

#endif