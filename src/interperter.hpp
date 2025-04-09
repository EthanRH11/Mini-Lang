#ifndef INTERPERTER_HPP
#define INTERPERTER_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <variant>

#include "parser.hpp"
#include "Value.hpp"

class Interperter
{
public:
    Interperter(AST_NODE *root) : root(root)
    {
        setupOutputFile();
    }

    ~Interperter()
    {
        if (outputFile.is_open())
        {
            outputFile.close();
        }
    }

    void setupOutputFile();

    void execute()
    {
        executeNode(root);
    }

private:
    AST_NODE *root;
    std::map<std::string, Value> variables;
    std::ofstream outputFile;

    void executeNode(AST_NODE *node);         // Main recursive function
    Value evaluateExpression(AST_NODE *node); // Evaluates expressions such as (10 + x)
    void executeStatement(AST_NODE *node);

    void printToOutput(const Value &val)
    {
        if (outputFile.is_open())
        {
            outputFile << val.toString() << std::endl;
        }
    }

    // May need to implement more handling
};

#endif // INTERPERTER_HPP