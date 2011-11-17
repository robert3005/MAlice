#ifndef GENERATOR_H_GUARD
#define GENERATOR_H_GUARD

#include <string>
#include <map>

#include "code_ast.hpp"

class CodeGenerator{
	public:
			std::map<int, SimpleNode* > parse( std::string );
			
};

#endif