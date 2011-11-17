#include "code_ast.hpp"

//SimpleNode
SimpleNode::SimpleNode( uniqueId, type, op, args ){
	this -> uniqueId = atoi( uniqueId.c_str() );

	if( type.compare( "OP" ) == 0 ){
		this -> type = OP;
	} else if( type.compare( "VAR" ) == 0 ){
		this -> type = VAR;
	} else if( type.compare( "CONST" ) == 0 ){
		this -> type = CONST;
	} else if( type.compare( "TYPE" ) == 0 ){
		this -> type = TYPE;
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
	} else if( op.compare( "NEG" ) == 0 ){
		this -> op = NEG;
	} 

	this -> data = args;
}

NodeType SimpleNode::getType(){
	return type;
}

//Node
static Node * Node::createAST( sn ){
	std::list<std::pair<int, int> > connectionsQueue;
	std::list<std::pair<int, int> >::iterator conIt;

	std::map<int, SimpleNode*>::iterator it = sn.begin();
	std::map<int, Node*> nodes;
	
	//Generate root of AST
	Node * root;

	switch( (*it).second -> getType() ){
		case OP: 	root = Node::createOPNode( (*it).second, connectionsQueue ); break;
		case VAR: 	root = Node::createVARNode( (*it).second, connectionsQueue ); break;
		case CONST: root = Node::createCONSTNode( (*it).second, connectionsQueue ); break;
		case TYPE:	root = Node::createTYPENode( (*it).second, connectionsQueue );  break;
		case default: break;
	}

	nodes[root -> getId()] = &root;
	it++;

	SimpleNode* processed;
	Node* newNode;

	//Generate children

	while( it != sn.end() ){
		processed = &( (*( ++it )).second );

		switch( processed -> getType() ){
			case OP: 	newNode = Node::createOPNode( (*it).second, connectionsQueue ); break;
			case VAR: 	newNode = Node::createVARNode( (*it).second, connectionsQueue ); break;
			case CONST: newNode = Node::createCONSTNode( (*it).second, connectionsQueue ); break;
			case TYPE:	newNode = Node::createTYPENode( (*it).second, connectionsQueue );  break;
			case default: break;
		}

		nodes[newNode -> getId()] = &newNode;

		it++;
	}


	//build AST :)

	for( conIt = connectionsQueue.begin(); conIt != connectionsQueue.end(); conIt++ ){
		if( nodes.find( ( *conIt ).first ) != nodes.end() && nodes.find( ( *conIt ).second ) != nodes.end() ){
			nodes[( *conIt ).first] -> addChild( nodes[( *conIt ).second] ); // always add to the right
		} else {
			//ERROR: missing node?!
		}
	}

	//free memory
	for( it = sn.begin(); it != sn.end(); it++ ){
		delete ( *it ).second;
	}	

	return root;
}

static Node * Node::createOPNode( simpleNode, connections ){
	Node * node = new OPNode( simpleNode );

	std::size_t pos;
	int last = 0;

	int childId;

	while( std::string::npos != ( pos = ( this -> getData() ).find( "," ) ) ){
		node = ( this -> getData() ).substr( last + 1, ( int( pos ) - last - 1 ) );

		connections.push_back(make_pair(this -> getId(), childId));

		last = int(pos);
	}

	return node;
}

//VARNode
Value* VARNode::codeGen( mod, builder ){
  	return NamedValues[id];
}

//CONSTNode
CONSTNode::CONSTNode(){
	codeGenFunctions[STRING] = &codeGenSTRING;
	codeGenFunctions[NUMBER] = &codeGenNUMBER;
}

Value* CONSTNode::codeGen( mod, builder ){
	return ( *codeGenFunctions[varType] )( mod, builder );
}

Value* OPNode::codeGenSTRING( mod, builder ){
	return ConstantArray::get( mod->getContext(), valueString, true);

Value* OPNode::codeGenNUMBER( mod, builder ){
	return ConstantInt::get( mod->getContext(), APInt( valueInt ) );
}

//OPNode
OPNode::OPNode(){
	codeGenFunctions[ADD] = &codeGenADD;
	codeGenFunctions[OR] = &codeGenOR;
	codeGenFunctions[XOR] = &codeGenXOR;
	codeGenFunctions[AND] = &codeGenAND;
	codeGenFunctions[SUB] = &codeGenSUB;
	codeGenFunctions[MUL] = &codeGenMUL;
	codeGenFunctions[DIV] = &codeGenDIV;
	codeGenFunctions[MOD] = &codeGenMOD;
	codeGenFunctions[UNR] = &codeGenUNR;
	codeGenFunctions[NEG] = &codeGenNEG;
}

Value* OPNode::codeGen( mod, builder ){
	lhs = children[0].codeGen( mod, builder );
	rhs = children[1].codeGen( mod, builder );
	
	return ( *codeGenFunctions[OPType] )( mod, builder );
}

Value* OPNode::codeGenADD( mod, builder ){
	return builder.CreateAdd( lhs, rhs );
}

Value* OPNode::codeGenOR( mod, builder ){
	return builder.CreateOr( lhs, rhs );
}

Value* OPNode::codeGenXOR( mod, builder ){
	return builder.CreateXor( lhs, rhs );
}

Value* OPNode::codeGenAND( mod, builder ){
	return builder.CreateAnd( lhs, rhs );
}

Value* OPNode::codeGenSUB( mod, builder ){
	return builder.CreateSub( lhs, rhs );
}

Value* OPNode::codeGenMUL( mod, builder ){
	return builder.CreateMul( lhs, rhs );
}

Value* OPNode::codeGenDIV( mod, builder ){
	return builder.CreateUDiv( lhs, rhs ); //unsigned 
}

Value* OPNode::codeGenUNR( mod, builder ){
	//need to think about it;
	return null;
}

Value* OPNode::codeGenNEG( mod, builder ){
	return builder.CreateNeg( lhs );
}