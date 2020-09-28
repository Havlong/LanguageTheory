#ifndef LANGUAGETHEORY_SYNTAXTREEBRANCH_H
#define LANGUAGETHEORY_SYNTAXTREEBRANCH_H

#include <vector>
#include "SyntaxTreeNode.h"

/**
 * 21.09.2020
 * SyntaxTreeBranch
 *
 * @author Havlong
 * @version v1.0
 */
class SyntaxTreeBranch : public SyntaxTreeNode {
private:
    std::vector<SyntaxTreeNode *> children;

public:
    explicit SyntaxTreeBranch(int nonTerminal) : SyntaxTreeNode(nonTerminal) {}

    SyntaxTreeBranch(SyntaxTreeBranch *parent, int nonTerminal) : SyntaxTreeNode(parent, nonTerminal) {}

    void appendChild(SyntaxTreeNode *child) {
        SyntaxTreeBranch::children.push_back(child);
    }

    [[nodiscard]] const std::vector<SyntaxTreeNode *> &getChildren() const {
        return children;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREEBRANCH_H
