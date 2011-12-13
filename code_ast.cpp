#include <cstdio>
#include <iostream>

#include "code_ast.hpp"

using namespace llvm;
using namespace std;

#define DEBUG false
//SimpleNode
SimpleNode::SimpleNode(){}

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
}

bool Node::wasGenerated(){
	//if( type == RET) return true;
	return generated;
}

std::map<int, Node*> Node::createAST( std::map<int, SimpleNode*>& sn ){
	std::list<std::pair<int, int> > connectionsQueue;
	std::list<std::pair<int, int> >::iterator conIt;

	std::map<int, SimpleNode*>::iterator it = sn.begin();
	std::map<int, Node*> nodes;
	
	//Generate root of AST
	Node * root;

	SimpleNode* processed;
	Node* newNode;

	//Generate children

	while( it != sn.end() ){
		processed = ( (*( it )).second );

		switch( processed -> getType() ){
			case OP: 	newNode = Node::createOPNode( *(*it).second, connectionsQueue ); nodes[ newNode -> getId() ] = newNode; break;
			case VAR: 	newNode = Node::createVARNode( *(*it).second, connectionsQueue ); nodes[ newNode -> getId() ] = newNode; break;
			case CONST: newNode = Node::createCONSTNode( *(*it).second, connectionsQueue ); nodes[ newNode -> getId() ] = newNode; break;
			case TYPE:	newNode = Node::createTYPENode( *(*it).second, connectionsQueue );  nodes[ newNode -> getId() ] = newNode; break;
			case RET:	newNode = Node::createRETNode( *(*it).second, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
			case IF:	newNode = Node::createIFNode( *(*it).second, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
			case WHILE:	newNode = Node::createWHILENode( *(*it).second, connectionsQueue ); 	nodes[ newNode -> getId() ] = newNode; break;
			default: break;
		}

		//newNode -> codeGen() -> dump();

		it++;
	}

	//build AST :)
	for( conIt = connectionsQueue.begin(); conIt != connectionsQueue.end(); conIt++ ){
		if(DEBUG) printf("%d %d\n", ( *conIt ).first, ( *conIt ).second);
		
		if( nodes.find( ( *conIt ).first ) != nodes.end() && nodes.find( ( *conIt ).second ) != nodes.end() ){
			nodes[( *conIt ).first] -> addChild( nodes[( *conIt ).second] ); // always add to the right
		} else {
			printf("ERROR: %d %d\n", ( *conIt ).first, ( *conIt ).second);
			//ERROR: missing node?!
		}
	}
	
	//if(DEBUG) printf("root #%d\n", root -> getId());

	return nodes;
}

Node * Node::createOPNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections ){
	Node * node = new OPNode( simpleNode );

	std::size_t pos;

	int childId;
	std::string childIdRaw;
	string data = node -> getData(); 

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		childIdRaw = data.substr( 0, pos );

		childId = atoi( childIdRaw.c_str() );

		connections.push_back(make_pair(node -> getId(), childId));

		if(data.length() > pos) data = data.substr( pos + 1 );
	}

	return node;
}

Node * Node::createCONSTNode( SimpleNode& simpleNode, std::list<std::pair<int, int> >& connections){
	Node * node = new CONSTNode( simpleNode );

	std::size_t pos;

	string type;
	
	std::string dataChunkRaw;
	
	string data = node -> getData(); 

	int i = 0;

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		dataChunkRaw = data.substr( 0, pos );

		if( i == 0 ){
			if( dataChunkRaw.compare( "STRING" ) == 0 ){
				node -> setVarType( STRING );
			} else if( dataChunkRaw.compare( "NUMBER" ) == 0 ){
				node -> setVarType( NUMBER );
			} else if( dataChunkRaw.compare( "NUMBER" ) == 0 ){
				node -> setVarType( LETTER );
			}
		} else if( i == 1 ){ 
			if( node -> getVarType() == NUMBER ){
				node -> setValueNumber( atoi( dataChunkRaw.c_str() ) );
			} else {
				node -> setValueString( dataChunkRaw );
			}
		}

		if(data.length() > pos) data = data.substr( pos + 1 );
	
		i++;
	}

	if(DEBUG) printf( "Create CONSTNode id: %d type: %d\n", node -> getId(), node -> getVarType() );

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

	std::size_t pos;

	string dataChunkRaw;
	
	string data = node -> getData(); 

	int i = 0;

	string varType;
	string varId;
	string varValue;

	VarType vt;

	Node * tn;

	while( data.length() > 0 && std::string::npos != ( pos = data.find( "," ) ) ){
		dataChunkRaw = data.substr( 0, pos );

		if( i == 0 ){
			varType = dataChunkRaw;
		} else if( i == 1 ){ 
			varId = dataChunkRaw;
		} else if( i == 2 ){
			varValue = dataChunkRaw;
			if(DEBUG) printf( "VARNode %s\n", varValue.c_str() );

		} 

		if(data.length() > pos) data = data.substr( pos + 1 );
	
		i++;
	}

	node -> setVarId( varId );
	node -> setVarType( mapOfTypes[varId] );

	if( varValue.length() > 0 ){ // set value of a variable
		connections.push_back( make_pair( node -> getId(), atoi( varValue.c_str() ) ) );
		node -> set = 1;
	} else { // get value of a variable
		node -> set = 2;
	}

	if(DEBUG) printf( "Create VARNode id: %d %s type: %d\n", node -> getId(), node -> getVarId().c_str(), node -> getVarType() );

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
	generated = true;

	if(DEBUG) printf("VARNode::codeGen %d\n", uniqueId);
	Value * V;
 		
	if( set == 1 ){	
		alloca = env.get( getVarId() ) -> alloca;
		lhs = children[0] -> codeGen( Builder, env );
		V = Builder.CreateStore( lhs, alloca );
		//lhs -> dump();
		set = 2;
		env.add( getVarId(), this );

		if(DEBUG) printf("VAR Value stored:\n");
		lhs -> dump();
	}
	
	alloca = env.get( getVarId() ) -> alloca;
	//lhs = children[0] -> codeGen( Builder, env );
	V = Builder.CreateLoad( alloca );
	//V -> dump();
	if(DEBUG) printf("VAR Value loaded\n");
	return V;
}

//CONSTNode
CONSTNode::CONSTNode( SimpleNode& s) : Node( s ){
	
}

Value* CONSTNode::codeGen(IRBuilder<> & Builder, Environment<Node>& env){
	generated = true;
	if(DEBUG) printf("CONSTNode::codeGen %d\n", uniqueId );
	switch( this -> varType ){
		case STRING: return CONSTNode::codeGenSTRING( *this ); break;
		case NUMBER: return CONSTNode::codeGenNUMBER( *this ); break;
		case LETTER: return CONSTNode::codeGenLETTER( *this ); break;
		case ARRAY: return CONSTNode::codeGenARRAY( *this ); break;
	}
}

Value* CONSTNode::codeGenLETTER( CONSTNode& n ){
	if(DEBUG) printf("CONSTNode::codeGenLETTER CG\n");
	out += n.getValueString();
	//printf("STRING: %s\n", n.getValueString().c_str());
	return ConstantArray::get( getGlobalContext(), n.getValueString(), true);
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
	if(DEBUG) printf("TYPENode::codeGen %d\n", uniqueId);

	if( !allocated ){
		Function *TheFunction = Builder.GetInsertBlock() -> getParent();
		
		IRBuilder<> TmpB( &TheFunction -> getEntryBlock(), TheFunction -> getEntryBlock().begin() );
		
		AllocaInst * Alloca = TmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0);

		alloca = Alloca;	

		if(DEBUG) printf("Memory allocated\n");

		env.add( getVarId(), this);

		allocated = true;

		return ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), 0 );
	} else if( env.get( getVarId() ) != this ) {
		return env.get( getVarId() ) -> codeGen( Builder, env );
	} else {
		return ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), 0 );
	}
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
		case UNR: return OPNode::codeGenUNR( Builder, *this ); break;
		case NEG: return OPNode::codeGenNEG( Builder, *this ); break;
		case S: return OPNode::codeGenS( Builder, *this ); break;
		case G: return OPNode::codeGenG( Builder, *this ); break;
		case E: return OPNode::codeGenE( Builder, *this ); break;
		case GOE: return OPNode::codeGenGOE( Builder, *this ); break;
		case SOE: return OPNode::codeGenSOE( Builder, *this ); break;
	}
}

Value* OPNode::codeGenADD( llvm::IRBuilder<> & Builder, OPNode & n ){
	
	if( n.children[0] -> getType() == CONST && n.children[1] -> getType() == CONST ) {
		int o = n.children[0] -> getValueNumber() + n.children[1] -> getValueNumber();
		bool overflow = o < n.children[0] -> getValueNumber() && o < n.children[1] -> getValueNumber();
		if( overflow ){
			cout << "This value is too big!";
			exit( 1 );
		}
	}

	return Builder.CreateAdd( n.lhs, n.rhs );
}

Value* OPNode::codeGenOR( llvm::IRBuilder<> & Builder, OPNode & n ){
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

Value* OPNode::codeGenUNR( llvm::IRBuilder<> & Builder, OPNode & n ){
	//need to think about it;
	return 0;
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

Value* OPNode::codeGenGOE( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpUGE( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}

Value* OPNode::codeGenSOE( IRBuilder<> & Builder, OPNode & n ){
	Value * b = Builder.CreateICmpULE( n.lhs, n.rhs );
	return Builder.CreateIntCast( b, Type::getInt32Ty(getGlobalContext()), false );
}
