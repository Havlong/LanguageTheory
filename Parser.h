#ifndef LANGUAGETHEORY_PARSER_H
#define LANGUAGETHEORY_PARSER_H


#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include "SyntaxTreeBranch.h"
#include "SyntaxTreeLeaf.h"

/**
 * 14.09.2020
 * Parser
 *
 * @author Havlong
 * @version v1.0
 */
class Parser {
private:
    std::stack<int> syntaxStack;

    std::set<char> usedTerminals;
    std::map<int, std::vector<std::vector<int>>> rules;
    std::map<int, std::set<char>> first;
    std::map<int, std::set<char>> next;
    std::map<int, std::map<char, int>> parsingTable;

    void initializeGrammar();

    void initializeFirstFunctionSets();

    void initializeFirstForNonTerminal(int nonTerminal);

    void initializeNextFunctionSets();

    void initializeNextWithTerminal(char terminal);

    void initializeNextEndedWithNonTerminal(int nonTerminal, char terminal);

    void copySetsFromNextNonTerminals();

    void constructParsingTable();

public:

    SyntaxTreeBranch *parseLine(const std::string &line, SyntaxTreeBranch *branch);

    SyntaxTreeBranch parseProgram(const std::string &program);


    Parser();

};


#endif //LANGUAGETHEORY_PARSER_H
