#include "Application.h"

Application* s_Application = nullptr;

Application::Application(int argc, char** argv) {
    s_Application = this;
    
    if (argc > 2) {
        std::cerr << "Usage : cpp-lox [script]" << std::endl;
        exit(64);
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runLoop();
    }
}

Application* Application::getInstance() {
    return s_Application;
}

void Application::runFile(char* filePath) {
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

void Application::runLoop() {
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

void Application::run(std::string source) {
    Scanner scanner{source};
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser = Parser(tokens);
    std::vector<Stmt*> statements = parser.parse();
    if (hadError)
    {
        return;
    }
    Interpreter i;
    i.interpret(statements);
}

void Application::error(int line, const std::string& message) {
    report(line, "", message);
}

void Application::error(Token* token, std::string message) {
    if (token->getType() == TOK_EOF) {
        report(token->getLine(), " at end", message);
    } else {
        report(token->getLine(), " at '" + token->getLexeme() + "'", message);
    }
}

void Application::runtimeError(Interpreter::RuntimeError error) {
    std::cout << error.what() << "[line " << error.getToken()->getLine() << "]" << std::endl;
    getInstance()->hadRuntimeError = true;
}

void Application::report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" + where + ": " + message;
}