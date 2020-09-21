/**
 * 21.09.2020
 * SyntaxTreeLeaf
 *
 * @author Havlong
 * @version v1.0
 */

#ifndef LANGUAGETHEORY_SYNTAXTREELEAF_H
#define LANGUAGETHEORY_SYNTAXTREELEAF_H


#include <string>
#include <utility>
#include "SyntaxTreeBranch.h"

class SyntaxTreeLeaf : public SyntaxTreeNode {
private:
    std::string data;
public:
    SyntaxTreeLeaf(SyntaxTreeNode *parent, std::string data) : SyntaxTreeNode(parent), data(std::move(data)) {}

    [[nodiscard]] const std::string &getData() const {
        return data;
    }

    [[nodiscard]] SyntaxTreeNode *pushLeaf() {
        auto *parentBranch = (SyntaxTreeBranch *) (getParent());
        parentBranch->appendChild(this);
        return parentBranch;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREELEAF_H
