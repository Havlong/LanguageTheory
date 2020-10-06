/**
 * 14.09.2020
 * Parser
 *
 * @author Havlong
 * @version v1.0
 */

#include "Parser.h"

using namespace StaticProvider;

Parser::Parser() {
    initializeGrammar();
    initializeFirstFunctionSets();
    initializeNextFunctionSets();
    constructParsingTable();
    syntaxStack.push(PROGRAM);
}

SyntaxTreeBranch *Parser::parseLine(const std::string &line, SyntaxTreeBranch *branch) {
    SyntaxTreeNode *current = branch;
    std::string data;
    for (int i = 0; i < line.size();) {
        int symbol = syntaxStack.top();
        syntaxStack.pop();
        if (symbol >= PROGRAM) {
            while (std::isspace(line[i])) ++i;

            if (parsingTable.count(symbol)) {
                if (parsingTable[symbol].count(line[i])) {
                    if (symbol == LETTER || symbol == DIGIT) {
                        data.push_back(line[i++]);
                    } else {
                        if (symbol == CONST_CONTINUATION || symbol == VAR_CONTINUATION) {
                            int ruleIndex = parsingTable[symbol][line[i]];
                            if (rules[symbol][ruleIndex].empty()) {
                                current = new SyntaxTreeLeaf(current, symbol, data);
                                current = ((SyntaxTreeLeaf *) current)->pushLeaf();
                                data.clear();
                            }
                        } else if (current->getNonTerminal() != symbol) {
                            current = new SyntaxTreeBranch((SyntaxTreeBranch *) current, symbol);
                            syntaxStack.push(END_OF_NODE);
                        }
                        const std::vector<int> &rule = rules[symbol][parsingTable[symbol][line[i]]];
                        for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
                            syntaxStack.push(*it);
                        }
                    }
                } else {
                    // TODO: Process Syntax Error
                    std::string remaining = line.substr(i);
                    std::cerr << "Syntax Error\n";
                    exit(-5);
                }
            } else {
                std::cerr << "Error, expected nonTerminal, that does not exist in parsing table\n";
                exit(-6);
            }
        } else if (symbol == END_OF_NODE) {
            while (std::isspace(line[i])) ++i;

            auto *parent = (SyntaxTreeBranch *) (current->getParent());
            parent->appendChild(current);
            current = parent;
        } else {
            if (line[i] == symbol) {
                i++;
            } else {
                // TODO: Process Syntax Error
                std::string remaining = line.substr(i);
                std::cerr << "Syntax Error\n";
                exit(-5);
            }
        }
    }
    return (SyntaxTreeBranch *) (current);
}

SyntaxTreeBranch Parser::parseProgram(const std::string &program) {
    // TODO: Implement line splitter and parseLine() usage
    // TODO: Treat space symbols as separators with non-terminals
    SyntaxTreeBranch programTree(PROGRAM);

    // FOR EACH LINE
    parseLine(program, &programTree);

    return programTree;
}

void Parser::initializeGrammar() {
    rules[PROGRAM].push_back({VAR_DECLARATION, OPERATOR_DECLARATION});
    rules[OPERATOR_DECLARATION].push_back({'<', '<', OPERATOR_LIST, '>', '>'});
    rules[VAR_DECLARATION].push_back({'V', 'A', 'R', VAR_LIST, ':', TYPE, ';'});

    rules[VAR_LIST].push_back({VAR, VAR_LIST_CONTINUATION});
    rules[VAR_LIST_CONTINUATION].push_back({',', VAR_LIST});
    rules[VAR_LIST_CONTINUATION].push_back({});
    rules[TYPE].push_back({'I', 'N', 'T', 'E', 'G', 'E', 'R'});

    rules[OPERATOR_LIST].push_back({OPERATOR, OPERATOR_LIST_CONTINUATION});
    rules[OPERATOR_LIST_CONTINUATION].push_back({OPERATOR_LIST});
    rules[OPERATOR_LIST_CONTINUATION].push_back({});

    rules[OPERATOR].push_back({VAR, '=', EXPRESSION, ';'});
    rules[OPERATOR].push_back({'R', 'E', 'A', 'D', '[', VAR_LIST, ']', ';'});
    rules[OPERATOR].push_back({'W', 'R', 'I', 'T', 'E', '[', VAR_LIST, ']', ';'});
    rules[OPERATOR].push_back(
            {'F', 'O', 'R', VAR, ':', EXPRESSION, '.', '.', EXPRESSION, OPERATOR_DECLARATION});

    rules[EXPRESSION].push_back({UNARY_OPERATOR, EXPRESSION_LIST});
    rules[EXPRESSION].push_back({EXPRESSION_LIST});

    rules[EXPRESSION_LIST].push_back({SUBEXPRESSION, EXPRESSION_LIST_CONTINUATION});
    rules[EXPRESSION_LIST_CONTINUATION].push_back({BINARY_OPERATOR, EXPRESSION_LIST});
    rules[EXPRESSION_LIST_CONTINUATION].push_back({});

    rules[SUBEXPRESSION].push_back({BRACKET_EXPRESSION});
    rules[SUBEXPRESSION].push_back({OPERAND});
    rules[BRACKET_EXPRESSION].push_back({'(', EXPRESSION, ')'});

    rules[UNARY_OPERATOR].push_back({'-'});
    rules[BINARY_OPERATOR].push_back({'+'});
    rules[BINARY_OPERATOR].push_back({'-'});
    rules[BINARY_OPERATOR].push_back({'*'});

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
                            std::cerr << "Grammar has intersection of FIRST sets for different rules" << std::endl;
                            exit(-nonTerminal);
                        }

                        first[nonTerminal].insert(terminal);
                    }
                } else {
                    if (first[nonTerminal].count(rule.front())) {
                        std::cerr << "Grammar has intersection of FIRST sets for different rules" << std::endl;
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
                        std::cerr << "Grammar had intersection of FIRST and FOLLOW sets for non-terminal" << std::endl;
                        exit(-nonTerminal);
                    }
                    parsingTable[nonTerminal][terminal] = i;
                }
            } else {
                if (subRules[i].front() >= PROGRAM) {
                    for (const auto &terminal: first[subRules[i].front()]) {
                        if (parsingTable[nonTerminal].count(terminal)) {
                            std::cerr << "Grammar had intersection while constructing parsing table" << std::endl;
                            exit(-nonTerminal);
                        }
                        parsingTable[nonTerminal][terminal] = i;
                    }
                } else {
                    if (parsingTable[nonTerminal].count(subRules[i].front())) {
                        std::cerr << "Grammar had intersection while constructing parsing table" << std::endl;
                        exit(-nonTerminal);
                    }
                    parsingTable[nonTerminal][subRules[i].front()] = i;
                }
            }
        }
    }
}
