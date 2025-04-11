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
    case NODE_IF:
    {
        Value condition = evaluateExpression(node->CHILD);

        bool conditionResult = false;

        if (condition.isInteger())
        {
            conditionResult = condition.getInteger() != 0;
        }
        else if (condition.isDouble())
        {
            conditionResult = condition.getDouble() != 0.0;
        }
        else if (condition.isBool())
        {
            conditionResult = condition.getBool();
        }
        else if (condition.isString())
        {
            conditionResult = !condition.getString().empty();
        }
        else if (condition.isChar())
        {
            conditionResult = condition.getChar() != '\0';
        }

        if (conditionResult)
        {
            if (node->SUB_STATEMENTS.size() > 0)
            {
                executeNode(node->SUB_STATEMENTS[0]);
            }
        }
        else if (node->SUB_STATEMENTS.size() > 1)
        {
            executeNode(node->SUB_STATEMENTS[1]);
        }
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
    case NODE_BLOCK:
        for (auto &stmt : node->SUB_STATEMENTS)
        {
            executeNode(stmt);
        }
        break;
    case NODE_FOR:
    {
        AST_NODE *args = node->CHILD;
        if (!args || args->TYPE != NODE_FOR_ARGS || args->SUB_STATEMENTS.size() != 3)
        {
            std::cerr << "Error: few too many arguments for loop structure";
            exit(1);
        }

        AST_NODE *initNode = args->SUB_STATEMENTS[0];
        if (initNode)
        {
            executeNode(initNode);
        }

        // Loop execution
        while (true)
        {
            // Check condition
            AST_NODE *condNode = args->SUB_STATEMENTS[1];
            if (condNode)
            {
                Value condResult = evaluateExpression(condNode);

                // Convert condition result to boolean
                bool continueLoop = false;
                if (condResult.isInteger())
                {
                    continueLoop = condResult.getInteger() != 0;
                }
                else if (condResult.isDouble())
                {
                    continueLoop = condResult.getDouble() != 0.0;
                }
                else if (condResult.isBool())
                {
                    continueLoop = condResult.getBool();
                }
                else if (condResult.isString())
                {
                    continueLoop = !condResult.getString().empty();
                }
                else if (condResult.isChar())
                {
                    continueLoop = condResult.getChar() != '\0';
                }

                // Exit loop if condition is false
                if (!continueLoop)
                {
                    break;
                }
            }

            // Execute loop body
            if (!node->SUB_STATEMENTS.empty())
            {
                executeNode(node->SUB_STATEMENTS[0]);
            }

            // Execute increment expression
            AST_NODE *incrNode = args->SUB_STATEMENTS[2];
            if (incrNode)
            {
                evaluateExpression(incrNode);
            }
        }
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
    case NODE_SUBT:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
            Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

            // Implement operator-
            return Value(left.getInteger() - right.getInteger());
        }
        return Value(0);
    case NODE_MULT:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
            Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

            // Implement operator*
            return Value(left.getInteger() * right.getInteger());
        }
        return Value(0);
    case NODE_LESS_THAN:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
            Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() < right.getInteger());
        }
        return Value(false);
    case NODE_GREATER_THAN:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
            Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() > right.getInteger());
        }
        return Value(false);
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
    case NODE_OPERATOR_INCREMENT:
    {
        if (node->SUB_STATEMENTS.size() != 1)
        {
            std::cerr << "ERROR: Increment operator requires exactly one operand" << std::endl;
            exit(1);
        }
        AST_NODE *operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            std::cerr << "ERROR: Increment operator can only be applied to variables" << std::endl;
            exit(1);
        }

        std::string varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            exit(1);
        }

        Value &value = variables[varName];
        if (value.isInteger())
        {
            int newValue = value.getInteger() + 1;
            value = Value(newValue);
            return value;
        }
        else if (value.isDouble())
        {
            double newValue = value.getDouble() + 1.0;
            value = Value(newValue);
            return value;
        }
        else if (value.isChar())
        {
            char newValue = value.getChar() + 1;
            value = Value(newValue);
            return value;
        }
        else
        {
            std::cerr << "ERROR: Increment operator not supported for this type" << std::endl;
            exit(1);
        }
    }
    break;

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