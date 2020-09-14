#include "Parser.h"
#include <iostream>

int main(int argc, char **argv) {
    Parser myParser;
    if (argc > 1) {
        // TODO: implement entire program interpretation
        std::cout << "File" << (std::string) argv[1] << " compilation" << std::endl;
    } else {
        // TODO: start live interpreter
        std::cout << "Live interpreter started" << std::endl;
    }
    return 0;
}
