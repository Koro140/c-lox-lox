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

    void runFile(char* filePath) {
        std::string path = SCRIPT_PATH;
        path.append(filePath);

        std::ifstream file(path);
        if (!file) {
            std::cerr << "Couldn't open file " << filePath << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        
        run(content);
        if (hadError){
            exit(65);
        }
        if (hadRuntimeError) {
            exit(75);
        }
    }

    void runLoop() {
        for (;;)
        {
            std::cout << ">> ";
            std::string inputStr;
            std::getline(std::cin, inputStr);
            if (inputStr.empty())
            {
                break;
            }

            run(inputStr);
        }
    }

    void run(std::string source) {
        Scanner scanner{source};
        std::vector<Token> tokens = scanner.scanTokens();
        Parser parser = Parser(tokens);
        std::vector<Stmt*> statements = parser.parse();

        if (hadError)
        {
            return;
        }
        
        //AstPrinter printer;
        //std::cout << printer.print(expression);

        Interpreter i;
        i.interpret(statements);
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
    }
    
    static void error(Token* token, std::string message) {
        if (token->getType() == TOK_EOF) {
            report(token->getLine(), " at end", message);
        } else {
            report(token->getLine(), " at '" + token->getLexeme() + "'", message);
        }
    }

    static void runtimeError(Interpreter::RuntimeError error) {
        std::cout << error.what() << "\n[line " << error.getToken()->getLine() << "]";
        getInstance()->hadRuntimeError = true;
    }

    static void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[line " << line << "] Error" + where + ": " + message;
    }
};