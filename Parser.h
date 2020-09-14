/**
 * 14.09.2020
 * Parser
 *
 * @author Havlong
 * @version v1.0
 */

#ifndef LANGUAGETHEORY_PARSER_H
#define LANGUAGETHEORY_PARSER_H


#include <stack>
#include <map>
#include <vector>

class Parser {
private:
    std::stack<int> stack;

    std::map<int, std::vector<int>> rules;
    std::map<char, std::map<char, int>> next;

    static std::vector<char> mapper;

    void initializeGrammar() ;

public:

    std::vector<int> parseLine(std::string line) ;
    std::vector<int> parseProgram(std::string program) ;


    Parser();

};


#endif //LANGUAGETHEORY_PARSER_H
