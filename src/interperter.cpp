#include <iostream>
#include <map>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <variant>
#include "interperter.hpp"
#include "Value.hpp"

namespace fs = std::filesystem;

void Interperter::setupOutputFile()
{
    fs::create_directories("output");

    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "../output/output_" << std::put_time(std::localtime(&timeNow), "%Y-%m-%d_%H-%M-%S") << ".txt";
    std::string filename = ss.str();

    outputFile.open(filename);
    if (!outputFile.is_open())
    {
        std::cerr << "failed to create output file: " << filename << std::endl;
        std::exit(1);
    }
}

void Interperter::executeNode(AST_NODE *node)
{
    if (!node)
        return;

    switch (node->TYPE)
    {
    case NODE_ROOT:
        for (auto &stmt : node->SUB_STATEMENTS)
        {
            executeNode(stmt);
        }
        break;
    case NODE_INT:
        if (node->CHILD)
        {
            int value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = 0;
        }
        break;
    case NODE_IDENTIFIER:
        if (variables.find(node->VALUE) == variables.end())
        {
            std::cerr << "ERROR: Undefined Variable '" << node->VALUE << "'" << std::endl;
            exit(1);
        }
        break;
    case NODE_PRINT:
        if (node->CHILD)
        {
            int result = evaluateExpression(node->CHILD);
            printToOutput(result);
        }
        else
        {
            std::cout << "EMPTY PRINT STATEMENT" << std::endl;
        }
        break;
    case NODE_PAREN_EXPR:
        if (node->CHILD)
        {
            evaluateExpression(node->CHILD);
        }
        break;
    case NODE_ADD:
        evaluateExpression(node);
        break;
    case NODE_SEMICOLON:
        break;
    case NODE_EOF:
        break;
    default:
        std::cerr << "Interpertation Error: Unknown node type: " << node->TYPE << std::endl;
        exit(1);
    }
}

int Interperter::evaluateExpression(AST_NODE *node)
{
    if (!node)
        return 0;

    switch (node->TYPE)
    {
    case NODE_INT_LITERAL:
        return std::stoi(node->VALUE);
    case NODE_ADD:
        return evaluateExpression(node->SUB_STATEMENTS[0]) +
               evaluateExpression(node->SUB_STATEMENTS[1]);
    case NODE_IDENTIFIER:
        if (variables.find(node->VALUE) != variables.end())
        {
            return variables[node->VALUE];
        }
        else
        {
            std::cerr << "ERROR: Undefined variable '" << node->VALUE << "'" << std::endl;
            exit(1);
        }
    default:
        std::cerr << "ERROR: Unexpected Expression of type: '" << node->VALUE << "'" << std::endl;
        exit(1);
    }
}
void Interperter::executeStatement(AST_NODE *node)
{
    /*Handle Statements such as:
     * Variable Declerations
     * Assignment Statements
     * Print Statements
     * Implement more later on...*/
    if (!node)
    {
        return;
    }

    switch (node->TYPE)
    {
    case NODE_INT:
        // Handle variable declerations
        //  - get var name
        //  - check if theres an initializer
        //  - if yes evaluate it
        //  - store in symbol table
        if (node->CHILD)
        {
            int value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = 0;
        }
        break;
    case NODE_PRINT:
        // Handle print:
        //  - check if there is an expression to print
        //  - evaluate it
        //  - output result
        if (node->CHILD)
        {
            Value result = evaluateExpression(node->CHILD);
            outputFile << result.toString() << std::endl;
        }
        else
        {
            std::cout << "EMPTY PRINT" << std::endl;
        }
        break;
    case NODE_SEMICOLON:
        break;
    default:
        std::cerr << "Unknown Statement Type" << std::endl;
        exit(1);
    }
}