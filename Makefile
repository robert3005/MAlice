all:
	npm install coffee-script
	npm install node-ffi
	npm install pegjs
	g++ -shared -Wl,-soname,libllvmGen.so -o libllvmGen.so llvmGen.o code_generator.o code_ast.o -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -lc
	g++ -fPIC -g -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -c -Wl,export-dynamic -Wall libllvmGen.so

clean:
	rm -f cg
