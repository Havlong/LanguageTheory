/**
 * 14.09.2020
 * Parser
 *
 * @author Havlong
 * @version v1.0
 */

#include <sstream>
#include "Parser.h"

using namespace StaticProvider;

Parser::Parser() {
    initializeGrammar();
    initializeFirstFunctionSets();
    initializeNextFunctionSets();
    constructParsingTable();
    syntaxStack.push(PROGRAM);
}

[[nodiscard]] SyntaxTreeNode *Parser::parseLine(const std::string &line, SyntaxTreeNode *current) {
    char lastChar;
    for (int i = 0; i < line.size();) {
        if (syntaxStack.empty()) {
            std::cerr << "[E]: Syntax error:\n";
            std::cerr << "Awaited end of program but got " << line[i];
            std::cerr << "[E]: " << line << std::endl;
            exit(-5);
        }
        int symbol = syntaxStack.top();
        syntaxStack.pop();
        if (symbol >= PROGRAM) {
            while (i < line.size() && symbol != LETTER && symbol != DIGIT && std::isspace(line[i])) ++i;

            if (parsingTable.count(symbol)) {
                if (parsingTable[symbol].count(line[i])) {
                    if (symbol == LETTER || symbol == DIGIT) {
                        lastChar = line[i++];
                    } else {
                        if (symbol >= VAR_CONTINUATION) {
                            current->getData().push_back(lastChar);
                        } else if (current->getNonTerminal() != symbol
                                   && symbol < VAR_LIST_CONTINUATION
                                   && symbol != EXPRESSION_LIST
                                   && symbol != OPERAND
                                   && symbol != SUBEXPRESSION) {
                            current = new SyntaxTreeNode(current, symbol);
                            syntaxStack.push(END_OF_NODE);
                            if (dataInfo.count(current->getNonTerminal())) {
                                current->setData(dataInfo[symbol][parsingTable[symbol][line[i]]]);
                            }
                        }
                        const std::vector<int> &rule = rules[symbol][parsingTable[symbol][line[i]]];
                        for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
                            syntaxStack.push(*it);
                        }
                    }
                } else {
                    std::cerr << "[E]: Syntax Error\n";
                    std::cerr << "Awaited ";
                    if (parsingTable[symbol].size() == 1) {
                        if (errorInfo.count(parsingTable[symbol].begin()->first)) {
                            std::cerr << errorInfo[symbol];
                        } else {
                            std::cerr << symbol;
                        }
                    }
                    std::cerr << " but met " << line[i] << std::endl;
                    std::cerr << "[E]: " << line << std::endl;
                    exit(-5);
                }
            } else {
                std::cerr << "[E]: Error, expected nonTerminal, that does not exist in parsing table\n";
                exit(-6);
            }
        } else if (symbol == END_OF_NODE) {
            while (std::isspace(line[i])) ++i;
            current = current->pushLeaf();
        } else {
            if (line[i] == symbol) {
                i++;
            } else {
                std::cerr << "[E]: Syntax Error\n";
                std::cerr << "Awaited ";
                if (errorInfo.count(symbol)) {
                    std::cerr << errorInfo[symbol];
                } else {
                    std::cerr << symbol;
                }
                std::cerr << " but met " << line[i] << std::endl;
                std::cerr << "[E]: " << line << std::endl;
                exit(-5);
            }
        }
    }
    while (!syntaxStack.empty() && syntaxStack.top() == END_OF_NODE) {
        current = current->pushLeaf();
        syntaxStack.pop();
    }
    return current;
}

SyntaxTreeNode *Parser::parseProgram(const std::string &program) {
    auto *programTree = new SyntaxTreeNode(PROGRAM);
    std::stringstream splitter(program);
    while (!splitter.eof()) {
        std::string line;
        getline(splitter, line);
        programTree = parseLine(line, programTree);
    }
    return programTree;
}

void Parser::initializeGrammar() {
    errorInfo['<'] = "start of operator list";
    errorInfo['>'] = "end of operator list";
    errorInfo[':'] = "colon";
    errorInfo[';'] = "semicolon line separator";
    errorInfo['['] = "start of variable list";
    errorInfo[']'] = "end of variable list";
    errorInfo['-'] = "- operator in expression";
    errorInfo['+'] = "binary operator in expression";
    errorInfo['*'] = "binary operator in expression";
    errorInfo['.'] = "range operator in cycle declaration";

    rules[PROGRAM].push_back({VAR_DECLARATION, OPERATOR_DECLARATION});
    rules[OPERATOR_DECLARATION].push_back({'<', '<', OPERATOR_LIST, '>', '>'});
    rules[VAR_DECLARATION].push_back({'V', 'A', 'R', VAR_LIST, ':', TYPE, ';'});

    rules[VAR_LIST].push_back({VAR, VAR_LIST_CONTINUATION});
    rules[VAR_LIST_CONTINUATION].push_back({',', VAR_LIST});
    rules[VAR_LIST_CONTINUATION].push_back({});
    rules[TYPE].push_back({'I', 'N', 'T', 'E', 'G', 'E', 'R'});
    dataInfo[TYPE].push_back("INTEGER");

    rules[OPERATOR_LIST].push_back({OPERATOR, OPERATOR_LIST_CONTINUATION});
    rules[OPERATOR_LIST_CONTINUATION].push_back({OPERATOR_LIST});
    rules[OPERATOR_LIST_CONTINUATION].push_back({});

    rules[OPERATOR].push_back({VAR, '=', EXPRESSION, ';'});
    dataInfo[OPERATOR].push_back("=");
    rules[OPERATOR].push_back({'R', 'E', 'A', 'D', '[', VAR_LIST, ']', ';'});
    dataInfo[OPERATOR].push_back("READ");
    rules[OPERATOR].push_back({'W', 'R', 'I', 'T', 'E', '[', VAR_LIST, ']', ';'});
    dataInfo[OPERATOR].push_back("WRITE");
    rules[OPERATOR].push_back(
            {'F', 'O', 'R', VAR, ':', EXPRESSION, '.', '.', EXPRESSION, OPERATOR_DECLARATION});
    dataInfo[OPERATOR].push_back("FOR");

    rules[EXPRESSION].push_back({UNARY_OPERATOR, EXPRESSION_LIST});
    rules[EXPRESSION].push_back({EXPRESSION_LIST});

    rules[EXPRESSION_LIST].push_back({SUBEXPRESSION, EXPRESSION_LIST_CONTINUATION});
    rules[EXPRESSION_LIST_CONTINUATION].push_back({BINARY_OPERATOR, EXPRESSION_LIST});
    rules[EXPRESSION_LIST_CONTINUATION].push_back({});

    rules[SUBEXPRESSION].push_back({BRACKET_EXPRESSION});
    rules[SUBEXPRESSION].push_back({OPERAND});
    rules[BRACKET_EXPRESSION].push_back({'(', EXPRESSION, ')'});

    rules[UNARY_OPERATOR].push_back({'-'});
    dataInfo[UNARY_OPERATOR].push_back("-");
    rules[BINARY_OPERATOR].push_back({'+'});
    dataInfo[BINARY_OPERATOR].push_back("+");
    rules[BINARY_OPERATOR].push_back({'-'});
    dataInfo[BINARY_OPERATOR].push_back("-");
    rules[BINARY_OPERATOR].push_back({'*'});
    dataInfo[BINARY_OPERATOR].push_back("*");

    rules[OPERAND].push_back({VAR});
    rules[OPERAND].push_back({CONST});

    rules[VAR].push_back({LETTER, VAR_CONTINUATION});
    rules[VAR_CONTINUATION].push_back({VAR});
    rules[VAR_CONTINUATION].push_back({});

    rules[CONST].push_back({DIGIT, CONST_CONTINUATION});
    rules[CONST_CONTINUATION].push_back({CONST});
    rules[CONST_CONTINUATION].push_back({});

    for (int i = 0; i < 10; ++i) {
        rules[DIGIT].push_back({'0' + i});
    }
    for (int i = 0; i < 26; ++i) {
        rules[LETTER].push_back({'a' + i});
    }
}

void Parser::initializeFirstFunctionSets() {
    for (const auto&[nonTerminal, rule]: rules) {
        initializeFirstForNonTerminal(nonTerminal);
    }
}

void Parser::initializeFirstForNonTerminal(int nonTerminal) {
    if (first[nonTerminal].empty()) {
        for (const auto &rule : rules[nonTerminal]) {
            if (!rule.empty()) {
                if (rule.front() >= PROGRAM) {
                    if (rule.front() != nonTerminal) {
                        initializeFirstForNonTerminal(rule.front());
                    }
                    for (const auto &terminal : first[rule.front()]) {
                        if (first[nonTerminal].count(terminal)) {
                            std::cerr << "[E]: Grammar has intersection of FIRST sets for different rules" << std::endl;
                            exit(-nonTerminal);
                        }

                        first[nonTerminal].insert(terminal);
                    }
                } else {
                    if (first[nonTerminal].count(rule.front())) {
                        std::cerr << "[E]: Grammar has intersection of FIRST sets for different rules" << std::endl;
                        exit(-nonTerminal);
                    }

                    first[nonTerminal].insert(rule.front());
                }
            }
        }
    }
}

void Parser::initializeNextFunctionSets() {
    for (const auto &[nonTerminal, ruleSet] : rules) {
        for (const auto &rule: ruleSet) {
            for (const auto &symbol: rule) {
                if (symbol < PROGRAM)
                    usedTerminals.insert(symbol);
            }
        }
    }
    for (const char &terminal: usedTerminals) {
        initializeNextWithTerminal(terminal);
    }
    copySetsFromNextNonTerminals();
    for (const auto &[nonTerminal, nonTerminalRules]: rules) {
        pushNextFromNonTerminal(nonTerminal);
    }
}

void Parser::initializeNextWithTerminal(char terminal) {
    for (const auto &[nonTerminal, ruleSet] : rules) {
        for (const auto &rule : ruleSet) {
            for (int i = 1; i < rule.size(); ++i) {
                if (rule[i] == terminal && rule[i - 1] >= PROGRAM) {
                    next[rule[i - 1]].insert(terminal);
                }
            }
        }
    }
}


void Parser::copySetsFromNextNonTerminals() {
    for (const auto &[nonTerminal, ruleSet] : rules) {
        for (const auto &rule : ruleSet) {
            for (int i = 1; i < rule.size(); ++i) {
                if (rule[i] >= PROGRAM && rule[i - 1] >= PROGRAM) {
                    for (const auto &terminal : first[rule[i]]) {
                        next[rule[i - 1]].insert(terminal);
                    }
                }
            }
        }
    }
}

void Parser::pushNextFromNonTerminal(int nonTerminal) {
    bool hasEmptyRule = false;
    for (const auto &rule : rules[nonTerminal]) {
        hasEmptyRule |= rule.empty();
        if (!rule.empty() && rule.back() >= PROGRAM) {
            bool terminalAdded = false;
            for (const auto &terminal: next[nonTerminal]) {
                if (!next[rule.back()].count(terminal)) {
                    terminalAdded = true;
                    next[rule.back()].insert(terminal);
                }
            }
            if (terminalAdded)
                pushNextFromNonTerminal(rule.back());
        }
    }
    if (hasEmptyRule) {
        for (const auto &[ruleNonTerminal, ruleSet] : rules) {
            for (const auto &rule : ruleSet) {
                for (int i = 1; i < rule.size(); ++i) {
                    if (rule[i] == nonTerminal && rule[i - 1] >= PROGRAM) {
                        int newNonTerminal = rule[i - 1];

                        bool terminalAdded = false;
                        for (const auto &terminal: next[nonTerminal]) {
                            if (!next[newNonTerminal].count(terminal)) {
                                terminalAdded = true;
                                next[newNonTerminal].insert(terminal);
                            }
                        }
                        if (terminalAdded)
                            pushNextFromNonTerminal(newNonTerminal);
                    }
                }
            }
        }
    }
}

void Parser::constructParsingTable() {
    for (const auto &[nonTerminal, subRules]: rules) {
        for (int i = 0; i < (int) subRules.size(); ++i) {
            if (subRules[i].empty()) {
                for (const auto &terminal: next[nonTerminal]) {
                    if (parsingTable[nonTerminal].count(terminal)) {
                        std::cerr << "[E]: Grammar had intersection of FIRST and FOLLOW sets for non-terminal"
                                  << std::endl;
                        exit(-nonTerminal);
                    }
                    parsingTable[nonTerminal][terminal] = i;
                }
            } else {
                if (subRules[i].front() >= PROGRAM) {
                    for (const auto &terminal: first[subRules[i].front()]) {
                        if (parsingTable[nonTerminal].count(terminal)) {
                            std::cerr << "[E]: Grammar had intersection while constructing parsing table" << std::endl;
                            exit(-nonTerminal);
                        }
                        parsingTable[nonTerminal][terminal] = i;
                    }
                } else {
                    if (parsingTable[nonTerminal].count(subRules[i].front())) {
                        std::cerr << "[E]: Grammar had intersection while constructing parsing table" << std::endl;
                        exit(-nonTerminal);
                    }
                    parsingTable[nonTerminal][subRules[i].front()] = i;
                }
            }
        }
    }
}

bool Parser::isStackEmpty() const {
    return syntaxStack.empty();
}
