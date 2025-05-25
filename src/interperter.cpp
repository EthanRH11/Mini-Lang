#include <iostream>
#include <map>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <variant>
#include <memory>
#include <random>
#include <cmath>

#include "interperter.hpp"
#include "Value.hpp"
#include "dynamic_array.hpp"
#include "ErrorHandler.hpp"
#include "parser.hpp"

namespace fs = std::filesystem;

void Interpreter::initializeInterperterMaps()
{
    nodeExecutors = {
        // type literals
        {NODE_INT_LITERAL, &Interpreter::evaluateIntLiteral},
        {NODE_DOUBLE_LITERAL, &Interpreter::evaluateDoubleLiteral},
        {NODE_CHAR_LITERAL, &Interpreter::evaluateCharLiteral},
        {NODE_STRING_LITERAL, &Interpreter::evaluateStringLiteral},
        {NODE_BOOL_LITERAL, &Interpreter::evaluateBoolLiteral},
        // Operators
        {NODE_ADD, &Interpreter::evaluateAdd},
        {NODE_SUBT, &Interpreter::evaluateSubt},
        {NODE_MULT, &Interpreter::evaluateMult},
        {NODE_DIVISION, &Interpreter::evaluateDiv},
        {NODE_MODULUS, &Interpreter::evaluateMod},
        {NODE_OPERATOR_DECREMENT, &Interpreter::evaluateDecrement},
        {NODE_OPERATOR_INCREMENT, &Interpreter::evaluateIncrement},
        // Input
        {NODE_KEYWORD_INPUT, &Interpreter::executeInputStatement},
        // Comparison Ops
        {NODE_NOT_EQUAL, &Interpreter::evaluateNotEqual},
        {NODE_LESS_THAN, &Interpreter::evaluateLessThan},
        {NODE_GREATER_THAN, &Interpreter::evaluateGreaterThan},
        {NODE_LESS_EQUAL, &Interpreter::evaluateLessEqual},
        // Newline
        {NODE_NEWLINE, &Interpreter::evaluateNewLine},
        // Array Stuff
        {NODE_ARRAY_DECLARATION, &Interpreter::evaluateArrayDecleration},
        {NODE_ARRAY_REPEAT, &Interpreter::evaluateArrayRepeat},
        {NODE_ARRAY_LENGTH, &Interpreter::evaluateArrayLength},
        {NODE_ARRAY_ACCESS, &Interpreter::evaluateArrayAccess},
        {NODE_ARRAY_ASSIGN, &Interpreter::evaluateArrayAssign},
        {NODE_ARRAY_INIT, &Interpreter::evaluateArrayInit},
        {NODE_ARRAY_RANGE, &Interpreter::evaluateArrayRange},
        {NODE_ARRAY_INSERT, &Interpreter::evaluateArrayInsert},
        {NODE_ARRAY_REMOVE, &Interpreter::evaluateArrayRemove},
        {NODE_DOT, &Interpreter::evaluateArrayIndexMod},
        {NODE_ARRAY_SORT_ASC, &Interpreter::evaluateArraySortAsc},
        {NODE_ARRAY_SORT_DESC, &Interpreter::evaluateArraySortDesc},

        {NODE_FUNCTION_CALL, &Interpreter::evaluateFunctionCall},
        {NODE_PAREN_EXPR, &Interpreter::evaluateParenExpr},
    };

    standardLib = {
        {"randomInt", &Interpreter::evaluateRandomInt},
        {"coinFlip", &Interpreter::evaluateCoinFlip},
        {"diceRoll", &Interpreter::evaluateDiceRoll},
        {"generatePin", &Interpreter::evaluateGeneratePin},
        {"sqrt", &Interpreter::evaluateSQRT},
        {"abs", &Interpreter::evaluateABS},
        {"pow", &Interpreter::evaluatePOW},
        {"min", &Interpreter::evaluateMIN},
        {"max", &Interpreter::evaluateMAX},
        {"ceil", &Interpreter::evaluateCEIL},
        {"floor", &Interpreter::evaluateFLOOR},
    };
}

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
        // std::cerr << "Error: No 'begin' block found in program." << std::endl;
        // exit(1);
        ErrorHandler::getInstance().reportSemanticError("No 'begin' block found in program.");
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

    auto it = nodeExecutors.find(node->TYPE);
    if (it != nodeExecutors.end())
    {
        return (this->*(it->second))(node);
    }

    if (node->TYPE == NODE_IDENTIFIER)
    {
        auto varIt = variables.find(node->VALUE);
        if (varIt != variables.end())
        {
            return varIt->second;
        }
        ErrorHandler::getInstance().reportSemanticError("Undefined variables: '" + node->VALUE + "'");
        return Value(0);
    }

    ErrorHandler::getInstance().reportSemanticError("Unexpected expression of type: '" + getNodeTypeName(node->TYPE) + "'");
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

    // std::cout << "Executing statement: " << getNodeTypeName(node->TYPE) << std::endl;

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
            variables[node->VALUE] = Value('\0');
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
    case NODE_ARRAY_DECLARATION:
    {
        std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
        variables[node->VALUE] = Value(array);
    }
    break;
    case NODE_SEMICOLON:
        break;
    default:
        // std::cerr << "Unknown Statement Type: " << getNodeTypeName(node->TYPE) << std::endl;
        // exit(1);
        ErrorHandler::getInstance().reportSemanticError("Unknown Statement Type: " + getNodeTypeName(node->TYPE));
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

    std::cout << "DEBUG: executeNode called with node type: " << getNodeTypeName(node->TYPE) << std::endl;
    if (!node->VALUE.empty())
    {
        std::cout << "DEBUG: Node value: " << node->VALUE << std::endl;
    }
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

        if (condition.isInt())
        {
            conditionResult = condition.asInt() != 0;
        }
        else if (condition.isDouble())
        {
            conditionResult = condition.asDouble() != 0.0;
        }
        else if (condition.isBool())
        {
            conditionResult = condition.asBool();
        }
        else if (condition.isString())
        {
            conditionResult = !condition.asString().empty();
        }
        else if (condition.isChar())
        {
            conditionResult = condition.asChar() != '\0';
        }
        // std::cout << "IF condition evaluated to: " << (conditionResult ? "true" : "false") << std::endl;

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
    case NODE_CHECK:
    {
        while (true)
        {
            Value condition = evaluateExpression(node->CHILD);

            bool continueLoop = false;
            if (condition.isInt())
            {
                continueLoop = condition.asInt() != 0;
            }
            else if (condition.isDouble())
            {
                continueLoop = condition.asDouble() != 0.0;
            }
            else if (condition.isBool())
            {
                continueLoop = condition.asBool();
            }

            if (!continueLoop)
            {
                break;
            }
            if (!node->SUB_STATEMENTS.empty())
            {
                executeNode(node->SUB_STATEMENTS[0]);
            }
        }
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
    case NODE_ELEMENT_TYPE:
        break;
    case NODE_ARRAY_DECLARATION:
    {
        std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
        variables[node->VALUE] = Value(array);
    }
    break;
    case NODE_OPERATOR_DECREMENT:
    {
        if (node->SUB_STATEMENTS.size() != 1)
        {
            // std::cerr << "ERROR: decrement operator requires exactly one operand" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Decrement operator requires exactly one operand.");
        }

        // Get the operand (should be an identifier)
        AST_NODE *operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            // std::cerr << "ERROR: decrement operator can only be applied to variables" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Decrement operator can only be applied to variables.");
        }

        // Get the variable name
        std::string varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            // std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Undefined variable '" + varName + "'");
        }

        // Decrement the value based on its type
        evaluateExpression(node); // This calls your existing decrement logic in evaluateExpression
        break;
    }
    case NODE_OPERATOR_INCREMENT:
    {
        if (node->SUB_STATEMENTS.size() != 1)
        {
            // std::cerr << "ERROR: Increment operator requires exactly one operand" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Increment operator requires exactly one operand.");
        }

        // Get the operand (should be an identifier)
        AST_NODE *operand = node->SUB_STATEMENTS[0];
        if (operand->TYPE != NODE_IDENTIFIER)
        {
            // std::cerr << "ERROR: Increment operator can only be applied to variables" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Increment operator can only be applied to variables.");
        }

        // Get the variable name
        std::string varName = operand->VALUE;
        if (variables.find(varName) == variables.end())
        {
            // std::cerr << "ERROR: Undefined variable '" << varName << "'" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Undefined variable: '" + varName + "'");
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
                // std::cerr << "ERROR: Undefined Variable '" << node->VALUE << "'" << std::endl;
                // exit(1);
                ErrorHandler::getInstance().reportSemanticError("Undefined variable '" + node->VALUE + "'");
            }

            variables[varName] = result;
        }
        else
        {
            // Just a variable reference
            if (variables.find(varName) == variables.end())
            {
                // std::cerr << "ERROR: Undefined Variable '" << varName << "'" << std::endl;
                // exit(1);
                ErrorHandler::getInstance().reportSemanticError("Undefined variable: '" + varName + "'");
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
    case NODE_ARRAY_ACCESS:
    {
    }
    break;
    case NODE_ARRAY_ASSIGN:
    {
        std::string arrayName = node->VALUE;
        if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
        {
            // std::cerr << "Error: " << arrayName << " is not an array" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        }

        int index = evaluateExpression(node->SUB_STATEMENTS[0]).asInt();
        Value value = evaluateExpression(node->SUB_STATEMENTS[1]);
        auto array = variables[arrayName].asArray();

        try
        {
            array->setElement(index, value);
        }
        catch (const std::out_of_range &e)
        {
            // std::cerr << "Error: Array index out of bounds" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Array index out of bounds.");
        }
        break;
    }
    case NODE_ARRAY_INIT:
    {
        std::string arrayName = node->VALUE;
        std::vector<Value> values;

        for (auto &stmt : node->SUB_STATEMENTS)
        {
            values.push_back(evaluateExpression(stmt));
        }

        std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>(values);
        variables[arrayName] = Value(array);
        break;
    }
    case NODE_ARRAY_RANGE:
    {
        std::string arrayName = node->VALUE;

        int start = evaluateExpression(node->CHILD).asInt();
        int end = evaluateExpression(node->SUB_STATEMENTS[0]).asInt();

        std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
        array->initializeRange(start, end);

        variables[arrayName] = Value(array);
        break;
    }
    case NODE_ARRAY_REPEAT:
    {
        std::string arrayName = node->VALUE;
        if (node->SUB_STATEMENTS.size() != 2)
        {
            // std::cerr << "Error: Repeat requires value and count" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Repeat requires value and count.");
        }

        Value value = evaluateExpression(node->SUB_STATEMENTS[0]);
        int count = evaluateExpression(node->SUB_STATEMENTS[1]).asInt();

        std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
        array->initializeRepeat(value, count);

        variables[arrayName] = Value(array);
        break;
    }
    case NODE_ARRAY_LENGTH:
    {
        // Already handled in evaluateExpression
        evaluateExpression(node);
        break;
    }
    case NODE_ARRAY_INSERT:
    {
        std::string arrayName = node->VALUE;
        if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
        {
            // std::cerr << "Error: " << arrayName << " is not an array" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        }

        // FIX: index is in node->CHILD, not SUB_STATEMENTS[0]
        int index = evaluateExpression(node->CHILD).asInt();
        Value value = evaluateExpression(node->SUB_STATEMENTS[0]);
        auto array = variables[arrayName].asArray();

        try
        {
            array->insertElement(index, value);
        }
        catch (const std::out_of_range &e)
        {
            // std::cerr << "Error: Invalid array index for insertion" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Invalid array index for insertion.");
        }
        break;
    }
    case NODE_ARRAY_REMOVE:
    {
        std::string arrayName = node->VALUE;
        if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
        {
            // std::cerr << "Error: " << arrayName << " is not an array" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        }

        int index = evaluateExpression(node->CHILD).asInt();
        auto array = variables[arrayName].asArray();

        try
        {
            array->removeElement(index);
        }
        catch (const std::out_of_range &e)
        {
            // std::cerr << "Error: Array index out of bounds" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Array index out of bounds.");
        }
        break;
    }
    case NODE_ARRAY_SORT_ASC:
    {
        std::string arrayName = node->VALUE;
        if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
        {
            // std::cerr << "Error: " << arrayName << " is not an array" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        }

        auto array = variables[arrayName].asArray();
        array->sortAscending();
        break;
    }
    case NODE_ARRAY_SORT_DESC:
    {
        std::string arrayName = node->VALUE;
        if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
        {
            // std::cerr << "Error: " << arrayName << " is not an array" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        }

        auto array = variables[arrayName].asArray();
        array->sortDescending();
        break;
    }
    case NODE_ARRAY_LAST_INDEX:
    {
        // Already handled in evaluateExpression
        evaluateExpression(node);
        break;
    }
    case NODE_FOR:
    {
        AST_NODE *args = node->CHILD;
        if (!args || args->TYPE != NODE_FOR_ARGS || args->SUB_STATEMENTS.size() != 3)
        {
            // std::cerr << "Error: few too many arguments for loop structure";
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Few too many arguments for loop structure.");
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
                if (condResult.isInt())
                {
                    continueLoop = condResult.asInt() != 0;
                }
                else if (condResult.isDouble())
                {
                    continueLoop = condResult.asDouble() != 0.0;
                }
                else if (condResult.isBool())
                {
                    continueLoop = condResult.asBool();
                }
                else if (condResult.isString())
                {
                    continueLoop = !condResult.asString().empty();
                }
                else if (condResult.isChar())
                {
                    continueLoop = condResult.asChar() != '\0';
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
    case NODE_DOT:
    {
        evaluateExpression(node);
        break;
    }
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
            // std::cerr << "Undefined function: " << funcName << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Undefined function: '" + funcName + "'");
        }

        // Create a new scope for function parameters
        std::map<std::string, Value> localScope = variables; // Save current variables

        // Bind arguments to parameters
        AST_NODE *params = funcDef->SUB_STATEMENTS[0]; // First sub-statement is params

        // Make sure params exists and is the right type
        if (!params || params->TYPE != NODE_FUNCTION_PARAMS)
        {
            // std::cerr << "Error: Function " << funcName << " has invalid parameter list" << std::endl;
            // exit(1);
            ErrorHandler::getInstance().reportSemanticError("Function: '" + funcName + "' has invalid parameter list.");
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
        // std::cerr << "Interpretation Error: Unknown node type: " << getNodeTypeName(node->TYPE) << std::endl;
        // exit(1);
        ErrorHandler::getInstance().reportSemanticError("Unknown node type: '" + getNodeTypeName(node->TYPE) + "'");
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
    // Helper function for recursive search
    std::function<AST_NODE *(AST_NODE *)> searchNodeForFunction =
        [&](AST_NODE *node) -> AST_NODE *
    {
        if (!node)
            return nullptr;

        // Check if this node is the function we're looking for
        if (node->TYPE == NODE_FUNCTION_DECLERATION && node->VALUE == name)
        {
            return node;
        }

        // Search in sub-statements
        for (auto &subNode : node->SUB_STATEMENTS)
        {
            // For header inclusions, check their children too
            if (subNode->TYPE == NODE_READ_HEADER && subNode->CHILD)
            {
                AST_NODE *headerRoot = subNode->CHILD;
                AST_NODE *result = searchNodeForFunction(headerRoot);
                if (result)
                    return result;
            }
            else
            {
                // Direct check for function in current sub-statement
                if (subNode->TYPE == NODE_FUNCTION_DECLERATION && subNode->VALUE == name)
                {
                    return subNode;
                }

                // Recursively search this sub-statement
                AST_NODE *result = searchNodeForFunction(subNode);
                if (result)
                    return result;
            }
        }

        // Check child node if it exists
        if (node->CHILD)
        {
            return searchNodeForFunction(node->CHILD);
        }

        return nullptr;
    };

    // Start the search from the root
    return searchNodeForFunction(root);
}

void Interpreter::evaluateImport(AST_NODE *node)
{
    std::string libraryName = node->VALUE;
    LibraryManager &libraryManager = LibraryManager::getInstance();

    if (libraryManager.isLibraryLoaded(libraryName))
    {
        return; // already loaded do nothing
    }

    if (libraryName == "random")
    {
        AST_NODE *randomLibraryAST = libraryManager.generateRandomAST();

        libraryManager.loadPreCompiledLibrary(libraryName, randomLibraryAST);

        return;
    }
    else if (libraryName == "Math")
    {
        AST_NODE *mathLibraryAST = libraryManager.generateMathAST();

        libraryManager.loadPreCompiledLibrary(libraryName, mathLibraryAST);
    }

    if (!libraryManager.loadLibrary(libraryName))
    {
        ErrorHandler::getInstance().reportRuntimeError("Failed to load library: " + libraryName);
    }
}

Value Interpreter::evaluateRandomInt(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() < 2)
    {
        ErrorHandler::getInstance().reportRuntimeError("randomInt requires two arguments: min and max.");
        return Value(0);
    }

    Value minVal = evaluateExpression(node->SUB_STATEMENTS[0]);
    Value maxVal = evaluateExpression(node->SUB_STATEMENTS[1]);

    int min = minVal.asInt();
    int max = maxVal.asInt();

    if (min > max)
    {
        ErrorHandler::getInstance().reportRuntimeError("randomInt: min must be less than or equal to max.");
        std::swap(min, max);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    int result = distrib(gen);
    return Value(result);
}

Value Interpreter::evaluateCoinFlip([[maybe_unused]] AST_NODE *node)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    bool result = (distrib(gen) == 1);
    return Value(result); // true or false;
}

Value Interpreter::evaluateDiceRoll(AST_NODE *node)
{
    int sides = 6;

    if (!node->SUB_STATEMENTS.empty())
    {
        Value sidesVal = evaluateExpression(node->SUB_STATEMENTS[0]);
        sides = sidesVal.asInt();

        if (sides < 6)
        {
            ErrorHandler::getInstance().reportRuntimeError("diceRoll: Minimum number of sides is 6.");
            sides = 6;
        }
        else if (sides > 20)
        {
            ErrorHandler::getInstance().reportRuntimeError("diceRoll: Maximum number of sides is 20.");
            sides = 20;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, sides);

    int result = distrib(gen);
    return Value(result);
}

Value Interpreter::evaluateGeneratePin(AST_NODE *node)
{
    int digits = 4;

    if (!node->SUB_STATEMENTS.empty())
    {
        Value digitsVal = evaluateExpression(node->SUB_STATEMENTS[0]);
        digits = digitsVal.asInt();

        if (digits < 1)
        {
            ErrorHandler::getInstance().reportRuntimeError("generatePin: Minimum number of digits is 1.");
            digits = 1;
        }
        else if (digits > 100)
        {
            ErrorHandler::getInstance().reportRuntimeError("generatePin: Maximum number of digits is 100.");
            digits = 100;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9);

    std::string pin;
    for (int i = 0; i < digits; i++)
    {
        pin += std::to_string(distrib(gen));
    }

    return Value(pin);
}

Value Interpreter::evaluateABS(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.empty())
    {
        ErrorHandler::getInstance().reportRuntimeError("abs: Must have value to evaluate absolute.");
        return Value(0);
    }

    Value absVal = evaluateExpression(node->SUB_STATEMENTS[0]);

    if (absVal.isNumeric())
    {
        return Value(std::abs(absVal.asDoubleSafe()));
    }
    else
    {
        std::stringstream errorMsg;
        ErrorHandler::getInstance().reportRuntimeError("abs: Expected numeric value.");
        return Value(0);
    }
}
Value Interpreter::evaluateSQRT(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.empty())
    {
        ErrorHandler::getInstance().reportRuntimeError("sqrt: Must have value to evaluate square root.");
        return Value(0);
    }

    Value sqrtVal = evaluateExpression(node->SUB_STATEMENTS[0]);

    if (sqrtVal.isNumeric())
    {
        return Value(std::sqrt(sqrtVal.asDoubleSafe()));
    }
    else
    {
        ErrorHandler::getInstance().reportRuntimeError("sqrt: Expected numerical value.");
        return Value(0);
    }
}
Value Interpreter::evaluatePOW(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() != 2)
    {
        ErrorHandler::getInstance().reportRuntimeError("pow: Expected two values to evaluate power.");
        return Value(0);
    }

    Value base = evaluateExpression(node->SUB_STATEMENTS[0]);
    Value exponent = evaluateExpression(node->SUB_STATEMENTS[1]);

    if (base.isNumeric() && exponent.isNumeric())
    {
        return Value(std::pow(base.asDoubleSafe(), exponent.asDoubleSafe()));
    }
    else
    {
        ErrorHandler::getInstance().reportRuntimeError("pow: Expected numerical values.");
        return Value(0);
    }
}
Value Interpreter::evaluateMIN(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() != 2)
    {
        ErrorHandler::getInstance().reportRuntimeError("min: Expected two values to compare.");
        return Value(0);
    }

    Value a = evaluateExpression(node->SUB_STATEMENTS[0]);
    Value b = evaluateExpression(node->SUB_STATEMENTS[1]);

    if (a.isNumeric() && b.isNumeric())
    {
        return Value(std::min(a.asDoubleSafe(), b.asDoubleSafe()));
    }
    else
    {
        ErrorHandler::getInstance().reportRuntimeError("min: Expected numerical values.");
        return Value(0);
    }
}
Value Interpreter::evaluateMAX(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() != 2)
    {
        ErrorHandler::getInstance().reportRuntimeError("max: Expected two values to evaluate max.");
        return Value(0);
    }

    Value a = evaluateExpression(node->SUB_STATEMENTS[0]);
    Value b = evaluateExpression(node->SUB_STATEMENTS[1]);

    if (a.isNumeric() && b.isNumeric())
    {
        return Value(std::max(a.asDoubleSafe(), b.asDoubleSafe()));
    }
    else
    {
        ErrorHandler::getInstance().reportRuntimeError("max: Expected two numerical values.");
        return Value(0);
    }
}

Value Interpreter::evaluateCEIL(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.empty())
    {
        ErrorHandler::getInstance().reportRuntimeError("ceil: Expeceted a numerical value for param.");
        return Value(0);
    }

    Value value = evaluateExpression(node->SUB_STATEMENTS[0]);

    if (value.isNumeric())
    {
        return Value(std::ceil(value.asDoubleSafe()));
    }
    else
    {
        ErrorHandler::getInstance().reportRuntimeError("ceil: Expected a numeric value to calculate ceiling.");
        return Value(0);
    }
}
Value Interpreter::evaluateFLOOR(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.empty())
    {
        ErrorHandler::getInstance().reportRuntimeError("floor: Expeceted a numerical value for param.");
        return Value(0);
    }

    Value value = evaluateExpression(node->SUB_STATEMENTS[0]);

    if (value.isNumeric())
    {
        return Value(std::floor(value.asDoubleSafe()));
    }
    else
    {
        ErrorHandler::getInstance().reportRuntimeError("floor: Expected a numeric value to calculate floor.");
        return Value(0);
    }
}

// Type Literals
Value Interpreter::evaluateIntLiteral(AST_NODE *node) { return Value(std::stoi(node->VALUE)); }
Value Interpreter::evaluateDoubleLiteral(AST_NODE *node) { return Value(std::stod(node->VALUE)); }
Value Interpreter::evaluateBoolLiteral(AST_NODE *node) { return node->VALUE == "true" ? Value(true) : Value(false); }
Value Interpreter::evaluateCharLiteral(AST_NODE *node) { return node->VALUE.length() == 1 ? Value(node->VALUE[0]) : Value('\0'); }
Value Interpreter::evaluateStringLiteral(AST_NODE *node) { return Value(node->VALUE); }
// Operator (+, -, /) Functions
Value Interpreter::evaluateAdd(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        return left + right;
    }
    return Value(0);
}
Value Interpreter::evaluateSubt(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() == 1)
    {
        Value operand = evaluateExpression(node->SUB_STATEMENTS[0]);

        if (operand.isNumeric())
        {
            return operand.isInt() ? Value(-operand.asInt()) : Value(-operand.asDouble());
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot negate non-numeric value.");
            return Value(0);
        }
    }
    else if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();

            return Value(leftVal - rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot perform subtraction on non numeric values.");
            return Value(0);
        }
    }
    return Value(0);
}
Value Interpreter::evaluateMult(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();

            return Value(leftVal * rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot perform multiplication on non-numeric values.");
            return Value(0);
        }
    }
    return Value(0);
}
Value Interpreter::evaluateDiv(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();

            if (rightVal == 0)
            {
                ErrorHandler::getInstance().reportSemanticError("Division by zero is not allowed.");
                return Value(0);
            }

            return Value(leftVal / rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot perform division on non-numeric values.");
            return Value(0);
        }
    }
    return Value(0);
}
Value Interpreter::evaluateMod(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isInt() && right.isInt())
        {
            if (right.asInt() == 0)
            {
                ErrorHandler::getInstance().reportSemanticError("Modulus by zero is not allowed.");
                return Value(0);
            }
            return Value(left.asInt() % right.asInt());
        }
    }
    return Value(0);
}
Value Interpreter::evaluateDecrement(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() != 1)
    {
        ErrorHandler::getInstance().reportSemanticError("Decrement operator requires one operand.");
        return Value(0);
    }
    AST_NODE *operand = node->SUB_STATEMENTS[0];
    if (operand->TYPE != NODE_IDENTIFIER)
    {
        ErrorHandler::getInstance().reportSemanticError("Decrement operator can only be performed on variables.");
        return Value(0);
    }

    std::string varName = operand->VALUE;
    if (variables.find(varName) == variables.end())
    {
        ErrorHandler::getInstance().reportSemanticError("Undefined variable '" + varName + "'");
        return Value(0);
    }

    Value *valueptr = &variables[varName];
    if (valueptr->isInt())
    {
        int newValue = valueptr->asInt() - 1;
        *valueptr = Value(newValue);
        return *valueptr;
    }
    else if (valueptr->isDouble())
    {
        double newValue = valueptr->asDouble() - 1.0;
        *valueptr = Value(newValue);
        return *valueptr;
    }
    else if (valueptr->isChar())
    {
        char newValue = valueptr->asChar() - 1;
        *valueptr = Value(newValue);
        return *valueptr;
    }
    else
    {
        ErrorHandler::getInstance().reportSemanticError("Decrement operator not supported for this type.");
        return Value(0);
    }
}
Value Interpreter::evaluateIncrement(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() != 1)
    {
        ErrorHandler::getInstance().reportSemanticError("Increment operator requires exactly one operand.");
        return Value(0);
    }

    AST_NODE *operand = node->SUB_STATEMENTS[0];
    if (operand->TYPE != NODE_IDENTIFIER)
    {
        ErrorHandler::getInstance().reportSemanticError("Increment operator can only be applied to variables.");
        return Value(0);
    }

    std::string varName = operand->VALUE;
    if (variables.find(varName) == variables.end())
    {
        ErrorHandler::getInstance().reportSemanticError("Undefined variable: '" + varName + "'");
        return Value(0);
    }

    Value *valuePtr = &variables[varName];
    if (valuePtr->isInt())
    {
        int newValue = valuePtr->asInt() + 1;
        *valuePtr = Value(newValue);
        return *valuePtr;
    }
    else if (valuePtr->isDouble())
    {
        double newValue = valuePtr->asDouble() + 1.0;
        *valuePtr = Value(newValue);
        return *valuePtr;
    }
    else if (valuePtr->isChar())
    {
        char newValue = valuePtr->asChar() + 1;
        *valuePtr = Value(newValue);
        return *valuePtr;
    }
    else
    {
        ErrorHandler::getInstance().reportSemanticError("Increment operator not supported for this type.");
        return Value(0);
    }
}
// Comparison Ops
Value Interpreter::evaluateNotEqual(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();

            return Value(leftVal != rightVal);
        }
        else if (left.isString() && right.isString())
        {
            return Value(left.asString() != right.asString());
        }
        else
        {
            return Value(left.toString() != right.toString());
        }
    }
    return Value(false);
}
Value Interpreter::evaluateLessThan(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();

            return Value(leftVal < rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot compare non-numeric values.");
            return Value(false);
        }
    }
    return Value(false);
}
Value Interpreter::evaluateGreaterThan(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();
            return Value(leftVal > rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot compare non-numeric values.");
            return Value(false);
        }
    }
    return Value(false);
}
Value Interpreter::evaluateLessEqual(AST_NODE *node)
{
    if (node->SUB_STATEMENTS.size() >= 2)
    {
        Value left = evaluateExpression(node->SUB_STATEMENTS[0]);
        Value right = evaluateExpression(node->SUB_STATEMENTS[1]);

        if (left.isNumeric() && right.isNumeric())
        {
            double leftVal = left.isInt() ? left.asInt() : left.asDouble();
            double rightVal = right.isInt() ? right.asInt() : right.asDouble();
            return Value(leftVal <= rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot compare non-numeric values.");
            return Value(false);
        }
    }
    return Value(false);
}
// Newline
Value Interpreter::evaluateNewLine([[maybe_unused]] AST_NODE *node)
{
    return Value('\n');
}
// Array stuff
Value Interpreter::evaluateArrayDecleration([[maybe_unused]] AST_NODE *node)
{
    std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
    return Value(array);
}
Value Interpreter::evaluateArrayRepeat(AST_NODE *node)
{
    Value element = evaluateExpression(node->CHILD);
    int count = std::stoi(node->SUB_STATEMENTS[0]->VALUE);

    std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
    array->initializeRepeat(element, count);

    return Value(array);
}
Value Interpreter::evaluateArrayLength(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }

    auto array = variables[arrayName].asArray();
    return Value(static_cast<int>(array->getLength()));
}
Value Interpreter::evaluateArrayAccess(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (!variables.count(arrayName) || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }
    auto arr = variables[arrayName].asArray();

    // if child is a literal index:
    if (node->CHILD->TYPE != NODE_ARRAY_LAST_INDEX)
    {
        int idx = evaluateExpression(node->CHILD).asInt();
        return arr->getElement(idx);
    }
    // else child is last-element marker:
    if (arr->getLength() == 0)
    {
        ErrorHandler::getInstance().reportSemanticError("Cannot get last element of an empty array.");
        return Value(0);
    }
    return arr->getLastElement();
}
Value Interpreter::evaluateArrayAssign(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }

    int index = evaluateExpression(node->SUB_STATEMENTS[0]).asInt();
    Value value = evaluateExpression(node->SUB_STATEMENTS[1]);
    auto array = variables[arrayName].asArray();

    try
    {
        array->setElement(index, value);
        return value;
    }
    catch (const std::out_of_range &e)
    {
        ErrorHandler::getInstance().reportSemanticError("Array index out of bounds.");
        return Value(0);
    }
}
Value Interpreter::evaluateArrayInit(AST_NODE *node)
{
    std::vector<Value> values;
    for (auto &stmt : node->SUB_STATEMENTS)
    {
        values.push_back(evaluateExpression(stmt));
    }

    std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>(values);
    return Value(array);
}
Value Interpreter::evaluateArrayRange(AST_NODE *node)
{
    int start = evaluateExpression(node->CHILD).asInt();
    int end = evaluateExpression(node->SUB_STATEMENTS[0]).asInt();

    std::shared_ptr<DynamicArray> array = std::make_shared<DynamicArray>();
    array->initializeRange(start, end);

    return Value(array);
}
Value Interpreter::evaluateArrayInsert(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }

    int index = evaluateExpression(node->SUB_STATEMENTS[0]).asInt();
    Value value = evaluateExpression(node->SUB_STATEMENTS[1]);
    auto array = variables[arrayName].asArray();

    try
    {
        array->insertElement(index, value);
        return value;
    }
    catch (const std::out_of_range &e)
    {
        ErrorHandler::getInstance().reportSemanticError("Invalid array index for insertion.");
        return Value(0);
    }
}
Value Interpreter::evaluateArrayRemove(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }

    int index = evaluateExpression(node->CHILD).asInt();
    auto array = variables[arrayName].asArray();

    try
    {
        Value removed = array->getElement(index);
        array->removeElement(index);
        return removed;
    }
    catch (const std::out_of_range &e)
    {
        ErrorHandler::getInstance().reportSemanticError("Array index out of bounds.");
        return Value(0);
    }
}
Value Interpreter::evaluateArrayIndexMod(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (!variables.count(arrayName) || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
    }

    auto array = variables[arrayName].asArray();

    if (!node || node->CHILD->TYPE != NODE_ARRAY_INDEX)
    {
        ErrorHandler::getInstance().reportSemanticError("Invalid dot expression structure.");
    }

    int index = std::stoi(node->CHILD->VALUE);

    if (index < 0 || static_cast<size_t>(index) >= array->getLength())
    {
        ErrorHandler::getInstance().reportSemanticError("Array index out of bounds: " + index);
    }

    Value currentValueOfIndex = array->getElement(index);

    if (!node->CHILD->CHILD)
    {
        ErrorHandler::getInstance().reportSemanticError("Missing operator in dot expression.");
    }

    Value operandValue = Value(std::stoi(node->CHILD->CHILD->CHILD->VALUE));

    Value resultOfExpression;

    switch (node->CHILD->CHILD->TYPE)
    {
    case NODE_ADD:
        resultOfExpression = currentValueOfIndex + operandValue;
        break;
    case NODE_MULT:
        if (currentValueOfIndex.isInt() && operandValue.isInt())
        {
            resultOfExpression = Value(currentValueOfIndex.asInt() * operandValue.asInt());
        }
        else if ((currentValueOfIndex.isInt() || currentValueOfIndex.isDouble()) &&
                 (operandValue.isInt() || operandValue.isDouble()))
        {
            double leftVal = currentValueOfIndex.isInt() ? currentValueOfIndex.asInt() : currentValueOfIndex.asDouble();
            double rightVal = operandValue.isInt() ? operandValue.asInt() : operandValue.asDouble();
            resultOfExpression = Value(leftVal * rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot multiply non-numeric values.");
        }
        break;
    case NODE_DIVISION:
        if ((operandValue.isInt() && operandValue.asInt() == 0) ||
            (operandValue.isDouble() && operandValue.asDouble() == 0.0))
        {
            ErrorHandler::getInstance().reportSemanticError("Division by zero.");
        }

        if (currentValueOfIndex.isInt() && operandValue.isInt())
        {
            resultOfExpression = Value(currentValueOfIndex.asInt() / operandValue.asInt());
        }
        else if ((currentValueOfIndex.isInt() || currentValueOfIndex.isDouble()) &&
                 (operandValue.isInt() || operandValue.isDouble()))
        {
            double leftVal = currentValueOfIndex.isInt() ? currentValueOfIndex.asInt() : currentValueOfIndex.asDouble();
            double rightVal = operandValue.isInt() ? operandValue.asInt() : operandValue.asDouble();
            resultOfExpression = Value(leftVal / rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot divide non-numeric values.");
        }
        break;
    case NODE_SUBT:
        if (currentValueOfIndex.isInt() && operandValue.isInt())
        {
            resultOfExpression = Value(currentValueOfIndex.asInt() - operandValue.asInt());
        }
        else if ((currentValueOfIndex.isInt() || currentValueOfIndex.isDouble()) &&
                 (operandValue.isInt() || operandValue.isDouble()))
        {
            double leftVal = currentValueOfIndex.isInt() ? currentValueOfIndex.asInt() : currentValueOfIndex.asDouble();
            double rightVal = operandValue.isInt() ? operandValue.asInt() : operandValue.asDouble();
            resultOfExpression = Value(leftVal - rightVal);
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Cannot subtract non-numeric values.");
        }
        break;
    case NODE_MODULUS:
        if (operandValue.isInt() && operandValue.asInt() == 0)
        {
            ErrorHandler::getInstance().reportSemanticError("Modulus by zero");
        }

        if (currentValueOfIndex.isInt() && operandValue.isInt())
        {
            resultOfExpression = Value(currentValueOfIndex.asInt() % operandValue.asInt());
        }
        else
        {
            ErrorHandler::getInstance().reportSemanticError("Modulus requires integer operands.");
        }
        break;
    default:
        ErrorHandler::getInstance().reportSemanticError("Unknown operator in dot expression.");
    }

    array->setElement(index, resultOfExpression);
    return resultOfExpression;
} // NODE_DOT
Value Interpreter::evaluateArraySortAsc(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }

    auto array = variables[arrayName].asArray();
    array->sortAscending();
    return variables[arrayName];
}
Value Interpreter::evaluateArraySortDesc(AST_NODE *node)
{
    std::string arrayName = node->VALUE;
    if (variables.find(arrayName) == variables.end() || !variables[arrayName].isArray())
    {
        ErrorHandler::getInstance().reportSemanticError(arrayName + " is not an array.");
        return Value(0);
    }

    auto array = variables[arrayName].asArray();
    array->sortDescending();
    return variables[arrayName];
}

// Other stuff
Value Interpreter::evaluateFunctionCall(AST_NODE *node)
{
    static int recursionDepth = 0;
    recursionDepth++;

    std::string funcName = node->VALUE;
    // std::cout << "Entering Function: " << funcName << " depth: " << recursionDepth << std::endl;
    std::string callID = generateCallID(funcName, recursionDepth);
    // ** Handle random library functions first ** //
    if (funcName == "randomInt")
    {
        Value result = evaluateRandomInt(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "coinFlip")
    {
        Value result = evaluateCoinFlip(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "diceRoll")
    {
        Value result = evaluateDiceRoll(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "generatePin")
    {
        Value result = evaluateGeneratePin(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "sqrt")
    {
        Value result = evaluateSQRT(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "abs")
    {
        Value result = evaluateABS(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "pow")
    {
        Value result = evaluatePOW(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "min")
    {
        Value result = evaluateMIN(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "max")
    {
        Value result = evaluateMAX(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "ceil")
    {
        Value result = evaluateCEIL(node);
        recursionDepth--;
        return result;
    }
    else if (funcName == "floor")
    {
        Value result = evaluateFLOOR(node);
        recursionDepth--;
        return result;
    }

    // Look up function definition
    AST_NODE *funcDef = findFunctionByName(funcName);
    if (!funcDef)
    {
        // std::cerr << "Undefined function: " << funcName << std::endl;
        // exit(1);
        ErrorHandler::getInstance().reportSemanticError("Undefined function: " + funcName);
    }

    // Save the current state of all variables
    std::map<std::string, Value> oldVariables = variables;

    // Bind arguments to parameters
    AST_NODE *params = funcDef->SUB_STATEMENTS[0];
    for (size_t i = 0; i < node->SUB_STATEMENTS.size() && i < params->SUB_STATEMENTS.size(); i++)
    {
        AST_NODE *paramNode = params->SUB_STATEMENTS[i];
        AST_NODE *argNode = node->SUB_STATEMENTS[i];

        // Evaluate argument and bind to parameter
        Value argValue = evaluateExpression(argNode);
        variables[paramNode->VALUE] = argValue;
    }

    // Save the current return value
    Value oldReturnValue = returnValue;
    // Clear for this function call
    returnValue = Value();

    // Execute function body
    executeNode(funcDef->CHILD);

    // Get this function's return value
    Value result = returnValue;

    // IMPORTANT: Print debug info to see what each recursive call is returning
    // std::cout << "Function " << funcName << " at depth " << recursionDepth
    //           << " returning: " << result.toString() << std::endl;

    // Restore the previous state
    variables = oldVariables;
    returnValue = oldReturnValue;

    recursionDepth--;

    // Return this function's result
    return result;
}
