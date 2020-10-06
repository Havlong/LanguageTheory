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

    virtual bool isBranch() = 0;

    friend std::ostream &operator<<(std::ostream &os, const SyntaxTreeNode &node) {
        os << "nonTerminal: ";
        switch (node.nonTerminal) {
            case StaticProvider::PROGRAM: {
                os << "PROGRAM";
                break;
            }
            case StaticProvider::TYPE: {
                os << "TYPE";
                break;
            }
            case StaticProvider::LETTER: {
                os << "LETTER";
                break;
            }
            case StaticProvider::DIGIT: {
                os << "DIGIT";
                break;
            }
            case StaticProvider::VAR_DECLARATION: {
                os << "VAR_DECLARATION";
                break;
            }
            case StaticProvider::VAR_LIST: {
                os << "VAR_LIST";
                break;
            }
            case StaticProvider::OPERATOR_DECLARATION: {
                os << "OPERATOR_DECLARATION";
                break;
            }
            case StaticProvider::OPERATOR: {
                os << "OPERATOR";
                break;
            }
            case StaticProvider::OPERATOR_LIST: {
                os << "OPERATOR_LIST";
                break;
            }
            case StaticProvider::EXPRESSION: {
                os << "EXPRESSION";
                break;
            }
            case StaticProvider::EXPRESSION_LIST: {
                os << "EXPRESSION_LIST";
                break;
            }
            case StaticProvider::BRACKET_EXPRESSION: {
                os << "BRACKET_EXPRESSION";
                break;
            }
            case StaticProvider::SUBEXPRESSION: {
                os << "SUBEXPRESSION";
                break;
            }
            case StaticProvider::UNARY_OPERATOR: {
                os << "UNARY_OPERATOR";
                break;
            }
            case StaticProvider::BINARY_OPERATOR: {
                os << "BINARY_OPERATOR";
                break;
            }
            case StaticProvider::OPERAND: {
                os << "OPERAND";
                break;
            }
            case StaticProvider::CONST: {
                os << "CONST";
                break;
            }
            case StaticProvider::VAR: {
                os << "VAR";
                break;
            }
            case StaticProvider::VAR_LIST_CONTINUATION: {
                os << "VAR_LIST_CONTINUATION";
                break;
            }
            case StaticProvider::EXPRESSION_LIST_CONTINUATION: {
                os << "EXPRESSION_LIST_CONTINUATION";
                break;
            }
            case StaticProvider::OPERATOR_LIST_CONTINUATION: {
                os << "OPERATOR_LIST_CONTINUATION";
                break;
            }
            case StaticProvider::VAR_CONTINUATION: {
                os << "VAR_CONTINUATION";
                break;
            }
            case StaticProvider::CONST_CONTINUATION: {
                os << "CONST_CONTINUATION";
                break;
            }
            default: {
                os << "Unknown non-terminal";
                break;
            }
        }
        return os;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREENODE_H
