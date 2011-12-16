all:
	npm install coffee-script
	npm install node-ffi
	npm install pegjs
	g++ -g code_generator.cpp code_ast.cpp -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include -I/usr/lib/llvm/lib -I/usr/lib/llvm/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` `/usr/lib/llvm/bin/llvm-config --cppflags --ldflags --libs core native` -o cg

clean:
	rm -f cg