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
#include "dynamic_array.hpp"
#include "ErrorHandler.hpp"
#include "library/LibraryManager.hpp"

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
        // LibraryManager &libraryManager = LibraryManager::getInstance();
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

    const std::map<std::string, std::stack<Value>> &getFunctionReturnValues() const
    {
        return functionReturnValues;
    }

private:
    AST_NODE *root;                                                ///< Root of the abstract syntax tree
    std::map<std::string, Value> variables;                        ///< Symbol table for variable storage
    std::ofstream outputFile;                                      ///< File stream for logging output
    Value returnValue;                                             ///< Holds return values from functions
    std::map<std::string, std::stack<Value>> functionReturnValues; ///< Tracks return values for recursive calls
    // bool hasReturnValue() const;
    /**
     * @brief Finds a function declaration by name
     * @param name The name of the function to find
     * @return Pointer to the function's AST node or nullptr if not found
     */
    AST_NODE *findFunctionByName(const std::string &name);

    /**
     * @brief handles the imports of standard libraries
     * @param node uses the import node
     *
     */
    void evaluateImport(AST_NODE *node);

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
        if (value.isInt())
        {
            std::cout << value.asInt();
            outputFile << value.asInt();
        }
        else if (value.isDouble())
        {
            std::cout << value.asDouble();
            outputFile << value.asDouble();
        }
        else if (value.isBool())
        {
            std::cout << (value.asBool() ? "true" : "false");
            outputFile << (value.asBool() ? "true" : "false");
        }
        else if (value.isString())
        {
            // Check if the string contains newline characters
            std::string str = value.asString();
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
            char ch = value.asChar();
            std::cout << ch;
            outputFile << ch;

            // Only flush if the character is a newline
            if (ch == '\n')
            {
                std::cout.flush();
                outputFile.flush();
            }
        }
        else if (value.isArray())
        {
            auto arrPtr = value.asArray();
            if (!arrPtr)
            {
                // std::cerr << "Error: null array\n";
                ErrorHandler::getInstance().reportSemanticError("Null Array.");
                return;
            }

            const DynamicArray &arr = *arrPtr;

            std::cout << "[";
            outputFile << "[";

            size_t n = arr.getLength();
            for (size_t i = 0; i < n; ++i)
            {
                Value elem = arr.getElement(static_cast<int>(i));

                printToOutput(elem);

                if (i + 1 < n)
                {
                    std::cout << ",";
                    outputFile << ",";
                }
            }

            std::cout << "]";
            outputFile << "]";
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
        // std::cerr << "DEBUG: Entering executeInputStatement" << std::endl;

        // Validate the node itself
        if (!node)
        {
            // std::cerr << "ERROR: Null node in executeInputStatement" << std::endl;
            ErrorHandler::getInstance().reportSemanticError("Null node in executeInputStatement.");
            return Value();
        }

        // std::cerr << "DEBUG: Node type: " << node->TYPE << std::endl;

        // Get input type from node->CHILD
        std::string inputType = "string"; // Default to string if type not specified
        if (node->CHILD)
        {
            inputType = node->CHILD->VALUE;
            // std::cerr << "DEBUG: Input type: " << inputType << std::endl;
        }
        else
        {
            // std::cerr << "WARNING: No input type specified, defaulting to string" << std::endl;
            ErrorHandler::getInstance().reportSemanticError("WARNING -> No input type specified, defaulting to string.");
        }

        // Get prompt node safely
        AST_NODE *promptNode = node->SUB_STATEMENTS[0];
        if (!promptNode)
        {
            // std::cerr << "ERROR: Null prompt node" << std::endl;
            ErrorHandler::getInstance().reportSemanticError("Null prompt node.");
            return Value();
        }

        // std::cerr << "DEBUG: Prompt node type: " << promptNode->TYPE << std::endl;

        // Get prompt string value
        std::string promptString = "";
        if (promptNode->CHILD)
        {
            promptString = promptNode->CHILD->VALUE;
            // std::cerr << "DEBUG: Prompt string: " << promptString << std::endl;
        }
        else
        {
            // std::cerr << "WARNING: Empty prompt" << std::endl;
            ErrorHandler::getInstance().reportSemanticError("WARNING -> Empty prompt.");
        }

        // Print prompt to console
        std::cout << promptString << std::flush;

        // Get variable name safely
        std::string varName = "";
        // std::cerr << "DEBUG: Prompt sub-statements count: " << promptNode->SUB_STATEMENTS.size() << std::endl;
        if (promptNode->SUB_STATEMENTS.size() > 0 && promptNode->SUB_STATEMENTS[0])
        {
            varName = promptNode->SUB_STATEMENTS[0]->VALUE;
            // std::cerr << "DEBUG: Variable name: " << varName << std::endl;
        }
        else
        {
            // std::cerr << "ERROR: No target variable for input" << std::endl;
            ErrorHandler::getInstance().reportSemanticError("No target variable for input.");
            return Value();
        }

        // Read user input
        // std::cerr << "DEBUG: Reading user input..." << std::endl;
        std::string userInput;
        std::getline(std::cin, userInput);
        // std::cerr << "DEBUG: User input received: " << userInput << std::endl;

        // Convert input to appropriate type
        Value result;
        // std::cerr << "DEBUG: Converting input to type: " << inputType << std::endl;

        try
        {
            if (inputType == "int")
            {
                result = Value(std::stoi(userInput));
            }
            else if (inputType == "float" || inputType == "double")
            {
                result = Value(std::stod(userInput));
            }
            else if (inputType == "bool")
            {
                std::string lowerInput = userInput;
                std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);
                bool boolValue = (lowerInput == "true" || lowerInput == "1" ||
                                  lowerInput == "yes" || lowerInput == "y");
                result = Value(boolValue);
            }
            else
            {
                result = Value(userInput);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "ERROR: Exception converting input: " << e.what() << std::endl;

            if (inputType == "int")
            {
                result = Value(0);
            }
            else if (inputType == "float" || inputType == "double")
            {
                result = Value(0.0);
            }
            else if (inputType == "bool")
            {
                result = Value(false);
            }
            else
            {
                result = Value("");
            }
        }

        // Store variable safely
        // std::cerr << "DEBUG: Storing variable '" << varName << "'" << std::endl;
        try
        {
            variables[varName] = result;
        }
        catch (const std::exception &e)
        {
            std::cerr << "ERROR: Exception storing variable: " << e.what() << std::endl;
        }

        // std::cerr << "DEBUG: Exiting executeInputStatement" << std::endl;
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

    bool hasReturnValue() const
    {
        return !returnValue.isNone();
    }

    // Functions for random Library
    Value evaluateRandomInt(AST_NODE *node);
    Value evaluateCoinFlip();
    Value evaluateDiceRoll(AST_NODE *node);
    Value evaluateGeneratePin(AST_NODE *node);
};

#endif // INTERPRETER_HPP