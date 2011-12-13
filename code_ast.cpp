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
		this -> type = IF;
	} else if( string(s.type).compare( "END_IF" ) == 0 ){
		this -> type = IF;
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

	std::size_t pos;

	string type;
	
	std::string dataChunkRaw;
	
	string data = node -> getData(); 

	if(DEBUG) printf("%s\n", data.c_str() );

	int i = 0;

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		dataChunkRaw = data.substr( 0, pos );

		if(DEBUG) printf("%s\n", dataChunkRaw.c_str() );

		if( i == 0 ){
			if( dataChunkRaw.compare( "STRING" ) == 0 ){
				node -> setVarType( STRING );
			} else if( dataChunkRaw.compare( "NUMBER" ) == 0 ){
				node -> setVarType( NUMBER );
			} else if( dataChunkRaw.compare( "LETTER" ) == 0 ){
				node -> setVarType( LETTER );
			}
		} else if( i == 1 ){ 
			node -> setVarId( dataChunkRaw );
			mapOfTypes[ node -> getVarId() ] = node -> getVarType();
		}

		if(data.length() > pos) data = data.substr( pos + 1 );
	
		i++;
	}

	node -> allocated = false;

	if(DEBUG) printf( "Create TYPE - VAR Node id: %d %s type: %d\n", node -> getId(), node -> getVarId().c_str(), node -> getVarType() );

	return node;

}

Node * Node::createVARNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new VARNode( simpleNode );

	
	return node;
}

Node * Node::createRETNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new RETNode( simpleNode );

	std::size_t pos;

	string dataChunkRaw;
	
	string data = node -> getData(); 

	int i = 0;

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		dataChunkRaw = data.substr( 0, pos );

		if( i == 0 && dataChunkRaw.length() > 0){
			node -> setVarId( dataChunkRaw );
		} else if( i == 1 ) {
			connections.push_back( make_pair( node -> getId(), atoi( dataChunkRaw.c_str() ) ) );
		}

		if(data.length() > pos) data = data.substr( pos + 1 );
	
		i++;
	}

	if(DEBUG) printf( "Create RETNode id: %d %s type: %d\n", node -> getId(), node -> getVarId().c_str(), node -> getVarType() );

	return node;
}

Node * Node::createIFNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	IFNode * node = new IFNode( simpleNode );

	std::size_t pos;

	string dataChunkRaw;
	
	string data = node -> getData(); 

	int i = 0;

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		dataChunkRaw = data.substr( 0, pos );

		/*if( i == 0 && dataChunkRaw.length() > 0){
			node -> setCondId( dataChunkRaw );
		} else {*/
			connections.push_back( make_pair( node -> getId(), atoi( dataChunkRaw.c_str() ) ) );
		/*}*/

		if(data.length() > pos) data = data.substr( pos + 1 );
	
		i++;
	}

	if(DEBUG) printf( "Create IFNode id: %d %s type: %d\n", node -> getId(), node -> getVarId().c_str(), node -> getVarType() );

	return node;
}


Node * Node::createWHILENode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	WHILENode * node = new WHILENode( simpleNode );

	std::size_t pos;

	string dataChunkRaw;
	
	string data = node -> getData(); 

	int i = 0;

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		dataChunkRaw = data.substr( 0, pos );

		/*if( i == 0 && dataChunkRaw.length() > 0){
			node -> setCondId( dataChunkRaw );
		} else {*/
			connections.push_back( make_pair( node -> getId(), atoi( dataChunkRaw.c_str() ) ) );
		/*}*/

		if(data.length() > pos) data = data.substr( pos + 1 );
	
		i++;
	}

	if(DEBUG) printf( "Create WHILENode id: %d %s type: %d\n", node -> getId(), node -> getVarId().c_str(), node -> getVarType() );

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
	
}

Value * VARNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	if( env.is( getVarId() ) ){
 		alloca = env.get( getVarId() ) -> alloca;
		Value * V = Builder.CreateLoad( alloca );
		return V;
	} else return 0;
}

//CONSTNode
CONSTNode::CONSTNode( SimpleNode& s) : Node( s ){
	
}

Value* CONSTNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	if(DEBUG) printf("CONSTNode::codeGen %d\n", uniqueId );
	switch( children[0] -> getVarType() ){
		case STRING: return CONSTNode::codeGenSTRING( *this ); break;
		case NUMBER: return CONSTNode::codeGenNUMBER( *this ); break;
		case LETTER: return CONSTNode::codeGenLETTER( *this ); break;
	}
}

Value* CONSTNode::codeGenLETTER( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenLETTER CG\n");
	out += n.getValueString();
	//printf("STRING: %s\n", n.getValueString().c_str());
	return ConstantInt::get( Type::getInt8Ty( getGlobalContext() ), n.getValueNumber() );
}

Value* CONSTNode::codeGenSTRING( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenSTRING CG\n");
	out += n.getValueString();
	//printf("STRING: %s\n", n.getValueString().c_str());
	return ConstantArray::get( getGlobalContext(), n.getValueString(), true);
}

Value* CONSTNode::codeGenNUMBER( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenNUMBER %d\n", n.getValueNumber());
	return ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), n.getValueNumber() );
}


TYPENode::TYPENode( SimpleNode& s) : Node( s ){
	
}

Value * TYPENode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
 	generated = true;
	if(DEBUG) printf("TYPENode::codeGen %d\n", uniqueId );
	switch( this -> varType ){
		case STRING: return TYPENode::codeGenSTRING( *this, Builder ); break;
		case NUMBER: return TYPENode::codeGenNUMBER( *this, Builder  ); break;
		case LETTER: return TYPENode::codeGenLETTER( *this, Builder  ); break;
	}
}
 
Value * TYPENode::codeGenSTRING(TYPENode & node, IRBuilder<> & Builder){
	Function *TheFunction = Builder.GetInsertBlock() -> getParent();
 		
	IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
	
	ArrayType* StringTy = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 1);
	AllocaInst * Alloca = TmpB.CreateAlloca(StringTy, 0);

	node.alloca = Alloca;	

	if(DEBUG) printf("Memory allocated\n");

	node.allocated = true;

	return node.alloca;		
}

Value * TYPENode::codeGenNUMBER(TYPENode & node, IRBuilder<> & Builder){
	Function *TheFunction = Builder.GetInsertBlock() -> getParent();
 		
	IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
	
	AllocaInst * Alloca = TmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0);
 
	node.alloca = Alloca;	
 
	if(DEBUG) printf("Memory allocated\n");
 
	node.allocated = true;
 
	return node.alloca;
}
 
Value * TYPENode::codeGenLETTER(TYPENode & node, IRBuilder<> & Builder){
	Function *TheFunction = Builder.GetInsertBlock() -> getParent();
		
	IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
	
	AllocaInst * Alloca = TmpB.CreateAlloca(Type::getInt8Ty(getGlobalContext()), 0);

	node.alloca = Alloca;	

	if(DEBUG) printf("Memory allocated\n");

	node.allocated = true;

	return node.alloca;	
}

ARRAYNode::ARRAYNode( SimpleNode& s) : Node( s ){

}

Value * ARRAYNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	
}



RETNode::RETNode( SimpleNode& s) : Node( s ){

}

Value * RETNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	if(DEBUG) printf("RETNode::codeGen %d\n", uniqueId);
	
	Value * v;

	if( getVarId().length() > 0 ){
		v = env.get( getVarId() ) -> codeGen( Builder, env );	
	} else {
		lhs = children[0] -> codeGen( Builder, env );
		v = lhs; 
	} 

  	Builder.CreateRet( v );
	return v;
}

IFNode::IFNode( SimpleNode& s) : Node( s ){
	
}

void IFNode::setCondId( string id ){
	cond_id = id;
}

Node * IFNode::getCondNode(){
	if( cond_id.length() == 0 ){
		cond = children[0];
	}

	return cond;
}



Value * IFNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	else_node = children[1];
	
	std::vector< Node * >::iterator itc = children.begin();
	itc++;
	itc++;
	for(; itc != children.end(); ++itc) {
	    body.push_back( *itc );
  	}

	if(DEBUG) printf("IFNode::codeGen %d\n", uniqueId);
	
	Value * condV = getCondNode() -> codeGen( Builder, env );

	condV = Builder.CreateICmpEQ( condV, 
                              ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 0 ) ),
                                "ifcond" );

	Function *theFunction = Builder.GetInsertBlock() -> getParent();
 	
	BasicBlock *thenBB = BasicBlock::Create( getGlobalContext(), "then", theFunction );
	BasicBlock *elseBB = BasicBlock::Create( getGlobalContext(), "else" );
	BasicBlock *mergeBB = BasicBlock::Create( getGlobalContext(), "ifcont" );

	Builder.CreateCondBr(condV, thenBB, elseBB);

    Builder.SetInsertPoint( thenBB );

    Value *thenV;

    for(std::list< Node * >::iterator it = body.begin(); it != body.end(); ++it) {
	    thenV = (*it) -> codeGen( Builder, env );
	    //if (thenV == 0) return 0;
  	}

    Builder.CreateBr( mergeBB );
    thenBB = Builder.GetInsertBlock();
    
    theFunction -> getBasicBlockList().push_back( elseBB );
	Builder.SetInsertPoint( elseBB );
	  
	Value *elseV = else_node -> codeGen( Builder, env );
	//if (elseV == 0) return 0;
	  
	Builder.CreateBr( mergeBB );
	// Codegen of 'Else' can change the current block, update ElseBB for the PHI.
	elseBB = Builder.GetInsertBlock();

	theFunction -> getBasicBlockList().push_back( mergeBB );
    Builder.SetInsertPoint( mergeBB );
    PHINode *PN = Builder.CreatePHI( Type::getInt32Ty( getGlobalContext() ) );
  
    PN -> addIncoming( thenV, thenBB );
    PN -> addIncoming( elseV, elseBB );
    
    return PN;
}

WHILENode::WHILENode( SimpleNode& s) : IFNode( s ){
	
}

Value * WHILENode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	
	std::vector< Node * >::iterator itc = children.begin();
	itc++;
	for(; itc != children.end(); ++itc) {
	    body.push_back( *itc );
  	}

	if(DEBUG) printf("WHILENode::codeGen %d\n", uniqueId);
	
	Function *theFunction = Builder.GetInsertBlock() -> getParent();
 	
	BasicBlock *whileBB = BasicBlock::Create( getGlobalContext(), "while", theFunction );
	BasicBlock *bodyBB = BasicBlock::Create( getGlobalContext(), "body" );
	BasicBlock *endBB = BasicBlock::Create( getGlobalContext(), "end" );

	Builder.CreateBr( whileBB );
    Builder.SetInsertPoint( whileBB );

    Value * condV = getCondNode() -> codeGen( Builder, env );

	condV = Builder.CreateICmpEQ( condV, 
                              ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 0 ) ),
                                "loopcond" );
	Builder.CreateCondBr(condV, bodyBB, endBB);

   

	theFunction -> getBasicBlockList().push_back( bodyBB );
    Builder.SetInsertPoint( bodyBB );

    Value *thenV;

    for(std::list< Node * >::iterator it = body.begin(); it != body.end(); ++it) {
	    thenV = (*it) -> codeGen( Builder, env );
	    //if (thenV == 0) return 0;
  	}

    Builder.CreateBr( whileBB );
    bodyBB = Builder.GetInsertBlock();

    theFunction -> getBasicBlockList().push_back( endBB );
	Builder.SetInsertPoint( endBB );    

 	
    Value * ret = ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), APInt( 32, 0 ) );
	//theFunction -> dump();
    return ret;

}

OPNode::OPNode( SimpleNode& s) : Node( s ){
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

Value* OPNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	if(DEBUG) printf("OPNode::codeGen %d\n", uniqueId);
	lhs = children[0] -> codeGen( Builder, env );
	if(this -> op != UNR && this -> op != NEG) rhs = children[1] -> codeGen( Builder, env );
	
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
	if( n.children[0] -> getType() == CONST && n.children[1] -> getType() == CONST ) {
		int o = n.children[0] -> getValueNumber() * n.children[1] -> getValueNumber();
		bool overflow = false;

		if( n.children[0] -> getValueNumber() > 0 &&  n.children[1] -> getValueNumber() > 0){
			overflow = o < n.children[0] -> getValueNumber() && o < n.children[1] -> getValueNumber();
		} else {
			overflow = o > n.children[0] -> getValueNumber() && o > n.children[1] -> getValueNumber();
		}

		if( overflow ){
			cout << "This value is too big!";
			exit( 1 );
		}
	}
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
