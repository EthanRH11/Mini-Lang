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

    std::cout << "Executing node: " << getNodeTypeName(node->TYPE) << std::endl;

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
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value(0);
        }
        break;
    case NODE_DOUBLE:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value(0.0);
        }
        break;
    case NODE_CHAR:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value('\0');
        }
        break;
    case NODE_STRING:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value("");
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
            Value result = evaluateExpression(node->CHILD);
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
        std::cerr << "Interpretation Error: Unknown node type: " << getNodeTypeName(node->TYPE) << std::endl;
        exit(1);
    }
}

Value Interperter::evaluateExpression(AST_NODE *node)
{
    if (!node)
        return Value(0);

    std::cout << "Evaluation expression node: " << getNodeTypeName(node->TYPE) << std::endl;

    switch (node->TYPE)
    {
    case NODE_INT_LITERAL:
        return Value(std::stoi(node->VALUE));
    case NODE_DOUBLE_LITERAL:
        return Value(std::stod(node->VALUE));
    case NODE_CHAR_LITERAL:
        if (node->VALUE.length() == 1)
        {
            return Value(node->VALUE);
        }
        else
        {
            return Value('\0');
        }
    case NODE_STRING_LITERAL:
        return Value(node->VALUE);
    case NODE_ADD:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
            Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return left + right;
        }
        return Value(0);
    case NODE_IDENTIFIER:
        if (variables.find(node->VALUE) != variables.end())
        {
            return variables[node->VALUE];
        }
        else
        {
            std::cerr << "ERROR: Unexpected Expression of type: " << getNodeTypeName(node->TYPE) << "'" << std::endl;
            exit(1);
        }
    default:
        std::cerr << "ERROR: Unexpected Expression of type: " << getNodeTypeName(node->TYPE) << "'" << std::endl;
        exit(1);
    }
}

void Interperter::executeStatement(AST_NODE *node)
{
    /*Handle Statements such as:
     * Variable Declerations
     * Assignment Statements
     * Print Statements
     * Implement more in future*/

    if (!node)
        return;

    std::cout << "Executing statement: " << getNodeTypeName(node->TYPE) << std::endl;

    switch (node->TYPE)
    {
    case NODE_INT:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value(0);
        }
        break;
    case NODE_DOUBLE:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value(0.0);
        }
        break;
    case NODE_CHAR:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value("");
        }
        break;
    case NODE_PRINT:
        if (node->CHILD)
        {
            Value result = evaluateExpression(node->CHILD);
            printToOutput(result);
        }
        else
        {
            std::cout << "EMPTY PRINT" << std::endl;
        }
        break;
    case NODE_SEMICOLON:
        break;
    default:
        std::cerr << "Unknown Statement Type: " << getNodeTypeName(node->TYPE) << std::endl;
        exit(1);
    }
}