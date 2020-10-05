#ifndef LANGUAGETHEORY_SYNTAXTREENODE_H
#define LANGUAGETHEORY_SYNTAXTREENODE_H

#include <ostream>
#include "Static.h"

/**
 * 21.09.2020
 * SyntaxTreeNode
 *
 * @author Havlong
 * @version v1.0
 */
class SyntaxTreeNode {
private:
    SyntaxTreeNode *parent;
    int nonTerminal;
public:
    explicit SyntaxTreeNode(int nonTerminal) : nonTerminal(nonTerminal) { this->parent = nullptr; }

    SyntaxTreeNode(SyntaxTreeNode *parent, int nonTerminal) : parent(parent), nonTerminal(nonTerminal) {}

    [[nodiscard]] SyntaxTreeNode *getParent() const {
        return parent;
    }

    [[nodiscard]] int getNonTerminal() const {
        return nonTerminal;
    }

    friend std::ostream &operator<<(std::ostream &os, const SyntaxTreeNode &node) {
        os << "nonTerminal: ";
        switch (node.nonTerminal) {
            case StaticProvider::PROGRAM: {
                os << "PROGRAM";
            }
            case StaticProvider::TYPE: {
                os << "TYPE";
            }
            case StaticProvider::LETTER: {
                os << "LETTER";
            }
            case StaticProvider::DIGIT: {
                os << "DIGIT";
            }
            case StaticProvider::VAR_DECLARATION: {
                os << "VAR_DECLARATION";
            }
            case StaticProvider::VAR_LIST: {
                os << "VAR_LIST";
            }
            case StaticProvider::OPERATOR_DECLARATION: {
                os << "OPERATOR_DECLARATION";
            }
            case StaticProvider::OPERATOR: {
                os << "OPERATOR";
            }
            case StaticProvider::OPERATOR_LIST: {
                os << "OPERATOR_LIST";
            }
            case StaticProvider::EXPRESSION: {
                os << "EXPRESSION";
            }
            case StaticProvider::EXPRESSION_LIST: {
                os << "EXPRESSION_LIST";
            }
            case StaticProvider::BRACKET_EXPRESSION: {
                os << "BRACKET_EXPRESSION";
            }
            case StaticProvider::SUBEXPRESSION: {
                os << "SUBEXPRESSION";
            }
            case StaticProvider::UNARY_OPERATOR: {
                os << "UNARY_OPERATOR";
            }
            case StaticProvider::BINARY_OPERATOR: {
                os << "BINARY_OPERATOR";
            }
            case StaticProvider::OPERAND: {
                os << "OPERAND";
            }
            case StaticProvider::CONST: {
                os << "CONST";
            }
            case StaticProvider::VAR: {
                os << "VAR";
            }
            case StaticProvider::VAR_LIST_CONTINUATION: {
                os << "VAR_LIST_CONTINUATION";
            }
            case StaticProvider::EXPRESSION_LIST_CONTINUATION: {
                os << "EXPRESSION_LIST_CONTINUATION";
            }
            case StaticProvider::OPERATOR_LIST_CONTINUATION: {
                os << "OPERATOR_LIST_CONTINUATION";
            }
            case StaticProvider::VAR_CONTINUATION: {
                os << "VAR_CONTINUATION";
            }
            case StaticProvider::CONST_CONTINUATION: {
                os << "CONST_CONTINUATION";
            }
            default: {
                os << "Unknown non-terminal";
            }
        }
        return os;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREENODE_H
