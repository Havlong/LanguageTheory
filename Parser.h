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
    inline static const int PROGRAM = 327554;
    inline static const int LETTER = 327555;
    inline static const int DIGIT = 327556;
    inline static const int VAR_DECLARATION = 327557;
    inline static const int OPERATOR_DECLARATION = 327558;
    inline static const int VAR_LIST = 327559;
    inline static const int ASSIGNMENT_LIST = 327560;
    inline static const int ASSIGNMENT = 327561;
    inline static const int EXPRESSION = 327562;
    inline static const int SUBEXPRESSION = 327563;
    inline static const int UNARY_OPERATOR = 327564;
    inline static const int BINARY_OPERATOR = 327565;
    inline static const int OPERAND = 327566;
    inline static const int CONST = 327567;
    inline static const int VAR = 327568;

    std::stack<int> stack;

    std::map<int, std::vector<std::vector<int>>> rules;
    std::map<int, std::set<char>> first;
    std::map<int, std::map<char, int>> next;

    void initializeGrammar();

    void initializeFirstFunctionSets();

    void initializeFirstForNonTerminal(int code, std::set<char> &terminals);

    void initializeNextFunctionMapper();

public:

    std::vector<int> parseLine(const std::string &line);

    std::vector<int> parseProgram(const std::string &program);


    Parser();

};


#endif //LANGUAGETHEORY_PARSER_H
