#ifndef GENERATOR_H_GUARD
#define GENERATOR_H_GUARD

#include <string>
#include <map>

#include "code_ast.hpp"

llvm::Function * makeLLVMModule( Node & );

class CodeGenerator{
	public:
			std::map<int, SimpleNode* > parse( std::string );
			
};

#define DEBUG false

using namespace llvm;
using namespace std;
/**
 * Converts string representation on a node to SimpleNode object
 */
map<int, SimpleNode* > CodeGenerator::parse( string rawData ){
	map<int, SimpleNode* > mapOfNodes;

	size_t pos;

	string node;
	string nodeIdNumber;
	string nodeType;
	string opType;
	string args;

	size_t fieldPos;

	int i = 0;

	while( rawData.length() > 1 && string::npos != ( pos = rawData.find( "|" ) )){
		node = rawData.substr( 0, pos );
	
		fieldPos = node.find( "#" );
		nodeIdNumber = node.substr( 0, fieldPos );
		node = node.substr( fieldPos + 1 );
		
		fieldPos = node.find( "#" );
		nodeType = node.substr( 0, fieldPos );
		node = node.substr( fieldPos + 1 );

		fieldPos = node.find( "#" );
		opType = node.substr( 0, fieldPos );
		node = node.substr( fieldPos + 1 );

		args = node;
		
		mapOfNodes[atoi( nodeIdNumber.c_str() )] = new SimpleNode( nodeIdNumber, nodeType, opType, args );
		
		rawData = rawData.substr( pos + 1 );
		i++;
	}

	return mapOfNodes;
}

Function * makeLLVMModule( map< int, Node * > & ast ){
	theModule = new Module( "alice", getGlobalContext() );

	FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
	Main = Function::Create(FT, Function::ExternalLinkage, "main", theModule);
	BB = BasicBlock::Create( getGlobalContext(), "EntryBlock", Main );
	Builder.SetInsertPoint( BB );

	Environment<Node> * env = new Environment<Node>();
	//env -> add("main", Main);

	for(std::vector< Node * >::iterator it = ast.children.begin(); it != ast.children.end(); ++it) {
	    ( *it ) -> debug();
		( *it ) -> codeGen( Builder, *env );
   	}  	  	
	
	/*
  	
	vector<const Type *> args;
	args.push_back(Type::getInt32Ty( getGlobalContext() ));
  	FunctionType *ft = FunctionType::get(Type::getInt32Ty(getGlobalContext()), args, false); 
	Function *g = Function::Create(ft, Function::ExternalLinkage, Twine("exit"), theModule);
	
  	CallInst * ce = Builder.CreateCall( g, root );*/

  	    //________________________________________________________________

    ArrayType* ArrayTy_0 = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 27);
    ArrayType* ArrayTy_2 = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 4);

    PointerType* PointerTy_5 = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	std::vector<const Type*>FuncTy_12_args;
	 FuncTy_12_args.push_back(PointerTy_5);
	 FunctionType* FuncTy_12 = FunctionType::get(
	  /*Result=*/IntegerType::get(getGlobalContext(), 32),
	  /*Params=*/FuncTy_12_args,
	  /*isVarArg=*/true);

     Function* func_printf = Function::Create(
  	/*Type=*/FuncTy_12,
  	/*Linkage=*/GlobalValue::ExternalLinkage,
  	/*Name=*/"printf", theModule); // (external, no body)
 	func_printf->setCallingConv(CallingConv::C);
 	AttrListPtr func_printf_PAL;
 	func_printf->setAttributes(func_printf_PAL);

 	GlobalVariable* gvar_array__arg = new GlobalVariable(/*Module=*/*theModule, 
	 /*Type=*/ArrayTy_0,
	 /*isConstant=*/true,
	 /*Linkage=*/GlobalValue::InternalLinkage,
	 /*Initializer=*/0, // has initializer, specified below
	 /*Name=*/".arg");
	 gvar_array__arg->setAlignment(1);
	 
	 GlobalVariable* gvar_array__str = new GlobalVariable(/*Module=*/*theModule, 
	 /*Type=*/ArrayTy_2,
	 /*isConstant=*/true,
	 /*Linkage=*/GlobalValue::PrivateLinkage,
	 /*Initializer=*/0, // has initializer, specified below
	 /*Name=*/".str");


	 	// Constant Definitions
	 Constant* const_array_11 = ConstantArray::get(getGlobalContext(), "jestem zajebistym printfem", true);
	 Constant* const_array_12 = ConstantArray::get(getGlobalContext(), "%s\x0A", true);
	 ConstantInt* const_int32_13 = ConstantInt::get(getGlobalContext(), APInt(32, StringRef("1"), 10));
	 ConstantInt* const_int32_14 = ConstantInt::get(getGlobalContext(), APInt(32, StringRef("0"), 10));
	 std::vector<Constant*> const_ptr_15_indices;
	 const_ptr_15_indices.push_back(const_int32_14);
	 const_ptr_15_indices.push_back(const_int32_14);
	 Constant* const_ptr_15 = ConstantExpr::getGetElementPtr(gvar_array__arg, &const_ptr_15_indices[0], const_ptr_15_indices.size());
	 ConstantInt* const_int64_16 = ConstantInt::get(getGlobalContext(), APInt(64, StringRef("5"), 10));
	 ConstantInt* const_int1_17 = ConstantInt::get(getGlobalContext(), APInt(1, StringRef("0"), 10));
	 std::vector<Constant*> const_ptr_18_indices;
	 const_ptr_18_indices.push_back(const_int32_14);
	 const_ptr_18_indices.push_back(const_int32_14);
	 Constant* const_ptr_18 = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_18_indices[0], const_ptr_18_indices.size());
	 
	 // Global Variable Definitions
	 gvar_array__arg->setInitializer(const_array_11);
	 gvar_array__str->setInitializer(const_array_12);

     std::vector<Value*> int32_25_params;
  	int32_25_params.push_back(const_ptr_18);
  	int32_25_params.push_back(const_ptr_15);

	//CallInst* int32_25 = CallInst::Create(func_printf, int32_25_params.begin(), int32_25_params.end(), "", BB);
	//int32_25->setCallingConv(CallingConv::C);
	//int32_25->setTailCall(false);

	if( Builder.GetInsertBlock() -> getTerminator() == 0 ){
		Builder.CreateRet( ConstantInt::get( Type::getInt32Ty( getGlobalContext() ), 0 ));
	}
    //________________________________________________________________

  	//theModule -> dump();
  	return Main;
}


#endif