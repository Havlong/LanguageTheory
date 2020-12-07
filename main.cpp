#include "Parser.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    Parser myParser;
    Interpreter interpreter;
    if (argc > 1) {
        // TODO: implement entire program interpretation
        std::string filename = (std::string) argv[1];
        std::cout << "[I]: Welcome to BACK interpreter\n";
        std::cout << "[I]: File " << filename << " is being compiled\n";
        std::ifstream fin(filename);
        std::string program((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

        SyntaxTreeNode *programTree = myParser.parseProgram(program);

        if (myParser.isStackEmpty()) {

            std::cout << "[I]: Tree is ready\n";
            std::cout << "[I]: Applying interpreter\n";

            interpreter.executeSubTree(programTree);
        } else {
            std::cout << "[E]: File end met, but program is not finished\n";
        }
    } else {
        std::cout << "[E]: There is no live interpreter with descending compilation\n";
        std::cout << "[E]: Please enter filename as the program argument\n";
    }
    return 0;
}
