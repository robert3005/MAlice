#include <cstdio>
#include <string>
#include "environment.hpp"

using namespace std;

template <class T> 
Environment<T>::Environment(){
	parent = 0;
}

template <class T> 
Environment<T>::Environment( Environment<T> * p ){
	parent = p;
}

template <class T> 
Environment<T> * Environment<T>::getScope( string key ){
	typename map<string, Environment<T> * >::iterator it;
	it = scopes.find( key );

	if( it != scopes.end() ){
		return *it;
	} else {
		return 0;
	}	
}

template <class T> 
void Environment<T>::addScope( string key ){
	typename map<string, Environment<T> * >::iterator it;
	it = scopes.find( key );

	if( it != elements.end() ){
		scopes[key] = new Environment<T>( this );
	} else {
		scopes[key] = new Environment<T>( this );
	}
}

template <class T> 
void Environment<T>::add( string key, T * n ){
	typename map<string, T* >::iterator it;
	it = elements.find( key );

	if( it != elements.end() ){
		elements[key] = n;
	} else {
		elements[key] = n;
	}
}

template <class T> 
T * Environment<T>::get( string key ){
	typename map<string, T* >::iterator it;
	it = elements.find( key );

	if( it != elements.end() ){
		return (*it).second;
	} else if( parent != 0 ) {
		return parent -> get( key );
	} else {
		return 0;
	}
}
/*
int main(){
	Environment<string> * env = new Environment<string>();

	string s = "kot";

	env -> add( string("pies"), &s );

	printf("%s\n", env -> get( string("pies") ) -> c_str() );

	return 0;
}
*/