g++ -g code_generator.cpp code_ast.cpp UnitTest++/libUnitTest++.a -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` -O2 -o cg