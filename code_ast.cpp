#include "code_ast.hpp"

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