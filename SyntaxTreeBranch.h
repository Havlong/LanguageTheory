/**
 * 21.09.2020
 * SyntaxTreeBranch
 *
 * @author Havlong
 * @version v1.0
 */

#ifndef LANGUAGETHEORY_SYNTAXTREEBRANCH_H
#define LANGUAGETHEORY_SYNTAXTREEBRANCH_H

#include <vector>
#include "SyntaxTreeNode.h"

class SyntaxTreeBranch : public SyntaxTreeNode {
private:
    std::vector<SyntaxTreeNode *> children;
    int nonTerminal;
public:
    explicit SyntaxTreeBranch(int nonTerminal) : nonTerminal(nonTerminal) {}

    SyntaxTreeBranch(SyntaxTreeNode *parent, int nonTerminal) : SyntaxTreeNode(parent), nonTerminal(nonTerminal) {}

    void appendChild(SyntaxTreeNode *child) {
        SyntaxTreeBranch::children.push_back(child);
    }

    [[nodiscard]] const std::vector<SyntaxTreeNode *> &getChildren() const {
        return children;
    }

    [[nodiscard]] int getNonTerminal() const {
        return nonTerminal;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREEBRANCH_H
