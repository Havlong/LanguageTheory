/**
 * 21.09.2020
 * SyntaxTreeNode
 *
 * @author Havlong
 * @version v1.0
 */

#ifndef LANGUAGETHEORY_SYNTAXTREENODE_H
#define LANGUAGETHEORY_SYNTAXTREENODE_H

class SyntaxTreeNode {
private:
    SyntaxTreeNode *parent;
public:
    SyntaxTreeNode() { this->parent = nullptr; }

    explicit SyntaxTreeNode(SyntaxTreeNode *parent) : parent(parent) {}

    [[nodiscard]] SyntaxTreeNode *getParent() const {
        return parent;
    }
};


#endif //LANGUAGETHEORY_SYNTAXTREENODE_H
