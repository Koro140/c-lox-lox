#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

static std::string trim(const std::string& str) {
    int start = str.find_first_not_of(" \t\n\r");
    int end = str.find_last_not_of(" \t\n\r");

    return (start == std::string::npos) ? "" : str.substr(start , end - start + 1);
}

void defineType(std::ofstream& file, const std::string& baseName, const std::string& className, const std::string& fields) {
    file << "class " << className << " : public " << baseName << " {\n";
    file << "public:\n";
    
    // Fields
    std::stringstream ss(fields);
    std::string field;

    while (std::getline(ss, field, ',')) {
        file << '\t' << trim(field) << ";\n";
    }
    
    file << "public:\n";

    // Constructor
    file << '\t' << className << '(' << (fields) << ") {\n";
    ss.clear();
    ss.str(fields);
    std::string name, type;
    while (std::getline(ss, field, ',')) {
        std::stringstream ssss(field);
        ssss >> type >> name;
        file << "\t\t" << "this->" << name << " = " << name << ";\n";
    }
    file << "\t}\n";
    file << "\tvoid accept(" << baseName << "Visitor& v) override {\n";
	file << "\t\tv.visit(*this);\n";

	file << "\t}\n;";
    file << "};\n\n";
}

static void defineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string>& types)
{
    std::string path_H = outputDir + "/" + baseName + ".h";

    std::ofstream file_H{path_H};

    file_H << "#pragma once\n\n";
    file_H << "#include <vector>\n";
    file_H << "#include <string>\n\n";
    file_H << "#include <any>\n\n";

    file_H << "#include \"Token.h\"\n\n";

    // forward declaration
    for (auto& type : types) {
        std::stringstream ss(type);
        std::string className;
        std::getline(ss, className, ':');
        file_H << "class " << trim(className) << ";\n";
    }

    file_H << "\n";
    file_H << "class "<< baseName << "Visitor {\n";
    file_H << "public:\n";
    
    for (auto& type : types) {
        std::stringstream ss(type);
        std::string className;
        std::getline(ss, className, ':');
        file_H << "\tvirtual void visit(" << trim(className) << "& v) = 0;\n";
    }
    file_H << "};\n";


    // Base Class
    file_H << "\n";
    file_H << "class " + baseName + " {\n";
    file_H << "public:\n";
    file_H << "\tvirtual void accept("  << baseName << "Visitor& v) = 0;\n";
    file_H << "\t~" << baseName << "() = default;\n";
    file_H << "};\n\n";
    
    // Types
    for (auto type : types)
    {
        std::stringstream ss(type);
        std::string className, fields;
        
        std::getline(ss, className, ':');
        std::getline(ss, fields);
        defineType(file_H, baseName, trim(className), trim(fields));
    }

    file_H.close();
}

int main() {
    defineAst("../src", "Expr", 
    {
        "Grouping : Expr* expr",
        "Assign : Token* name, Expr* value",
        "Binary : Expr* left, Token* op, Expr* right",
        "Logical : Expr* left, Token* op, Expr* right",
        "Unary : Token* op, Expr* right",
        "Literal : std::any value",
        "Variable : Token* name"
    });

    defineAst("../src", "Stmt", 
    {
        "Expression : Expr* expr",
        "Print : Expr* expr",
        "Block : std::vector<Stmt*> statements",
        "If: Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
        "Var : Token* name, Expr* right"
    });
}