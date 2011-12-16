#include <cstdio>
#include <string>

using namespace std;

template <class T>
class Environment{
	public:
		Environment(){
			parent = 0;
		}

		Environment( Environment<T> * p ){
			parent = p;
		}

		Environment<T> * getScope( std::string key ){
			typename std::map<std::string, Environment<T> * >::iterator it;
			it = scopes.find( key );

			if( it != scopes.end() ){
				return (*it).second;
			} else {
				return 0;
			}	
		}

		void addScope( std::string key ){
			scopes[key] = new Environment<T>( this );
		}

		void add( std::string key, T * n ){
			typename std::map<std::string, T* >::iterator it;
			it = elements.find( key );

			if( it != elements.end() ){
				elements[key] = n;
			} else {
				elements[key] = n;
			}
		}
		 
		T * get( std::string key ){
			typename std::map<std::string, T* >::iterator it;
			it = elements.find( key );

			if( it != elements.end() ){
				return (*it).second;
			} else if( parent != 0 ) {

				return parent -> get( key );
			} else {
				return 0;
			}
		}

		bool is( std::string key ){
			return (elements.find( key ) != elements.end());
		}

	protected:
		std::map< std::string, Environment<T> * > scopes;
		std::map< std::string, T * > elements;
		Environment<T> * parent;
};