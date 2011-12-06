#ifndef ENV_H_GUARD
#define ENV_H_GUARD

#include <map>
#include <string>

template <class T>
class Environment{
	public:
		Environment();
		Environment( Environment<T> * );

		void addScope( std::string );
		Environment<T> * getScope( std::string );

		void add( std::string, T * );
		T * get( std::string );

	protected:
		std::map< std::string, Environment<T> * > scopes;
		std::map< std::string, T * > elements;
		Environment<T> * parent;
};

#endif