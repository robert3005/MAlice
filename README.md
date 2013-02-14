# MAlice
## Alice Compiler

* Robert Kruszewski
* Łukasz Koprowski
* Piotr Bar

## Language
This compiler is designed for language called Alice, sample below. Given Alice source code it compiles it into suitable binary for operating system we are running on (all architectures supported by LLVM 2.8 should be usable)

### Sample
    i was a number but what was i? i became fib(i).
    i spoke and "\n" said Alice.

    The room fib(number n) contained a number
      either (n <= 1) so
        Alice found n.
      or
        Alice found (fib(n-1) + fib(n-2)).
      Alice was unsure which.

## Code Organisation
In order to use compile simply call ./compile $FILE. It invokes compileLLVM.coffee which does lexing, parsing, semantic analysis then passes build up AST to libLLVMGen.so library. This library is responsible for code generation, i.e. optimisation and produces LLVM assembly which is then compiled by LLVM assembler.

## Dependencies:
* LLVM (2.8)
* Node.js
* CoffeeScript
* PEG.js
* node-ffi
* jessie (for tests)
