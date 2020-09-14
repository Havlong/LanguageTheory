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
#include <set>

class Parser {
private:
    static const int LETTER = 327555;
    static const int DIGIT = 327555;
    static const int PROGRAM = 327556;
    static const int VAR_DECLARATION = 327557;
    static const int OPERATOR_DECLARATION = 327558;
    static const int VAR_LIST = 327559;
    static const int ASSIGNMENT_LIST = 327560;
    static const int ASSIGNMENT = 327561;
    static const int EXPRESSION = 327562;
    static const int SUBEXPRESSION = 327563;
    static const int UNARY_OPERATOR = 327564;
    static const int BINARY_OPERATOR = 327565;
    static const int OPERAND = 327566;
    static const int CONST = 327567;
    static const int VAR = 327568;

    std::stack<int> stack;

    std::map<int, std::vector<std::vector<int>>> rules;
    std::map<char, std::set<char>> first;
    std::map<char, std::map<char, int>> next;

    static std::vector<char> mapper;

    void initializeGrammar();

public:

    std::vector<int> parseLine(std::string line);

    std::vector<int> parseProgram(std::string program);


    Parser();

};


#endif //LANGUAGETHEORY_PARSER_H
