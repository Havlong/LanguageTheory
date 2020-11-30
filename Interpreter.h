#ifndef LANGUAGETHEORY_INTERPRETER_H
#define LANGUAGETHEORY_INTERPRETER_H


#include <map>
#include "SyntaxTreeNode.h"

/**
 * 14.09.2020
 * Interpreter
 *
 * @author Havlong
 * @version v1.0
 */

class Interpreter {
private:
    std::map<std::string, long long> variableMap;

    static std::vector<SyntaxTreeNode *> useVarList(SyntaxTreeNode *varList);

    static SyntaxTreeNode *useVar(SyntaxTreeNode *var);

    long long evaluateExpression(SyntaxTreeNode *expressionNode);

    long long getValue(SyntaxTreeNode *expressionNode);

public:
    void executeSubTree(SyntaxTreeNode *subTree);
};


#endif //LANGUAGETHEORY_INTERPRETER_H
