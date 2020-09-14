/**
 * 14.09.2020
 * Parser
 *
 * @author Havlong
 * @version v1.0
 */

#include "Parser.h"

Parser::Parser() {
    initializeGrammar();
    initializeFirstFunctionSets();
    initializeNextFunctionMapper();
}

std::vector<int> Parser::parseLine(const std::string &line) {
    // TODO: Implement syntax parser
    return std::vector<int>();
}

std::vector<int> Parser::parseProgram(const std::string &program) {
    // TODO: Implement line splitter and parseLine() usage
    // TODO: Treat space symbols as separators with non-terminals
    return std::vector<int>();
}

void Parser::initializeGrammar() {
    rules[PROGRAM].push_back({VAR_DECLARATION, OPERATOR_DECLARATION});
    rules[OPERATOR_DECLARATION].push_back({'B', 'E', 'G', 'I', 'N', OPERATOR_LIST, 'E', 'N', 'D'});
    rules[VAR_DECLARATION].push_back({'V', 'A', 'R', VAR_LIST, ':', 'I', 'N', 'T', 'E', 'G', 'E', 'R', ';'});
    rules[VAR_LIST].push_back({VAR});
    rules[VAR_LIST].push_back({VAR, ',', VAR_LIST});
    rules[OPERATOR_LIST].push_back({OPERATOR});
    rules[OPERATOR_LIST].push_back({OPERATOR, OPERATOR_LIST});
    rules[OPERATOR].push_back({VAR, '=', EXPRESSION, ';'});
    rules[OPERATOR].push_back({'R', 'E', 'A', 'D', '(', OPERATOR_LIST, ')', ';'});
    rules[OPERATOR].push_back({'W', 'R', 'I', 'T', 'E', '(', OPERATOR_LIST, ')', ';'});
    rules[OPERATOR].push_back(
            {'F', 'O', 'R', VAR, '=', EXPRESSION, 'T', 'O', EXPRESSION, 'D', 'O',
             OPERATOR_LIST,
             'E', 'N', 'D', '_', 'F', 'O', 'R', ';'});
    rules[EXPRESSION].push_back({UNARY_OPERATOR, SUBEXPRESSION});
    rules[EXPRESSION].push_back({SUBEXPRESSION});
    rules[SUBEXPRESSION].push_back({'(', EXPRESSION, ')'});
    rules[SUBEXPRESSION].push_back({OPERAND});
    rules[SUBEXPRESSION].push_back({SUBEXPRESSION, BINARY_OPERATOR, SUBEXPRESSION});
    rules[UNARY_OPERATOR].push_back({'-'});
    rules[BINARY_OPERATOR].push_back({'+'});
    rules[BINARY_OPERATOR].push_back({'-'});
    rules[BINARY_OPERATOR].push_back({'*'});
    rules[OPERAND].push_back({VAR});
    rules[OPERAND].push_back({CONST});
    for (int i = 11; i > 0; --i) {
        rules[VAR + i].push_back({LETTER, VAR + i - 1});
        rules[VAR + i].push_back({LETTER});
    }
    rules[VAR].push_back({LETTER});
    rules[CONST].push_back({DIGIT});
    rules[CONST].push_back({DIGIT, CONST});
    for (int i = 0; i < 10; ++i) {
        rules[DIGIT].push_back({'0' + i});
    }
    for (int i = 0; i < 26; ++i) {
        rules[LETTER].push_back({'a' + i});
    }
}

void Parser::initializeFirstFunctionSets() {
    for (const auto&[nonTerminal, rule]: rules) {
        if (first[nonTerminal].empty()) {
            initializeFirstForNonTerminal(nonTerminal);
        }
    }
}

void Parser::initializeFirstForNonTerminal(int nonTerminal) {
    if (first[nonTerminal].empty()) {
        for (const auto &rule : rules[nonTerminal]) {
            if (rule.front() >= PROGRAM) {
                if (rule.front() != nonTerminal) {
                    initializeFirstForNonTerminal(rule.front());
                }
                for (const auto &terminal : first[rule.front()]) {
                    first[nonTerminal].insert(terminal);
                }
            } else {
                first[nonTerminal].insert(rule.front());
            }
        }
    }
}

void Parser::initializeNextFunctionMapper() {
    // TODO: Initialize sets for NEXT function
}
