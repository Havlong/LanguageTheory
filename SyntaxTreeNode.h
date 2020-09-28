#ifndef LANGUAGETHEORY_SYNTAXTREENODE_H
#define LANGUAGETHEORY_SYNTAXTREENODE_H

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
};


#endif //LANGUAGETHEORY_SYNTAXTREENODE_H
