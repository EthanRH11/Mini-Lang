#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <variant>
#include <fstream>

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
    AST_NODE *root;                         ///< Root of the abstract syntax tree
    std::map<std::string, Value> variables; ///< Symbol table for variable storage
    std::ofstream outputFile;               ///< File stream for logging output

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
};

#endif // INTERPRETER_HPP