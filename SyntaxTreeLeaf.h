#ifndef LANGUAGETHEORY_SYNTAXTREELEAF_H
#define LANGUAGETHEORY_SYNTAXTREELEAF_H


#include <string>
#include <utility>
#include "SyntaxTreeBranch.h"

/**
 * 21.09.2020
 * SyntaxTreeLeaf
 *
 * @author Havlong
 * @version v1.0
 */
class SyntaxTreeLeaf : public SyntaxTreeNode {
private:
    std::string data;
public:
    SyntaxTreeLeaf(SyntaxTreeNode *parent, int nonTerminal, std::string data) : SyntaxTreeNode(parent, nonTerminal),
                                                                                data(std::move(data)) {}

    [[nodiscard]] const std::string &getData() const {
        return data;
    }

    [[nodiscard]] SyntaxTreeBranch *pushLeaf() {
        auto *parentBranch = (SyntaxTreeBranch *) (getParent());
        parentBranch->appendChild(this);
        return parentBranch;
    }

    bool isBranch() override {
        return false;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREELEAF_H
