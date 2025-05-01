#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <chrono>
#include <ctime>
#include <variant>
#include <fstream>
#include <stack>
#include <algorithm> // For std::transform

#include "parser.hpp"
#include "Value.hpp"

/**
 * @class Interpreter
 * @brief Executes the abstract syntax tree produced by the parser
 *
 * The Interpreter class traverses the AST and performs the operations
 * specified by the language. It maintains a symbol table for variables
 * and handles function calls, expressions, and statements.
 */
class Interpreter
{
public:
    /**
     * @brief Constructor that initializes the interpreter with an AST
     * @param root The root node of the abstract syntax tree
     *
     * Sets up the interpreter with the provided AST and initializes the output file.
     */
    Interpreter(AST_NODE *root) : root(root)
    {
        setupOutputFile();
    }

    /**
     * @brief Destructor that ensures output file is closed
     */
    ~Interpreter()
    {
        if (outputFile.is_open())
        {
            outputFile.close();
        }
    }

    /**
     * @brief Sets up the output file for logging program execution
     */
    void setupOutputFile();

    /**
     * @brief Begins execution of the program from the root node
     */
    void execute();

private:
    AST_NODE *root;                                                ///< Root of the abstract syntax tree
    std::map<std::string, Value> variables;                        ///< Symbol table for variable storage
    std::ofstream outputFile;                                      ///< File stream for logging output
    Value returnValue;                                             ///< Holds return values from functions
    std::map<std::string, std::stack<Value>> functionReturnValues; ///< Tracks return values for recursive calls

    /**
     * @brief Finds a function declaration by name
     * @param name The name of the function to find
     * @return Pointer to the function's AST node or nullptr if not found
     */
    AST_NODE *findFunctionByName(const std::string &name);

    /**
     * @brief Executes a node in the AST
     * @param node The node to execute
     *
     * Main recursive function that processes nodes according to their type.
     */
    void executeNode(AST_NODE *node);

    /**
     * @brief Evaluates an expression and returns its value
     * @param node The expression node to evaluate
     * @return The calculated value of the expression
     *
     * Handles expressions such as arithmetic operations and function calls.
     */
    Value evaluateExpression(AST_NODE *node);

    /**
     * @brief Executes a statement node
     * @param node The statement node to execute
     *
     * Processes statements such as variable declarations and assignments.
     */
    void executeStatement(AST_NODE *node);

    /**
     * @brief Executes a function call and returns the result
     * @param node The function call node
     * @return The value returned by the function
     */
    Value executeFunctionCall(AST_NODE *node);

    /**
     * @brief Processes a result/return statement
     * @param node The result statement node
     * @return The value to be returned from the function
     */
    Value executeResultStatement(AST_NODE *node);

    /**
     * @brief Outputs a value to both console and the output file
     * @param value The value to print
     *
     * Handles different value types and formats the output appropriately.
     * Manages flushing of output streams based on content.
     */
    void printToOutput(const Value &value)
    {
        // Print to console
        if (value.isInteger())
        {
            std::cout << value.getInteger();
            outputFile << value.getInteger();
        }
        else if (value.isDouble())
        {
            std::cout << value.getDouble();
            outputFile << value.getDouble();
        }
        else if (value.isBool())
        {
            std::cout << (value.getBool() ? "true" : "false");
            outputFile << (value.getBool() ? "true" : "false");
        }
        else if (value.isString())
        {
            // Check if the string contains newline characters
            std::string str = value.getString();
            bool containsNewline = str.find('\n') != std::string::npos;

            std::cout << str;
            outputFile << str;

            // Only flush if the string contains a newline
            if (containsNewline)
            {
                std::cout.flush();
                outputFile.flush();
            }
        }
        else if (value.isChar())
        {
            char ch = value.getChar();
            std::cout << ch;
            outputFile << ch;

            // Only flush if the character is a newline
            if (ch == '\n')
            {
                std::cout.flush();
                outputFile.flush();
            }
        }
        else
        {
            std::cout << "NULL";
            outputFile << "NULL";
        }
    }

    /**
     * @brief Processes an input statement and stores the user input in a variable
     * @param node the input statement node
     * @return the value that was input
     */
    Value executeInputStatement(AST_NODE *node)
    {
        // Get the variable name from the sub-statement
        if (node->SUB_STATEMENTS.empty())
        {
            std::cerr << "Runtime Error: No variable specified for input" << std::endl;
            exit(1);
        }

        AST_NODE *varNode = node->SUB_STATEMENTS[0];
        std::string varName = varNode->VALUE;

        // Get the input type from the child node
        if (!node->CHILD)
        {
            std::cerr << "Runtime Error: No input type specified" << std::endl;
            exit(1);
        }

        std::string inputType = node->CHILD->VALUE;
        std::string userInput;

        // Read input from the user
        // std::cout << "Enter " << inputType << " value for " << varName << ": ";
        std::getline(std::cin, userInput);

        // Convert and store the input according to its type
        Value result;
        try
        {
            if (inputType == "int")
            {
                int value = std::stoi(userInput);
                result = Value(value);
            }
            else if (inputType == "double")
            {
                double value = std::stod(userInput);
                result = Value(value);
            }
            else if (inputType == "string" || inputType == "str")
            {
                result = Value(userInput);
            }
            else if (inputType == "char")
            {
                if (userInput.empty())
                {
                    result = Value('\0');
                }
                else
                {
                    result = Value(userInput[0]);
                }
            }
            else if (inputType == "bool")
            {
                // Convert various forms of boolean input
                std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
                if (userInput == "true" || userInput == "1" || userInput == "yes" || userInput == "y")
                {
                    result = Value(true);
                }
                else
                {
                    result = Value(false);
                }
            }
            else
            {
                std::cerr << "Runtime Error: Unsupported input type: " << inputType << std::endl;
                exit(1);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Runtime Error: Invalid input format for type " << inputType << std::endl;
            exit(1);
        }

        // Store the result in the symbol table
        variables[varName] = result;

        // Log the input operation
        // outputFile << "Input processed: " << varName << " = ";
        printToOutput(result);
        outputFile << std::endl;

        return result;
    }

    // Helper functions for function return values
    Value getReturnValue()
    {
        Value temp = returnValue;
        // Create a new uninitialized value
        returnValue = Value();
        return temp;
    }

    void setReturnValue(const Value &value)
    {
        returnValue = value;
    }

    // Function to create a unique call ID for each recursive function call
    std::string generateCallID(const std::string &funcName, int depth)
    {
        return funcName + "_" + std::to_string(depth);
    }

    bool hasReturnValue()
    {
        return returnValue.isInitialized();
    }
};

#endif // INTERPRETER_HPP