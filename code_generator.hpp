#ifndef GENERATOR_H_GUARD
#define GENERATOR_H_GUARD

#include <string>
#include <map>

#include "code_ast.hpp"

llvm::Module * makeLLVMModule( Node & );

#endif