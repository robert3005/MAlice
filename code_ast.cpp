#include <cstdio>
#include <iostream>

#include "code_ast.hpp"

using namespace llvm;
using namespace std;

#define DEBUG false
//SimpleNode
SimpleNode::SimpleNode(){}

SimpleNode::SimpleNode( node_struct& s ){
	this -> uniqueId = s.id;
	value = string(s.value);
	position = s.position;
	numberOfChildren = s.numberOfChildren;
	
	if( string(s.type).compare( "OPERATOR" ) == 0 ){
 		this -> type = OP;
	} else if( string(s.type).compare( "VARIABLE" ) == 0 ){
 		this -> type = VAR;
	} else if( string(s.type).compare( "CONSTANT" ) == 0 ){
 		this -> type = CONST;
	} else if( string(s.type).compare( "TYPE" ) == 0 ){
 		this -> type = TYPE;
	} else if( string(s.type).compare( "RETURN" ) == 0 ){
 		this -> type = RET;
	} else if( string(s.type).compare( "IF" ) == 0 ){
		this -> type = IF;
	} else if( string(s.type).compare( "ELSE_IF" ) == 0 ){
 		this -> type = IF;
	} else if( string(s.type).compare( "ELSE" ) == 0 ){
		this -> type = ELSE;
	} else if( string(s.type).compare( "END_IF" ) == 0 ){
		this -> type = END_IF;
	} else if( string(s.type).compare( "WHILE" ) == 0 ){
 		this -> type = WHILE;
	} else if( string(s.type).compare( "ARRAY" ) == 0 ){
		this -> type = ARRAY;
	} else if( string(s.type).compare( "ARRAY_ELEM" ) == 0 ){
		this -> type = ARRAY_ELEM;
	} else if( string(s.type).compare( "FUNCTION" ) == 0 ){
		this -> type = FUNC;
	} else if( string(s.type).compare( "FUNCTION_DEFINITION" ) == 0 ){
		this -> type = FUNC_DEF;
	} else if( string(s.type).compare( "FUNCTION_CALL" ) == 0 ){
		this -> type = FUNC_CALL;
	} else if( string(s.type).compare( "LOOKING_GLASS" ) == 0 ){
		this -> type = LOOK_DEF;
	}
 
	SimpleNode * child;
	for(int i = 0; i < numberOfChildren; i++){
		child = new SimpleNode( *s.children[i]);
		children.push_back(child);
	}
}

SimpleNode::SimpleNode(std::string uniqueId, std::string type, std::string op, std::string args ){
	this -> uniqueId = atoi( uniqueId.c_str() );

	if( type.compare( "OP" ) == 0 ){
		this -> type = OP;
	} else if( type.compare( "VAR" ) == 0 ){
		this -> type = VAR;
	} else if( type.compare( "CONST" ) == 0 ){
		this -> type = CONST;
	} else if( type.compare( "TYPE" ) == 0 ){
		this -> type = TYPE;
	} else if( type.compare( "RET" ) == 0 ){
		this -> type = RET;
	} else if( type.compare( "IF" ) == 0 ){
		this -> type = IF;
	}  else if( type.compare( "WHILE" ) == 0 ){
		this -> type = WHILE;
	}  

	if( op.compare( "NONE" ) == 0 ){
		this -> op = NONE;
	} else if( op.compare( "ADD" ) == 0 ){
		this -> op = ADD;
	} else if( op.compare( "OR" ) == 0 ){
		this -> op = OR;
	} else if( op.compare( "XOR" ) == 0 ){
		this -> op = XOR;
	} else if( op.compare( "AND" ) == 0 ){
		this -> op = AND;
	} else if( op.compare( "SUB" ) == 0 ){
		this -> op = SUB;
	} else if( op.compare( "MUL" ) == 0 ){
		this -> op = MUL;
	} else if( op.compare( "DIV" ) == 0 ){
		this -> op = DIV;
	} else if( op.compare( "MOD" ) == 0 ){
		this -> op = MOD;
	} else if( op.compare( "UNR" ) == 0 ){
		this -> op = UNR;
	} else if( op.compare( "NOT" ) == 0 ){
		this -> op = NEG;
	} else if( op.compare( "E" ) == 0 ){
		this -> op = E;
	} else if( op.compare( "G" ) == 0 ){
		this -> op = G;
	} else if( op.compare( "S" ) == 0 ){
		this -> op = S;
	} else if( op.compare( "GOE" ) == 0 ){
		this -> op = GOE;
	} else if( op.compare( "SOE" ) == 0 ){
		this -> op = SOE;
	} 

	this -> data = args;
}

void SimpleNode::debug(){
	if(DEBUG) printf( "#%d - %d - %d: %s\n", uniqueId, type, op, data.c_str() );
}
void Node::debug(){
	if(DEBUG) printf( "#ID %d - TYPE %d - OP %d: DATA %s | VT %d VN %d VS %s\n", uniqueId, type, op, data.c_str(), varType, valueNumber, valueString.c_str() );
}


NodeType SimpleNode::getType(){
	return type;
}

OPType SimpleNode::getOP(){
	return op;
}

int SimpleNode::getId(){
	return uniqueId;
}

string SimpleNode::getData(){
	return data;
}

//Node
Node::Node( SimpleNode& s ){
	uniqueId = s.getId();
	type = s.getType();
	op = s.getOP();
	data = s.getData();
	generated = false;

	Node * child;
	for(int i = 0; i < s.numberOfChildren; i++){
		child = Node::createNode( *s.children[i] );
		children.push_back(child);
	}
}

bool Node::wasGenerated(){
	//if( type == RET) return true;
	return generated;
}

Node * Node::createNode( SimpleNode& node){
	Node* newNode;
	
 	std::map<int, Node*> nodes;
	std::list<std::pair<int, int> > connectionsQueue;
 
	switch( node.getType() ){
			case OP: 	newNode = Node::createOPNode( node, connectionsQueue ); nodes[ newNode -> getId() ] = newNode; break;
			case VAR: 	newNode = Node::createVARNode( node, connectionsQueue ); nodes[ newNode -> getId() ] = newNode; break;
			case CONST: newNode = Node::createCONSTNode( node, connectionsQueue ); nodes[ newNode -> getId() ] = newNode; break;
			case TYPE:	newNode = Node::createTYPENode( node, connectionsQueue );  nodes[ newNode -> getId() ] = newNode; break;
			case RET:	newNode = Node::createRETNode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
			case IF:	newNode = Node::createIFNode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
			case WHILE:	newNode = Node::createWHILENode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
 			//case ARRAY:	newNode = Node::createARRAYNode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
 			case ARRAY_ELEM:	newNode = Node::createARRAYELNode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
 			case ELSE:	newNode = Node::createELSENode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
 			case END_IF:	newNode = Node::createENDIFNode( node, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
 			
 			default: break;
 	}
 	
	return newNode;
}
 
Node *  Node::createAST( node_struct &node ){
	SimpleNode * root = new SimpleNode( node );
	Node * rootN = Node::createNode( *root );	
	return rootN;
}

Node * Node::createOPNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections ){
	Node * node = new OPNode( simpleNode );

	return node;
}

Node * Node::createCONSTNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new CONSTNode( simpleNode );

	return node;

}

Node * Node::createTYPENode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new TYPENode( simpleNode );

	node -> allocated = false;

	return node;

}

Node * Node::createVARNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new VARNode( simpleNode );

	return node;
}

Node * Node::createRETNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new RETNode( simpleNode );

	return node;
}

Node * Node::createIFNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new IFNode( simpleNode );

	return node;
}


Node * Node::createWHILENode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new WHILENode( simpleNode );

	return node;
}

Node * Node::createARRAYELNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new ARRAYELNode( simpleNode );

	return node;
}

Node * Node::createELSENode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new ELSENode( simpleNode );

	return node;
}

Node * Node::createENDIFNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new ENDIFNode( simpleNode );

	return node;
}

void Node::addChild( Node* n ){
	children.push_back(n);
	//printf("Add child %d\n", n -> getId() );
}

VarType Node::getVarType(){
	return varType;
}
void Node::setVarType( VarType v ){
	varType = v;
}
string Node::getVarId(){
	return id;
}
void Node::setVarId( string s ){
	id = s;
}
Value * Node::getValue(){
	return value;
}
void Node::setValue( Value * v ){
	value = v;
}
string Node::getValueString(){
	return valueString;
}
void Node::setValueString( string v ){
	valueString = v;
}
int Node::getValueNumber(){
	return valueNumber;
}
void Node::setValueNumber( int v ){
	valueNumber = v;
}
char Node::getValueLetter(){
	return valueLetter;
}
void Node::setValueLetter( char l ){
	valueLetter = l;
}
//VARNode
VARNode::VARNode( SimpleNode& s) : Node( s ){
	setVarId(s.value);
}

//Sets pointer to memory allocated for this variable
void VARNode::setAlloca( AllocaInst * a){
	alloca = a;
}

//Loads variable from memory location
Value * VARNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	//Check if variable has been allocated
	if( env.is( getVarId() ) ){
		//Get memory location
 		alloca = env.get( getVarId() ) -> alloca;
		
		//Load data
		Value * V = Builder.CreateLoad( alloca );
		return V;
	} else return 0;
}

//CONSTNode
CONSTNode::CONSTNode( SimpleNode& s) : Node( s ){
	VarType v;

	//Determine type of CONSTNode
	if( s.children[0] -> value.compare( "number" ) == 0 ){
		v = NUMBER ;
	} else if( s.children[0] -> value.compare( "letter" ) == 0 ){
		v = LETTER ;
	} else if( s.children[0] -> value.compare( "sentence" ) == 0 ){
		v = STRING ;
	} else if( s.children[0] -> value.compare( "argument" ) == 0 ){
		v = T_NONE ;
	}

	//Set its value
	switch( v ){
		case STRING: setValueString( s.value ); break;
		case NUMBER: setValueNumber( atoi(s.value.c_str()) ); break;
		case LETTER: setValueLetter( s.value[0] ); break; 
	}
}

//Returns Value of the constant
Value* CONSTNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	typeNode = (TYPENode*)children[0];
	
	if(DEBUG) printf("CONSTNode::codeGen %d\n", uniqueId );
	
	//Check type and generate corresponding code
	switch( typeNode -> getVarType() ){
		case STRING: return val = CONSTNode::codeGenSTRING( *this ); break;
		case NUMBER: return val = CONSTNode::codeGenNUMBER( *this ); break;
		case LETTER: return val = CONSTNode::codeGenLETTER( *this ); break;
	}
}

//Generates code for a letter - 8 bit integer
Value* CONSTNode::codeGenLETTER( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenLETTER CG\n");
	return ConstantInt::get( Type::getInt8Ty( getGlobalContext() ), n.getValueLetter() );
}

//Generates code for a string - constant array of 8 bit integers
Value* CONSTNode::codeGenSTRING( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenSTRING CG\n");
	return ConstantArray::get( getGlobalContext(), n.getValueString(), true);
}

//Generates code for a number - 32 bit integer 
Value* CONSTNode::codeGenNUMBER( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenNUMBER %d\n", n.getValueNumber());
	return ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), n.getValueNumber() );
}


TYPENode::TYPENode( SimpleNode& s) : Node( s ){
		
	//Sets type of the node
	if( s.value.compare( "number" ) == 0 ){
		setVarType( NUMBER );
	} else if( s.value.compare( "letter" ) == 0 ){
		setVarType( LETTER );
	} else if( s.value.compare( "sentence" ) == 0 ){
		setVarType( STRING );
	} else if( s.value.compare( "argument" ) == 0 ){
		setVarType( T_NONE );
	}
	
	arrayLength = 1;
}

//sets the length of an array represented by this type
void TYPENode::setArrayLength( int l ){
	arrayLength = l;
}

//Allocates memory for a given type, and returns address of the momory
Value * TYPENode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
 	generated = true;
	if(DEBUG) printf("TYPENode::codeGen %d\n", uniqueId );
	switch( this -> varType ){
		case STRING: return TYPENode::codeGenSTRING( *this, Builder ); break;
		case NUMBER: return TYPENode::codeGenNUMBER( *this, Builder  ); break;
		case LETTER: return TYPENode::codeGenLETTER( *this, Builder  ); break;
		case T_NONE: { //argument of a function
			//call codeGen for type node of the argument
			AllocaInst * alloca = (AllocaInst *) children[1] -> codeGen( Builder, env );
			//set Alloca 
			((VARNode*)children[0]) -> setAlloca( alloca );
			//create argument in the scope 
			children[0] -> codeGen( Builder, env);
			return alloca;
		}; break;
	}
}

//Allocates memory for a string
Value * TYPENode::codeGenSTRING(TYPENode & node, IRBuilder<> & Builder){
	Function *TheFunction = Builder.GetInsertBlock() -> getParent();
 		
	IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
	
	//Define string type
	ArrayType* StringTy = ArrayType::get(IntegerType::get(getGlobalContext(), 8), node.arrayLength);
	
	//Allocate memory
	AllocaInst * Alloca = TmpB.CreateAlloca(StringTy, 0);

	node.alloca = Alloca;	

	if(DEBUG) printf("Memory allocated\n");

	node.allocated = true;

	return node.alloca;		
}

//Allocate memory for a number
Value * TYPENode::codeGenNUMBER(TYPENode & node, IRBuilder<> & Builder){
	Function *TheFunction = Builder.GetInsertBlock() -> getParent();
 		
	IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
	
	AllocaInst * Alloca = TmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0);
 
	node.alloca = Alloca;	
 
	if(DEBUG) printf("Memory allocated\n");
 
	node.allocated = true;
 
	return node.alloca;
}
 
//Allocate memory for a letter
Value * TYPENode::codeGenLETTER(TYPENode & node, IRBuilder<> & Builder){
	Function *TheFunction = Builder.GetInsertBlock() -> getParent();
		
	IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
	
	AllocaInst * Alloca = TmpB.CreateAlloca(Type::getInt8Ty(getGlobalContext()), 0);

	node.alloca = Alloca;	

	if(DEBUG) printf("Memory allocated\n");

	node.allocated = true;

	return node.alloca;	
}

//Returns type of the node in its llvm representation
Type * TYPENode::getLlvmType(){
	switch( getVarType() ){
		case STRING: return (Type*)ArrayType::get(IntegerType::get(getGlobalContext(), 8), arrayLength); break;
		case NUMBER: return (Type*)Type::getInt32Ty(getGlobalContext()); break;
		case LETTER: return (Type*)Type::getInt8Ty(getGlobalContext()); break; 
	}
}

Type * TYPENode::getLlvmArgType(){
	return ((TYPENode*)children[1]) -> getLlvmType(); 
}

string TYPENode::getArgName(){
	return ((VARNode*)children[0]) -> getVarId(); 
}

RETNode::RETNode( SimpleNode& s) : Node( s ){

}

//Generates code for the value it's supposed to return and creates a Ret statement
Value * RETNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	if(DEBUG) printf("RETNode::codeGen %d\n", uniqueId);
	
	Value * v = children[0] -> codeGen( Builder, env ); 

  	Builder.CreateRet( v );
	return v;
}

IFNode::IFNode( SimpleNode& s) : Node( s ){
	
}

//Generates code for if blocks
Value * IFNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	
	//Extract condition, instructions and possible else node
	Node * condition = children[0];
	vector<Node *> body (children.begin() + 1, children.end() - 1 );
	Node * else_node = children[numberOfChildren - 1];
	
	//Generate code for the condition
	Value * condV = condition -> codeGen( Builder, env );

	//Convert condition to bool
	condV = Builder.CreateICmpEQ( condV, 
                              ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 0 ) ),
                                "ifcond" );

    //Get overlaying function
	Function *theFunction = Builder.GetInsertBlock() -> getParent();
 	
 	//Generate blocks for each branch, and a merging branch
	BasicBlock *thenBB = BasicBlock::Create( getGlobalContext(), "then", theFunction );
	BasicBlock *elseBB = BasicBlock::Create( getGlobalContext(), "else" );
	BasicBlock *mergeBB = BasicBlock::Create( getGlobalContext(), "ifcont" );

	//Create condition jump
	Builder.CreateCondBr(condV, thenBB, elseBB);

	//Start inserting code into BasicBlock thenBB
    Builder.SetInsertPoint( thenBB );


    Value *thenV;
    //Generate instructions' code
    for(std::vector< Node * >::iterator it = body.begin(); it != body.end(); ++it) {
	    thenV = (*it) -> codeGen( Builder, env );
  	}

  	//Create jump to merge block
    Builder.CreateBr( mergeBB );
    thenBB = Builder.GetInsertBlock();
    
    //Insert elseBB BacicBlock, and start inserting into it
    theFunction -> getBasicBlockList().push_back( elseBB );
	Builder.SetInsertPoint( elseBB );
	  
	//Generate code of else node - which is either Else, Else If or End If
	Value *elseV = else_node -> codeGen( Builder, env );
	  
	//Create jump from else branch to the merging block
	Builder.CreateBr( mergeBB );

	// Codegen of 'Else' can change the current block, update ElseBB for the PHI.
	elseBB = Builder.GetInsertBlock();

	theFunction -> getBasicBlockList().push_back( mergeBB );
    
    //Start inserting into merge Block, generate PhiNode
    Builder.SetInsertPoint( mergeBB );
    PHINode *PN = Builder.CreatePHI( Type::getInt32Ty( getGlobalContext() ) );
  
    PN -> addIncoming( thenV, thenBB );
    PN -> addIncoming( elseV, elseBB );
    
    return PN;
}

ELSENode::ELSENode( SimpleNode& s) : Node( s ){
	
}

// Generates code for else{} element
Value * ELSENode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	
	// get instructions and generate them
	vector<Node *> body (children.begin(), children.end() - 1 );
		
	for(std::vector< Node * >::iterator it = body.begin(); it != body.end(); ++it) {
	    (*it) -> codeGen( Builder, env );
  	}

    return 0;
}

ENDIFNode::ENDIFNode( SimpleNode& s) : Node( s ){
	
}

// Responsible for finishing generation of if else statement
Value * ENDIFNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	return 0;
}

WHILENode::WHILENode( SimpleNode& s) : Node( s ){
	
}

//Generates code for Looops
Value * WHILENode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	
	//Get condition, and instructions which will be executed if it's true
	Node * condition = children[0];
	vector<Node *> body (children.begin() + 1, children.end());
	
	if(DEBUG) printf("WHILENode::codeGen %d\n", uniqueId);
	
	//Get necessary code building structures
	Function *theFunction = Builder.GetInsertBlock() -> getParent();
 	
	BasicBlock *whileBB = BasicBlock::Create( getGlobalContext(), "while", theFunction );
	BasicBlock *bodyBB = BasicBlock::Create( getGlobalContext(), "body" );
	BasicBlock *endBB = BasicBlock::Create( getGlobalContext(), "end" );

	//Create jump to while branch (condition), and start insertion into it
	Builder.CreateBr( whileBB );
    Builder.SetInsertPoint( whileBB );

    //Generate condition, and convert its value to bool
    Value * condV = condition -> codeGen( Builder, env );

	condV = Builder.CreateICmpEQ( condV, 
                              ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 0 ) ),
                                "loopcond" );
	
	//Create conditional jump, if condition is true execute instructions, otherwise terminate
	Builder.CreateCondBr(condV, bodyBB, endBB);

	theFunction -> getBasicBlockList().push_back( bodyBB );
    Builder.SetInsertPoint( bodyBB );

    //Generate code of instructions
    for(std::vector< Node * >::iterator it = body.begin(); it != body.end(); ++it) {
	    (*it) -> codeGen( Builder, env );
	}

	//Jump back to while branch (loop)
    Builder.CreateBr( whileBB );
    bodyBB = Builder.GetInsertBlock();

    //Create terminating block
    theFunction -> getBasicBlockList().push_back( endBB );
	Builder.SetInsertPoint( endBB );    

 	
    Value * ret = ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 0 ) );
	//theFunction -> dump();
    return ret;

}

OPNode::OPNode( SimpleNode& s) : Node( s ){

	//Set type of operation
	if( s.value.compare( "NO_OP" ) == 0 ){
		this -> op = NONE;
	} else if( s.value.compare( "ADD" ) == 0 ){
		this -> op = ADD;
	} else if( s.value.compare( "OR" ) == 0 ){
		this -> op = OR;
	} else if( s.value.compare( "XOR" ) == 0 ){
		this -> op = XOR;
	} else if( s.value.compare( "AND" ) == 0 ){
		this -> op = AND;
	} else if( s.value.compare( "SUBTRACT" ) == 0 ){
		this -> op = SUB;
	} else if( s.value.compare( "MULTIPLY" ) == 0 ){
		this -> op = MUL;
	} else if( s.value.compare( "DIVIDE" ) == 0 ){
		this -> op = DIV;
	} else if( s.value.compare( "MOD" ) == 0 ){
		this -> op = MOD;
	} else if( s.value.compare( "NOT" ) == 0 ){
		this -> op = NEG;
	} else if( s.value.compare( "NEGATE" ) == 0 ){
		this -> op = NOT;
	} else if( s.value.compare( "LOGICAL_OR" ) == 0 ){
		this -> op = BOOL_OR;
	} else if( s.value.compare( "LOGICAL_AND" ) == 0 ){
		this -> op = BOOL_AND;
	} else if( s.value.compare( "EQUAL" ) == 0 ){
		this -> op = E;
	} else if( s.value.compare( "NOT_EQUAL" ) == 0 ){
		this -> op = NE;
	} else if( s.value.compare( "GREATER_THAN" ) == 0 ){
		this -> op = G;
	} else if( s.value.compare( "LESS_THAN" ) == 0 ){
		this -> op = S;
	} else if( s.value.compare( "LESS_THAN_EQUAL" ) == 0 ){
		this -> op = SOE;
	} else if( s.value.compare( "GREATER_THAN_EQUAL" ) == 0 ){
		this -> op = GOE;
	} 
}

//Generate code specific for each operation
Value* OPNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	if(DEBUG) printf("OPNode::codeGen %d\n", uniqueId);

	//Generate code for left hand side of the operation
	lhs = children[0] -> codeGen( Builder, env );


	//Generate code for right hand side of the operation, if the operation has two arguments
	if(this -> op != UNR && this -> op != NEG && this -> op != NOT) rhs = children[1] -> codeGen( Builder, env );
	
	switch( this -> op ){
		case ADD: return OPNode::codeGenADD( Builder, *this ); break;
		case OR: return OPNode::codeGenOR( Builder, *this ); break;
		case XOR: return OPNode::codeGenXOR( Builder, *this ); break;
		case AND: return OPNode::codeGenAND( Builder, *this ); break;
		case SUB: return OPNode::codeGenSUB( Builder, *this ); break;
		case MUL: return OPNode::codeGenMUL( Builder, *this ); break;
		case DIV: return OPNode::codeGenDIV( Builder, *this ); break;
		case MOD: return OPNode::codeGenMOD( Builder, *this ); break;
		case NEG: return OPNode::codeGenNEG( Builder, *this ); break;
		case S: return OPNode::codeGenS( Builder, *this ); break;
		case G: return OPNode::codeGenG( Builder, *this ); break;
		case E: return OPNode::codeGenE( Builder, *this ); break;
		case NE: return OPNode::codeGenNE( Builder, *this ); break;
		case BOOL_OR: return OPNode::codeGenBOR( Builder, *this ); break;
		case BOOL_AND: return OPNode::codeGenBAND( Builder, *this ); break;
		case GOE: return OPNode::codeGenGOE( Builder, *this ); break;
		case SOE: return OPNode::codeGenSOE( Builder, *this ); break;
	}
}

Value* OPNode::codeGenADD( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateAdd( n.lhs, n.rhs );
}

Value* OPNode::codeGenOR( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateOr( n.lhs, n.rhs );
}

Value* OPNode::codeGenBAND( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateAnd( n.lhs, n.rhs );
}

Value* OPNode::codeGenNOT( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateNeg( n.lhs );
}

Value* OPNode::codeGenBOR( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateOr( n.lhs, n.rhs );
}

Value* OPNode::codeGenXOR(llvm::IRBuilder<> & Builder,  OPNode & n ){
	return Builder.CreateXor( n.lhs, n.rhs );
}

Value* OPNode::codeGenAND( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateAnd( n.lhs, n.rhs );
}

Value* OPNode::codeGenSUB( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateSub( n.lhs, n.rhs );
}

Value* OPNode::codeGenMUL( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateMul( n.lhs, n.rhs );
}

Value* OPNode::codeGenDIV( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateUDiv( n.lhs, n.rhs );
}

Value* OPNode::codeGenMOD( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateURem( n.lhs, n.rhs	);
}

Value* OPNode::codeGenNEG( llvm::IRBuilder<> & Builder, OPNode & n ){
	return Builder.CreateNot( n.lhs );
}

Value* OPNode::codeGenS( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpULT( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}

Value* OPNode::codeGenG( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpUGT( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}

Value* OPNode::codeGenE( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpEQ( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}

Value* OPNode::codeGenNE( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpNE( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}

Value* OPNode::codeGenGOE( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpUGE( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}

Value* OPNode::codeGenSOE( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpULE( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}


FUNCTIONNode::FUNCTIONNode( SimpleNode& s) : Node( s ){
	//Set name of the function
	funName = s.value;
}

string FUNCTIONNode::getFunName(){
	return funName;
}

//Returns llvm types of arguments
vector<const Type*> FUNCTIONNode::getArgsTypes(){
	return args;
}

//Returns names of the arguments
vector<string> FUNCTIONNode::getArgs(){
	return argsNames;
}

//Generates code for arguments of a function
Value * FUNCTIONNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	for(int i = 0; i < children.size(); i++){
		children[i] -> codeGen(Builder, env);

		//collect types of arguments
		args.push_back( ((TYPENode*)children[i]) -> getLlvmArgType());
		argsNames.push_back( ((TYPENode*)children[i]) -> getArgName());
	}	
	return 0;
}

FUNCTIONDEFNode::FUNCTIONDEFNode( SimpleNode& s) : Node( s ){

}

Function * FUNCTIONDEFNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	//Set node responsible for definition of arguments
	Fn = (FUNCTIONNode*)children[1];
	
	//Set function name
	funName = Fn -> getFunName();
	
	//Create new scope
	env.add( Fn -> getFunName(), this);

	env.addScope( Fn -> getFunName() );

	env = env.getScope( Fn -> getFunName() );

	//Generate arguments
	Fn -> codeGen(Builder, env);

	//Declare function type
	FT = FunctionType::get(((TYPENode*)children[0]) -> getLlvmType(), Fn -> getArgsTypes(), false);
	
	//Define function
	F = Function::Create(FT, Function::ExternalLinkage, Fn -> getFunName(), theModule);
	

	//Create new insertion block
	Function *theFunction = Builder.GetInsertBlock() -> getParent();
	BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", theFunction);
  	Builder.SetInsertPoint(BB);
	
	//Generate body
	for(int i = 2; i < children.size(); i++){
		children[i] -> codeGen(Builder, env);
	}

	return F;
}

FUNCTIONCALLNode::FUNCTIONCALLNode( SimpleNode& s) : Node( s ){
	funName = s.value;
}

Value * FUNCTIONCALLNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	if( funName.compare( "became" ) == 0 ){
		//Get variable from environment
		Node * x = env.get( children[0] -> getVarId() ); 

		//Get its new value
		Value * val = children[1] -> codeGen( Builder, env );
		
		//Assign value to the variable
		Value * V = Builder.CreateStore( val, x -> alloca );
		return 0;
	} else if( funName.compare( "ate" ) == 0 ){ //++
		return Builder.CreateAdd( children[0] -> codeGen(Builder, env), ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 1 ) ) );
	} else if( funName.compare( "drank" ) == 0 ){ //--
		return Builder.CreateSub( children[0] -> codeGen(Builder, env), ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 1 ) ) );
	} else if( funName.compare( "had" ) == 0 ){
		Node * x = children[0];

		//Add variable to the scope
		env.add( x -> getVarId(), x );

		//Get size
		Value * size = children[1] -> codeGen( Builder, env );

		Type * t = ((TYPENode*)children[2]) -> getLlvmType();

		//Get pointer of the type
		PointerType* Pt = PointerType::get(t, 0);

		//Allocate memory (type node)
		x -> alloca = Builder.CreateAlloca(Pt, size);

		return 0;
	} else if( funName.compare( "was" ) == 0 ){
		Node * x = children[0];

		//Add variable to the scope
		env.add( x -> getVarId(), x );

		//Allocate memory (type node)
		AllocaInst * alloca = (AllocaInst *) children[1] -> codeGen( Builder, env );
		
		//Set variable's memory location
		x -> alloca = alloca;

		return 0;
	} else{
		Function * f = theModule -> getFunction( funName );
		vector<Value *> f_args;
		vector<string> f_argsNames = ((FUNCTIONDEFNode*) env.get( funName )) -> Fn -> getArgs();
		for( int i = 0; i < f_argsNames.size(); i++ ){
			f_args.push_back( env.get( f_argsNames[i] ) -> codeGen(Builder, env) );
		}
		return Builder.CreateCall(f, f_args.begin(), f_args.end(), funName);
	}
}

IONode::IONode( SimpleNode& s) : Node( s ){
	funName = s.value;
}


// Generates code responsible for handling IO operations
Value * IONode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	if( funName.compare( "what was" ) == 0 ){
		Node * x = env.get( children[0] -> getVarId() ); 

		vector<const Type*> FuncTy_7_args;
		PointerType* PointerTy_4 = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	 	FuncTy_7_args.push_back(PointerTy_4);
	 	FunctionType* FuncTyScanf = FunctionType::get( IntegerType::get(getGlobalContext(), 32), FuncTy_7_args, true);
		
		Function* func_scanf = Function::Create(FuncTyScanf, Function::ExternalLinkage, "scanf", theModule);
		
		 ArrayType* ArrayTy_0 = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 3);
		 GlobalVariable* gvar_array__str = new GlobalVariable( *theModule, ArrayTy_0, true, GlobalValue::PrivateLinkage, 0, ".str");
		 
		 // Constant Definitions
		 Constant* const_array_8 = ConstantArray::get(getGlobalContext(), "%d", true);

		 std::vector<Constant*> const_ptr_10_indices;
		 ConstantInt* const_int64_11 = ConstantInt::get(getGlobalContext(), APInt(64, StringRef("0"), 10));
		 const_ptr_10_indices.push_back(const_int64_11);
		 const_ptr_10_indices.push_back(const_int64_11);
		 Constant* const_ptr_10 = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_10_indices[0], const_ptr_10_indices.size());

		 gvar_array__str->setInitializer(const_array_8);

		std::vector<Value*> int32_14_params;
	  	int32_14_params.push_back(const_ptr_10);
	  	int32_14_params.push_back(x -> alloca);

		Builder.CreateCall(func_scanf, int32_14_params.begin(), int32_14_params.end());
		return Builder.CreateLoad(x->alloca);
	} else if( funName.compare( "spoke" ) == 0 ){
		
		ArrayType* ArrayTy_2 = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 4);

    	PointerType* PointerTy_5 = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
		std::vector<const Type*>FuncTy_12_args;
	 	FuncTy_12_args.push_back(PointerTy_5);
	 	FunctionType* FuncTy_12 = FunctionType::get( IntegerType::get(getGlobalContext(), 32), FuncTy_12_args, true);

     	Function* func_printf = Function::Create( FuncTy_12, GlobalValue::ExternalLinkage,	"printf", theModule);
 		GlobalVariable* gvar_array__str = new GlobalVariable( *theModule, ArrayTy_2, true, GlobalValue::PrivateLinkage, 0, ".str");

	 	Constant* printfFormat; 
	 	Constant* ptrToFormat;
	 	Value * valueToPrint = children[0] -> codeGen( Builder, env);
	 	Value * ptrToValue;
	 	const Type * valToPrintTy = valueToPrint -> getType();
	 
	 	ConstantInt* const_int32_14 = ConstantInt::get(getGlobalContext(), APInt(32, StringRef("0"), 10));
	 	ConstantInt* const_int64_14 = ConstantInt::get(getGlobalContext(), APInt(64, StringRef("0"), 10));
	 
	 	std::vector<Constant*> const_ptr_18_indices;
	 	const_ptr_18_indices.push_back(const_int32_14);
	 	const_ptr_18_indices.push_back(const_int32_14);
	 
	 	ptrToFormat = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_18_indices[0], const_ptr_18_indices.size());
	 
		 if( valToPrintTy -> isIntegerTy(8)){
		 	printfFormat = ConstantArray::get(getGlobalContext(), "%c\x0A", true);
		 	AllocaInst * allocaOfValue = Builder.CreateAlloca(valToPrintTy);
		 	Builder.CreateStore(valueToPrint, allocaOfValue);
		 	ptrToValue = Builder.CreateLoad(allocaOfValue);
		 } else if( valToPrintTy -> isArrayTy()){
		 	printfFormat = ConstantArray::get(getGlobalContext(), "%s\x0A", true);
		 	int arraySize = ((ConstantArray*)valueToPrint) -> getAsString().length();
		 	
		 	GlobalVariable* gvar_array_str = new GlobalVariable( *theModule, valToPrintTy, false, GlobalValue::ExternalLinkage, 0, ".str");
		 	gvar_array_str -> setInitializer((Constant*)valueToPrint);

		 	std::vector<Constant*> const_ptr_14_indices;
	 		const_ptr_14_indices.push_back(const_int32_14);
	 		const_ptr_14_indices.push_back(const_int32_14);
	 		ptrToValue = ConstantExpr::getGetElementPtr(gvar_array_str, &const_ptr_14_indices[0], const_ptr_14_indices.size());

		} else {
			printfFormat = ConstantArray::get(getGlobalContext(), "%d\x0A", true);	 	
		 	AllocaInst * allocaOfValue = Builder.CreateAlloca(valToPrintTy);
		 	Builder.CreateStore(valueToPrint, allocaOfValue);
		 	ptrToValue = Builder.CreateLoad(allocaOfValue);	 	
		}

		gvar_array__str -> setInitializer(printfFormat);

	    std::vector<Value*> int32_25_params;
	  	int32_25_params.push_back(ptrToFormat);
	  	int32_25_params.push_back(ptrToValue);

		Builder.CreateCall(func_printf, int32_25_params.begin(), int32_25_params.end());
		return 0;
	} else if( funName.compare( "found" ) == 0 ){

		Value * v = children[0] -> codeGen( Builder, env ); 

	  	Builder.CreateRet( v );
		return v;
	
	} else if( funName.compare( "said Alice" ) == 0 ){
		ArrayType* ArrayTy_2 = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 4);

    	PointerType* PointerTy_5 = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
		std::vector<const Type*>FuncTy_12_args;
	 	FuncTy_12_args.push_back(PointerTy_5);
	 	FunctionType* FuncTy_12 = FunctionType::get( IntegerType::get(getGlobalContext(), 32), FuncTy_12_args, true);

     	Function* func_printf = Function::Create( FuncTy_12, GlobalValue::ExternalLinkage,	"printf", theModule);
 		GlobalVariable* gvar_array__str = new GlobalVariable( *theModule, ArrayTy_2, true, GlobalValue::PrivateLinkage, 0, ".str");

	 	Constant* printfFormat; 
	 	Constant* ptrToFormat;
	 	Value * valueToPrint = children[0] -> codeGen( Builder, env);
	 	Value * ptrToValue;
	 	const Type * valToPrintTy = valueToPrint -> getType();
	 
	 	ConstantInt* const_int32_14 = ConstantInt::get(getGlobalContext(), APInt(32, StringRef("0"), 10));
	 	ConstantInt* const_int64_14 = ConstantInt::get(getGlobalContext(), APInt(64, StringRef("0"), 10));
	 
	 	std::vector<Constant*> const_ptr_18_indices;
	 	const_ptr_18_indices.push_back(const_int32_14);
	 	const_ptr_18_indices.push_back(const_int32_14);
	 
	 	ptrToFormat = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_18_indices[0], const_ptr_18_indices.size());
	 
		 if( valToPrintTy -> isIntegerTy(8)){
		 	printfFormat = ConstantArray::get(getGlobalContext(), "%c\x0A", true);
		 	AllocaInst * allocaOfValue = Builder.CreateAlloca(valToPrintTy);
		 	Builder.CreateStore(valueToPrint, allocaOfValue);
		 	ptrToValue = Builder.CreateLoad(allocaOfValue);
		 } else if( valToPrintTy -> isArrayTy()){
		 	printfFormat = ConstantArray::get(getGlobalContext(), "%s\x0A", true);
		 	int arraySize = ((ConstantArray*)valueToPrint) -> getAsString().length();
		 	
		 	GlobalVariable* gvar_array_str = new GlobalVariable( *theModule, valToPrintTy, false, GlobalValue::ExternalLinkage, 0, ".str");
		 	gvar_array_str -> setInitializer((Constant*)valueToPrint);

		 	std::vector<Constant*> const_ptr_14_indices;
	 		const_ptr_14_indices.push_back(const_int32_14);
	 		const_ptr_14_indices.push_back(const_int32_14);
	 		ptrToValue = ConstantExpr::getGetElementPtr(gvar_array_str, &const_ptr_14_indices[0], const_ptr_14_indices.size());

		} else {
			printfFormat = ConstantArray::get(getGlobalContext(), "%d\x0A", true);	 	
		 	AllocaInst * allocaOfValue = Builder.CreateAlloca(valToPrintTy);
		 	Builder.CreateStore(valueToPrint, allocaOfValue);
		 	ptrToValue = Builder.CreateLoad(allocaOfValue);	 	
		}

		gvar_array__str -> setInitializer(printfFormat);

	    std::vector<Value*> int32_25_params;
	  	int32_25_params.push_back(ptrToFormat);
	  	int32_25_params.push_back(ptrToValue);

		Builder.CreateCall(func_printf, int32_25_params.begin(), int32_25_params.end());
	}
}

ARRAYELNode::ARRAYELNode( SimpleNode& s) : Node( s ){
	arrName = s.value;
}


// Generates code responsible for handling IO operations
Value * ARRAYELNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	Node * arr = env.get(arrName);
	Value * elNumber = children[1] -> codeGen(Builder, env);

	 Value* ptr_10 = Builder.CreateGEP(arr -> alloca, elNumber);

	return Builder.CreateLoad(ptr_10);
}