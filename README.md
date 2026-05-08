# Lox Interpreter

Implementation of the **Lox programming language** in C++

*Crafting Interpreters*: https://craftinginterpreters.com/

## Build and Run

To compile and run the interpreter:

```bash
cmake -S . -B build
cmake --build build
```

To run a script
```bash
./build/Lox [script]
```

To run in interactive mode
```bash
./build/Lox
```

### AST Builder
To generate the Expr.h file build and run
```bash
./build/AST_Gen
```