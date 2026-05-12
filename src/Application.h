#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Token.h"
#include "Scanner.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Parser.h"
#include "Interpreter.h"

class Application
{
private:
    bool hadError = false;  
    bool hadRuntimeError = false;
public:

    Application(int argc, char** argv);
    static Application* getInstance();

    void runFile(char* filePath);

    void runLoop();
    void run(std::string source);

    static void error(int line, const std::string& message);
    
    static void error(Token* token, std::string message);

    static void runtimeError(Interpreter::RuntimeError error);
    static void report(int line, const std::string& where, const std::string& message);
};