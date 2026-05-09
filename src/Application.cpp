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