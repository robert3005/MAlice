#include <cstdio>
#include <iostream>
#include <vector>

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
/*
g++ -g test.cpp code_ast.cpp  UnitTest++/libUnitTest++.a `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native jit` -o test

*/
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Instructions.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/PassManager.h"
#include "llvm/CallingConv.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/System/DynamicLibrary.h"


#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

#include "llvm/Support/MemoryBuffer.h"

using namespace std;
using namespace llvm;

static llvm::Module * theModule;
static llvm::IRBuilder<> Builder( getGlobalContext() );
static llvm::Function * Main;
static llvm::BasicBlock * BB;

int main(int, char const *[])
{
	theModule = new Module( "alice", getGlobalContext() );

	FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
	Main = Function::Create(FT, Function::ExternalLinkage, "main", theModule);
	BB = BasicBlock::Create( getGlobalContext(), "EntryBlock", Main );
	Builder.SetInsertPoint( BB );

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

	AllocaInst* ptr_x = Builder.CreateAlloca(IntegerType::get(getGlobalContext(), 32), 0, "x");
	std::vector<Value*> int32_14_params;
  	int32_14_params.push_back(const_ptr_10);
  	int32_14_params.push_back(ptr_x);

	Builder.CreateCall(func_scanf, int32_14_params.begin(), int32_14_params.end());

	ConstantArray::get( getGlobalContext(), "kotek", true) -> dump();
	ConstantInt::get( Type::getInt8Ty( getGlobalContext() ), 'a' ) -> dump();
	theModule -> dump();
}