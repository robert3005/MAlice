~/node_modules/coffee-script/bin/coffee parse.coffee ./milestone2/$1 >alice.bitch.in     
g++ -g code_generator.cpp code_ast.cpp -I/usr/lib/llvm-2.8/lib -I/usr/lib/llvm-2.8/include -I/usr/lib/llvm/lib -I/usr/lib/llvm/include `/usr/lib/llvm-2.8/bin/llvm-config --cppflags --ldflags --libs core native` `/usr/lib/llvm/bin/llvm-config --cppflags --ldflags --libs core native` -o cg
./cg <alice.bitch.in >alice.ll
/usr/lib/llvm-2.8/bin/llc alice.ll
gcc alice.s -o alice
./alice
echo $?
