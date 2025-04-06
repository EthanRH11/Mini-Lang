#ifndef INTERPERTER_HPP
#define INTERPERTER_HPP

#include <map>

#include "parser.hpp"

class Interperter
{
public:
    Interperter(AST_NODE *root) : root(root) {}

    void execute()
    {
        executeNode(root);
    }

private:
    AST_NODE *root;
    std::map<std::string, int> variables;

    void executeNode(AST_NODE *node);
    int evaluateExpression(AST_NODE *node);
    void executeStatement(AST_NODE *node);

    // May need to implement more handling
};

#endif // INTERPERTER_HPP