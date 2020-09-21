/**
 * 14.09.2020
 * Parser
 *
 * @author Havlong
 * @version v1.0
 */

#ifndef LANGUAGETHEORY_PARSER_H
#define LANGUAGETHEORY_PARSER_H


#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

class Parser {
private:
    inline static const int PROGRAM = 327550;
    inline static const int LETTER = 327551;
    inline static const int DIGIT = 327552;
    inline static const int VAR_DECLARATION = 327553;
    inline static const int OPERATOR_DECLARATION = 327554;
    inline static const int VAR_LIST = 327555;
    inline static const int VAR_LIST_CONTINUATION = 327556;
    inline static const int OPERATOR_LIST = 327557;
    inline static const int OPERATOR = 327558;
    inline static const int EXPRESSION = 327559;
    inline static const int EXPRESSION_LIST = 327560;
    inline static const int EXPRESSION_LIST_CONTINUATION = 327561;
    inline static const int BRACKET_EXPRESSION = 327562;
    inline static const int SUBEXPRESSION = 327563;
    inline static const int UNARY_OPERATOR = 327564;
    inline static const int BINARY_OPERATOR = 327565;
    inline static const int OPERAND = 327566;
    inline static const int CONST = 327567;
    inline static const int VAR = 327568;

    std::stack<int> syntaxStack;

    std::map<int, std::vector<std::vector<int>>> rules;
    std::map<int, std::set<char>> first;
    std::map<int, std::set<char>> next;
    std::map<int, std::map<char, int>> parsingTable;

    void initializeGrammar();

    void initializeFirstFunctionSets();

    void initializeFirstForNonTerminal(int nonTerminal);

    void initializeNextFunctionSets();

    void initializeNextForNonTerminal(int nonTerminal);

    void constructParsingTable();

public:

    std::vector<int> parseLine(const std::string &line);

    std::vector<int> parseProgram(const std::string &program);


    Parser();

};


#endif //LANGUAGETHEORY_PARSER_H
