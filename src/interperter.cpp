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
#include "parser.hpp"

namespace fs = std::filesystem;

/**
 * @brief Sets up the output file for the interpreter
 *
 * Creates a directory for output files if it doesn't exist and
 * opens a new file with a timestamp in the filename.
 */
void Interpreter::setupOutputFile()
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
        std::cerr << "Failed to create output file: " << filename << std::endl;
        std::exit(1);
    }
}

/**
 * @brief Executes the program starting from the 'begin' block
 *
 * Searches for the 'begin' block in the AST and starts execution from there.
 */
void Interpreter::execute()
{
    AST_NODE *beginBlock = nullptr;

    for (auto &stmt : root->SUB_STATEMENTS)
    {
        if (stmt->TYPE == NODE_BEGIN_BLOCK)
        {
            beginBlock = stmt;
            break;
        }
    }

    if (!beginBlock)
    {
        std::cerr << "Error: No 'begin' block found in program." << std::endl;
        exit(1);
    }
    executeNode(beginBlock);
}

/**
 * @brief Evaluates an expression AST node and returns its value
 *
 * @param node The AST node representing the expression
 * @return Value The result of the expression evaluation
 */
Value Interpreter::evaluateExpression(AST_NODE *node)
{
    if (!node)
        return Value(0);

    // Initialize variables that could be referenced in switch cases at the beginning
    Value left;
    Value right;
    std::string varName;
    AST_NODE *operand = nullptr;
    Value *valuePtr = nullptr;

    switch (node->TYPE)
    {
    case NODE_INT_LITERAL:
        return Value(std::stoi(node->VALUE));
    case NODE_DOUBLE_LITERAL:
        return Value(std::stod(node->VALUE));
    case NODE_BOOL_LITERAL:
        if (node->VALUE == "true")
        {
            return Value(true);
        }
        else if (node->VALUE == "false")
        {
            return Value(false);
        }
        else
        {
            return Value(false);
        }
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
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return left + right;
        }
        return Value(0);
    case NODE_SUBT:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() - right.getInteger());
        }
        return Value(0);
    case NODE_MULT:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() * right.getInteger());
        }
        return Value(0);
    case NODE_DIVISION:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() / right.getInteger());
        }
        return Value(0);
    case NODE_KEYWORD_INPUT:
        return executeInputStatement(node);
    case NODE_MODULUS:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() % right.getInteger());
        }
        return Value(0);
    case NODE_NOT_EQUAL:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() != right.getInteger());
        }
        return Value(false);
    case NODE_LESS_THAN:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() < right.getInteger());
        }
        return Value(false);
    case NODE_GREATER_THAN:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

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
            std::cerr << "ERROR: Undefined variable: '" << node->VALUE << "'" << std::endl;
            exit(1);
        }
    case NODE_LESS_EQUAL:
        if (node->SUB_STATEMENTS.size() >= 2)
        {
            left = evaluateExpression(node->SUB_STATEMENTS[0]);
            right = evaluateExpression(node->SUB_STATEMENTS[1]);

            return Value(left.getInteger() <= right.getInteger());
        }
        return Value(false);
    case NODE_OPERATOR_DECREMENT:
        if (node->SUB_STATEMENTS.size() != 1)
        {
            std::cerr << "ERROR: Decrement operator requires exactly one operand" << std::endl;
            exit(1);
        }

        operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            std::cerr << "ERROR: Decrement operator can only be applied to variables" << std::endl;
            exit(1);
        }

        varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            exit(1);
        }

        valuePtr = &variables[varName];
        if (valuePtr->isInteger())
        {
            int newValue = valuePtr->getInteger() - 1;
            *valuePtr = Value(newValue);
            return *valuePtr;
        }
        else if (valuePtr->isDouble())
        {
            double newValue = valuePtr->getDouble() - 1.0;
            *valuePtr = Value(newValue);
            return *valuePtr;
        }
        else if (valuePtr->isChar())
        {
            char newValue = valuePtr->getChar() - 1;
            *valuePtr = Value(newValue);
            return *valuePtr;
        }
        else
        {
            std::cerr << "ERROR: Decrement operator not supported for this type" << std::endl;
            exit(1);
        }
        break;
    case NODE_OPERATOR_INCREMENT:
        if (node->SUB_STATEMENTS.size() != 1)
        {
            std::cerr << "ERROR: Increment operator requires exactly one operand" << std::endl;
            exit(1);
        }

        operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            std::cerr << "ERROR: Increment operator can only be applied to variables" << std::endl;
            exit(1);
        }

        varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            exit(1);
        }

        valuePtr = &variables[varName];
        if (valuePtr->isInteger())
        {
            int newValue = valuePtr->getInteger() + 1;
            *valuePtr = Value(newValue);
            return *valuePtr;
        }
        else if (valuePtr->isDouble())
        {
            double newValue = valuePtr->getDouble() + 1.0;
            *valuePtr = Value(newValue);
            return *valuePtr;
        }
        else if (valuePtr->isChar())
        {
            char newValue = valuePtr->getChar() + 1;
            *valuePtr = Value(newValue);
            return *valuePtr;
        }
        else
        {
            std::cerr << "ERROR: Increment operator not supported for this type" << std::endl;
            exit(1);
        }
        break;
    case NODE_NEWLINE:
        return Value('\n');
    case NODE_FUNCTION_CALL:
    {
        static int recursionDepth = 0;
        recursionDepth++;

        std::string funcName = node->VALUE;
        std::cout << "Entering Function: " << funcName << " depth: " << recursionDepth << std::endl;
        std::string callID = generateCallID(funcName, recursionDepth);

        // Look up function definition
        AST_NODE *funcDef = findFunctionByName(funcName);
        if (!funcDef)
        {
            std::cerr << "Undefined function: " << funcName << std::endl;
            exit(1);
        }

        // Create a complete copy of the current variable scope
        std::map<std::string, Value> localScope;
        localScope.insert(variables.begin(), variables.end());

        // Bind arguments to parameters
        AST_NODE *params = funcDef->SUB_STATEMENTS[0];
        for (size_t i = 0; i < node->SUB_STATEMENTS.size() && i < params->SUB_STATEMENTS.size(); i++)
        {
            AST_NODE *paramNode = params->SUB_STATEMENTS[i];
            AST_NODE *argNode = node->SUB_STATEMENTS[i];

            // Evaluate argument
            Value argValue = evaluateExpression(argNode);

            // Bind to parameter name in the local scope
            variables[paramNode->VALUE] = argValue;
        }
        returnValue = Value(); // Clear the return value before executing
        // Execute function body
        executeNode(funcDef->CHILD);

        // Get the function's return value
        Value result = returnValue;
        // std::cout << "Return value from " << funcName << " depth " << recursionDepth << ": " << result.getInteger() << std::endl;

        // Restore previous scope
        variables = localScope;

        recursionDepth--;
        return result;
    }
    default:
        std::cerr << "ERROR: Unexpected Expression of type: " << getNodeTypeName(node->TYPE) << "'" << std::endl;
        exit(1);
    }

    // Default return to avoid compiler warning - this code should never be reached
    return Value(0);
}

/**
 * @brief Executes a single statement AST node
 *
 * @param node The AST node representing the statement to execute
 */
void Interpreter::executeStatement(AST_NODE *node)
{
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
    case NODE_BOOL:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value(false);
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

/**
 * @brief Recursively executes an AST node
 *
 * @param node The AST node to execute
 */
void Interpreter::executeNode(AST_NODE *node)
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
        std::cout << "IF condition evaluated to: " << (conditionResult ? "true" : "false") << std::endl;

        if (conditionResult)
        {
            if (node->SUB_STATEMENTS.size() > 0)
            {
                executeNode(node->SUB_STATEMENTS[0]);
                // Important: If this was a result statement, we should stop execution
                if (hasReturnValue())
                {
                    return; // Exit current function execution
                }
            }
        }
        else if (node->SUB_STATEMENTS.size() > 1)
        {
            executeNode(node->SUB_STATEMENTS[1]);
            // Important: If this was a result statement, we should stop execution
            if (hasReturnValue())
            {
                return; // Exit current function execution
            }
        }
        break;
    }
    case NODE_BOOL:
        if (node->CHILD)
        {
            Value value = evaluateExpression(node->CHILD);
            variables[node->VALUE] = value;
        }
        else
        {
            variables[node->VALUE] = Value(false);
        }
        break;
    case NODE_KEYWORD_INPUT:
        executeInputStatement(node);
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
    case NODE_OPERATOR_DECREMENT:
    {
        if (node->SUB_STATEMENTS.size() != 1)
        {
            std::cerr << "ERROR: decrement operator requires exactly one operand" << std::endl;
            exit(1);
        }

        // Get the operand (should be an identifier)
        AST_NODE *operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            std::cerr << "ERROR: decrement operator can only be applied to variables" << std::endl;
            exit(1);
        }

        // Get the variable name
        std::string varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            exit(1);
        }

        // Increment the value based on its type
        evaluateExpression(node); // This calls your existing increment logic in evaluateExpression
        break;
    }
    case NODE_OPERATOR_INCREMENT:
    {
        if (node->SUB_STATEMENTS.size() != 1)
        {
            std::cerr << "ERROR: Increment operator requires exactly one operand" << std::endl;
            exit(1);
        }

        // Get the operand (should be an identifier)
        AST_NODE *operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            std::cerr << "ERROR: Increment operator can only be applied to variables" << std::endl;
            exit(1);
        }

        // Get the variable name
        std::string varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            exit(1);
        }

        // Increment the value based on its type
        evaluateExpression(node); // This calls your existing increment logic in evaluateExpression
        break;
    }
    case NODE_IDENTIFIER:
    {
        std::string varName = node->VALUE;

        if (node->CHILD != nullptr)
        {
            Value result = evaluateExpression(node->CHILD);

            if (variables.find(node->VALUE) == variables.end())
            {
                std::cerr << "ERROR: Undefined Variable '" << node->VALUE << "'" << std::endl;
                exit(1);
            }

            variables[varName] = result;
        }
        else
        {
            // Just a variable reference
            if (variables.find(varName) == variables.end())
            {
                std::cerr << "ERROR: Undefined Variable '" << varName << "'" << std::endl;
                exit(1);
            }
        }
        break;
    }
    case NODE_NEWLINE:
        // Print a newline to both console and output file
        std::cout << std::endl;
        if (outputFile.is_open())
        {
            outputFile << std::endl;
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
    case NODE_BEGIN_BLOCK:
        for (auto &stmt : node->SUB_STATEMENTS)
        {
            executeNode(stmt);
        }
        break;
    case NODE_FUNCTION_CALL:
    {
        std::string funcName = node->VALUE;

        // Look up function definition
        AST_NODE *funcDef = findFunctionByName(funcName);
        if (!funcDef)
        {
            std::cerr << "Undefined function: " << funcName << std::endl;
            exit(1);
        }

        // Create a new scope for function parameters
        std::map<std::string, Value> localScope = variables; // Save current variables

        // Bind arguments to parameters
        AST_NODE *params = funcDef->SUB_STATEMENTS[0]; // First sub-statement is params

        // Make sure params exists and is the right type
        if (!params || params->TYPE != NODE_FUNCTION_PARAMS)
        {
            std::cerr << "Error: Function " << funcName << " has invalid parameter list" << std::endl;
            exit(1);
        }

        for (size_t i = 0; i < node->SUB_STATEMENTS.size() && i < params->SUB_STATEMENTS.size(); i++)
        {
            AST_NODE *paramNode = params->SUB_STATEMENTS[i];
            AST_NODE *argNode = node->SUB_STATEMENTS[i];

            // Evaluate argument
            Value argValue = evaluateExpression(argNode);

            // Bind to parameter name
            variables[paramNode->VALUE] = argValue;
        }

        // Clear the return value before executing
        returnValue = Value();

        // Execute function body
        executeNode(funcDef->CHILD);

        // Note: We don't need to use the return value here since this is not in an expression context
        // If in an expression context, evaluateExpression handles returning the value

        // Restore scope
        variables = localScope;

        break;
    }
    case NODE_FUNCTION_DECLERATION:
        break;
    case NODE_FUNCTION_BODY:
        // Execute all statements in the function body
        for (auto &stmt : node->SUB_STATEMENTS)
        {
            executeNode(stmt);

            if (hasReturnValue())
            {
                return;
            }
        }
        break;
    case NODE_DIVISION:
        evaluateExpression(node);
        break;
    case NODE_MODULUS:
        evaluateExpression(node);
        break;
    case NODE_SUBT:
        evaluateExpression(node);
        break;
    case NODE_NEWLINE_SYMBOL:
        if (outputFile.is_open())
        {
            outputFile << std::endl;
        }
        break;
    case NODE_NOT_EQUAL:
        evaluateExpression(node);
        break;
    case NODE_RESULTSTATEMENT:
        if (node->CHILD)
        {
            Value result = evaluateExpression(node->CHILD);
            setReturnValue(result);
        }
        else
        {
            std::cout << "EMPTY RESULT STATEMENT" << std::endl;
            setReturnValue(Value(0));
        }
        break;
    case NODE_LESS_EQUAL:
        evaluateExpression(node);
        break;
    default:
        std::cerr << "Interpretation Error: Unknown node type: " << getNodeTypeName(node->TYPE) << std::endl;
        exit(1);
    }
}

/**
 * @brief Finds a function by name in the AST
 *
 * @param name The name of the function to find
 * @return AST_NODE* Pointer to the function node, or nullptr if not found
 */
AST_NODE *Interpreter::findFunctionByName(const std::string &name)
{
    for (auto &stmt : root->SUB_STATEMENTS)
    {
        if (stmt->TYPE == NODE_FUNCTION_DECLERATION && stmt->VALUE == name)
        {
            return stmt;
        }
    }
    return nullptr;
}