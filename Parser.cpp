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
    initializeNextFunctionSets();
    constructParsingTable();
    syntaxStack.push(PROGRAM);
}

template<typename Base, typename T>
inline bool instance_of(const T *) {
    return std::is_base_of<Base, T>::value;
}

SyntaxTreeBranch *Parser::parseLine(const std::string &line, SyntaxTreeBranch *branch) {
    SyntaxTreeNode *current = branch;
    std::string data;
    for (int i = 0; i < line.size();) {
        if (i == 38) {
            std::cout << "HELLO, BUG" << std::endl;
        }
        int symbol = syntaxStack.top();
        syntaxStack.pop();
        if (symbol >= PROGRAM) {
            // TODO: TEST spaces skip
            while (std::isspace(line[i])) ++i;

            if (parsingTable.count(symbol)) {
                if (parsingTable[symbol].count(line[i])) {
                    if (symbol == LETTER || symbol == DIGIT) {
                        data.push_back(line[i++]);
                    } else {
                        if (symbol >= CONST) {
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
            // TODO: TEST spaces skip
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
    rules[VAR_LIST_CONTINUATION].push_back({',', VAR, VAR_LIST_CONTINUATION});
    rules[VAR_LIST_CONTINUATION].push_back({});
    rules[TYPE].push_back({'I', 'N', 'T', 'E', 'G', 'E', 'R'});

    rules[OPERATOR_LIST].push_back({OPERATOR, OPERATOR_LIST});
    rules[OPERATOR_LIST].push_back({});
    rules[OPERATOR].push_back({VAR, '=', EXPRESSION, ';'});
    rules[OPERATOR].push_back({'R', 'E', 'A', 'D', '[', VAR_LIST, ']', ';'});
    rules[OPERATOR].push_back({'W', 'R', 'I', 'T', 'E', '[', VAR_LIST, ']', ';'});
    rules[OPERATOR].push_back(
            {'F', 'O', 'R', VAR, ':', EXPRESSION, '.', '.', EXPRESSION, OPERATOR_DECLARATION});

    rules[EXPRESSION].push_back({UNARY_OPERATOR, EXPRESSION_LIST});
    rules[EXPRESSION].push_back({EXPRESSION_LIST});
    rules[EXPRESSION_LIST].push_back({SUBEXPRESSION, EXPRESSION_LIST_CONTINUATION});
    rules[EXPRESSION_LIST_CONTINUATION].push_back({BINARY_OPERATOR, SUBEXPRESSION, EXPRESSION_LIST_CONTINUATION});
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

    rules[VAR].push_back({LETTER, VAR});
    rules[VAR].push_back({});

    rules[CONST].push_back({DIGIT, CONST});
    rules[CONST].push_back({});

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
    for (const auto&[nonTerminal, rule]: rules) {
        initializeNextForNonTerminal(nonTerminal);
    }
}

void Parser::initializeNextForNonTerminal(int nonTerminal) {
    if (next[nonTerminal].empty()) {
        for (const auto&[ruleNonTerminal, subRules]: rules) {
            for (const auto &rule : subRules) {
                if (!rule.empty()) {
                    for (int i = 0; i < (int) rule.size() - 1; ++i) {
                        if (rule[i] == nonTerminal) {
                            if (rule[i + 1] >= PROGRAM) {
                                for (const auto &terminal : first[rule[i + 1]]) {
                                    next[nonTerminal].insert(terminal);
                                }
                            } else {
                                next[nonTerminal].insert(rule[i + 1]);
                            }
                        }
                    }

                    if (rule.back() == nonTerminal && ruleNonTerminal != nonTerminal) {
                        initializeNextForNonTerminal(ruleNonTerminal);

                        for (const auto &terminal: next[ruleNonTerminal]) {
                            next[nonTerminal].insert(terminal);
                        }
                    }
                }
            }
        }

        for (const auto &rule : rules[nonTerminal]) {
            if (rule.empty()) {
                for (const auto &terminal: first[nonTerminal]) {
                    if (next[nonTerminal].count(terminal)) {
                        std::cerr << "Grammar has intersection of FIRST and FOLLOW sets for empty rule" << std::endl;
                        exit(-nonTerminal);
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
                    parsingTable[nonTerminal][terminal] = i;
                }
            } else {
                if (subRules[i].front() >= PROGRAM)
                    for (const auto &terminal: first[subRules[i].front()]) {
                        parsingTable[nonTerminal][terminal] = i;
                    }
                else
                    parsingTable[nonTerminal][subRules[i].front()] = i;
            }
        }
    }
}
