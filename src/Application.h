#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Token.h"
#include "Scanner.h"

class Application
{
private:
    bool hadError = false;
public:

    Application(int argc, char** argv) {
        if (argc > 2) {
            std::cerr << "Usage : cpp-lox [script]" << std::endl;
            exit(64);
        } else if (argc == 2) {
            runFile(argv[1]);
        } else {
            runLoop();
        }
    }

    ~Application() {

    }
    
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
        if (hadError)
        {
            exit(65);
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
                std::cout << "Goodbye\n";
                break;
            }

            run(inputStr);
        }
    }

    void run(std::string source) {
        Scanner scanner{source};
        std::vector<Token> tokens = scanner.scanTokens();

        for (auto &i : tokens)
        {
            std::cout << i.stringfy() << std::endl;
        }
        
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
    }

    static void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[line " << line << "] Error" + where + ": " + message;
    }
};