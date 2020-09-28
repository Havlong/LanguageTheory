#include "Parser.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    Parser myParser;
    if (argc > 1) {
        // TODO: implement entire program interpretation
        std::string filename = (std::string) argv[1];
        std::cout << "File " << filename << " compilation" << std::endl;
        std::ifstream fin(filename);
        std::string program((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
        SyntaxTreeBranch programTree = myParser.parseProgram(program);
        std::cout << "[I]: Tree is ready\n";
        std::cout << "[I]: Applying interpreter\n";
    } else {
        // TODO: start live interpreter
        std::cout << "[I]: Live interpreter started\n";
    }
    return 0;
}
