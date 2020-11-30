/**
 * 14.09.2020
 * Interpreter
 *
 * @author Havlong
 * @version v1.0
 */

#include <stack>
#include "Interpreter.h"

void Interpreter::executeSubTree(SyntaxTreeNode *subTree) {
    switch (subTree->getNonTerminal()) {
        case StaticProvider::PROGRAM:
        case StaticProvider::OPERATOR_LIST:
        case StaticProvider::OPERATOR_DECLARATION: {
            for (const auto &child : subTree->getChildren()) {
                executeSubTree(child);
            }
            break;
        }
        case StaticProvider::VAR_DECLARATION: {
            SyntaxTreeNode *varList = subTree->getChildren()[0];
            SyntaxTreeNode *type = subTree->getChildren()[1];

            if (type->getNonTerminal() != StaticProvider::TYPE || type->getData() != "INTEGER") {
                std::cerr << "[E]: Tree error. Integer type expected, but got:\n" << *type;
                exit(-7);
            }
            for (const auto &var : useVarList(varList)) {
                std::string varName = useVar(var)->getData();
                if (varName.size() > StaticProvider::VAR_MAX_SIZE) {
                    std::cerr << "[E]: Used identifier with size more than " << StaticProvider::VAR_MAX_SIZE
                              << " symbols in it\n";
                    exit(-7);
                }
                if (variableMap.count(varName)) {
                    std::cerr << "[E]: Variable name " << varName << " already used\n";
                    exit(-7);
                }
                variableMap[varName] = 0;
            }
            break;
        }
        case StaticProvider::OPERATOR: {
            if (subTree->getData() == "=") {
                std::string varName = useVar(subTree->getChildren()[0])->getData();
                long long result = evaluateExpression(subTree->getChildren()[1]);
                if (!variableMap.count(varName)) {
                    std::cerr << "[E]: Variable " << varName << " has not been declared\n";
                    exit(-7);
                }
                variableMap[varName] = result;
            } else if (subTree->getData() == "READ") {
                for (const auto &var : useVarList(subTree->getChildren()[0])) {
                    std::string varName = useVar(var)->getData();
                    if (!variableMap.count(varName)) {
                        std::cerr << "[E]: Variable " << varName << " is not declared\n";
                        exit(-7);
                    }
                    std::cin >> variableMap[varName];
                }
            } else if (subTree->getData() == "WRITE") {
                for (const auto &var : useVarList(subTree->getChildren()[0])) {
                    std::string varName = useVar(var)->getData();
                    if (!variableMap.count(varName)) {
                        std::cerr << "[E]: Variable " << varName << " is not declared\n";
                        exit(-7);
                    }
                    std::cout << variableMap[varName] << '\n';
                }
            } else if (subTree->getData() == "FOR") {
                std::string varName = useVar(subTree->getChildren()[0])->getData();
                long long a = evaluateExpression(subTree->getChildren()[1]);
                long long b = evaluateExpression(subTree->getChildren()[2]);
                if (a < b)
                    for (variableMap[varName] = a; variableMap[varName] <= b; ++variableMap[varName]) {
                        executeSubTree(subTree->getChildren()[3]);
                    }
                else
                    for (variableMap[varName] = a; variableMap[varName] >= b; --variableMap[varName]) {
                        executeSubTree(subTree->getChildren()[3]);
                    }
            } else {
                std::cerr << "[E]: Tree error. Operator expected, but got:" << subTree->getData() << '\n';
                exit(-7);
            }
            break;
        }
    }
}

std::vector<SyntaxTreeNode *> Interpreter::useVarList(SyntaxTreeNode *varList) {
    if (varList->getNonTerminal() != StaticProvider::VAR_LIST) {
        std::cerr << "[E]: Tree error. Variable list expected, but got:\n" << *varList;
        exit(-7);
    }
    return varList->getChildren();
}

SyntaxTreeNode *Interpreter::useVar(SyntaxTreeNode *var) {
    if (var->getNonTerminal() != StaticProvider::VAR) {
        std::cerr << "[E]: Tree error. Variable expected, but got:\n" << *var;
        exit(-7);
    }
    return var;
}

long long Interpreter::evaluateExpression(SyntaxTreeNode *expressionNode) {
    if (expressionNode->getNonTerminal() != StaticProvider::EXPRESSION) {
        std::cerr << "[E]: Tree error. Expression expected, but got:\n" << *expressionNode;
        exit(-7);
    }
    std::vector<SyntaxTreeNode *> expression(expressionNode->getChildren());
    if (expression.empty()) {
        std::cerr << "[E]: Tree error. Empty expression provided.\n";
        exit(-7);
    }
    std::stack<long long> values;
    std::stack<std::pair<char, int>> binary;
    if (expression[0]->getNonTerminal() == StaticProvider::UNARY_OPERATOR) {
        values.push(-getValue(expression[1]));
        expression.erase(expression.begin());
        expression.erase(expression.begin());
    }
    for (const auto &token : expression) {
        if (token->getNonTerminal() == StaticProvider::BINARY_OPERATOR) {
            std::pair<char, int> newOp;
            if (token->getData() == "*")
                newOp = {'*', 2};
            else if (token->getData() == "+")
                newOp = {'+', 1};
            else if (token->getData() == "-")
                newOp = {'-', 1};
            else {
                std::cerr << "[E]: Unknown binary operator met\n";
                exit(-7);
            }
            while (!binary.empty() && binary.top().second >= newOp.second) {
                int second = values.top();
                values.pop();

                int first = values.top();
                values.pop();

                auto op = binary.top();
                binary.pop();

                switch (op.first) {
                    case '-': {
                        values.push(first - second);
                        break;
                    }
                    case '+': {
                        values.push(first + second);
                        break;
                    }
                    case '*': {
                        values.push(first * second);
                        break;
                    }
                }
            }
            binary.push(newOp);
        } else {
            values.push(getValue(token));
        }
    }
    while (!binary.empty()) {
        int second = values.top();
        values.pop();

        int first = values.top();
        values.pop();

        auto op = binary.top();
        binary.pop();

        switch (op.first) {
            case '-': {
                values.push(first - second);
                break;
            }
            case '+': {
                values.push(first + second);
                break;
            }
            case '*': {
                values.push(first * second);
                break;
            }
        }
    }
    return values.top();
}

long long Interpreter::getValue(SyntaxTreeNode *expressionNode) {
    switch (expressionNode->getNonTerminal()) {
        case StaticProvider::EXPRESSION: {
            return evaluateExpression(expressionNode);
        }
        case StaticProvider::VAR: {
            std::string varName = useVar(expressionNode)->getData();
            if (!variableMap.count(varName)) {
                std::cerr << "[E]: Variable " << varName << " has not been declared\n";
                exit(-7);
            }
            return variableMap[varName];
        }
        case StaticProvider::CONST: {
            if (expressionNode->getNonTerminal() != StaticProvider::CONST) {
                std::cerr << "[E]: Tree error. Variable list expected, but got:\n" << *expressionNode;
                exit(-7);
            }
            return std::stoi(expressionNode->getData());
        }
        default: {
            std::cerr << "[E]: Tree error. Unknown subexpression can not be calculated\n";
            exit(-7);
        }
    }
}


