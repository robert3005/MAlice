all:
	npm install coffee-script
	npm install node-ffi
	npm install pegjs
	g++ -fPIC -g -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -c -Wl,export-dynamic -Wall code_ast.cpp
	g++ -fPIC -g -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -c -Wl,export-dynamic -Wall code_generator.cpp
	g++ -fPIC -g -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -c -Wl,export-dynamic -Wall environment.cpp
	g++ -fPIC -g -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -c -Wl,export-dynamic -Wall llvmGen.cpp
	g++ -shared -Wl,-soname,libllvmGen.so -o libllvmGen.so llvmGen.o code_generator.o code_ast.o environment.o -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -lc

clean:
	rm -f libllvmGen.so *.o 
