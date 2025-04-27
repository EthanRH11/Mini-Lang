#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <vector>
#include <string>
#include <iostream>

/**
 * @brief Node types for the Abstract Syntax Tree (AST)
 *
 * These enumeration constants represent all possible types of nodes
 * that can appear in the AST during parsing.
 */
enum NODE_TYPE
{
    // Program structure nodes
    NODE_ROOT,        // Root node of the AST
    NODE_BLOCK,       // Code block (multiple statements)
    NODE_BEGIN_BLOCK, // The 'begin:' block that starts a program
    NODE_SEMICOLON,   // Statement terminator
    NODE_EOF,         // End of file marker

    // Variable and type declaration nodes
    NODE_VARIABLE,   // Variable declaration
    NODE_IDENTIFIER, // Variable or function identifier
    NODE_INT,        // Integer type declaration
    NODE_DOUBLE,     // Double type declaration
    NODE_CHAR,       // Character type declaration
    NODE_STRING,     // String type declaration

    // Literal value nodes
    NODE_INT_LITERAL,    // Integer literal value
    NODE_DOUBLE_LITERAL, // Double literal value
    NODE_CHAR_LITERAL,   // Character literal value
    NODE_STRING_LITERAL, // String literal value

    // Operator nodes
    NODE_EQUALS,             // Assignment operator
    NODE_ADD,                // Addition operator
    NODE_MULT,               // Multiplication operator
    NODE_SUBT,               // Subtraction operator
    NODE_LESS_THAN,          // Less than operator
    NODE_GREATER_THAN,       // Greater than operator
    NODE_OPERATOR_INCREMENT, // Increment operator (++)
    NODE_NEWLINE,            // Newline character

    // Control structure nodes
    NODE_IF,       // If statement
    NODE_FOR,      // For loop
    NODE_FOR_ARGS, // For loop arguments

    // I/O nodes
    NODE_PRINT, // Print statement (out_to_console)

    // Expression grouping nodes
    NODE_LEFT_PAREN,  // Left parenthesis
    NODE_RIGHT_PAREN, // Right parenthesis
    NODE_PAREN_EXPR,  // Parenthesized expression
    NODE_LEFT_CURL,   // Left curly brace
    NODE_RIGHT_CURL,  // Right curly brace

    // Function-related nodes
    NODE_FUNCTION,             // Function type
    NODE_FUNCTION_DECLERATION, // Function declaration
    NODE_FUNCTION_PARAMS,      // Function parameters list
    NODE_FUNCTION_BODY,        // Function body
    NODE_PARAM,                // Individual function parameter
    NODE_FUNCTION_SPACESHIP,   // Function return type separator (=>)
    NODE_FUNCTION_CALL,        // Function call
    NODE_RETURN                // Return statement
};

/**
 * @brief Abstract Syntax Tree Node structure
 *
 * Represents a node in the AST with its type, value, child node,
 * and a list of sub-statements (for compound statements).
 */
struct AST_NODE
{
    enum NODE_TYPE TYPE;                    // Type of the node
    std::string VALUE;                      // Value associated with the node
    AST_NODE *CHILD;                        // Child node (for nodes with single child)
    std::vector<AST_NODE *> SUB_STATEMENTS; // List of sub-statements (for compound nodes)

    /**
     * @brief Default constructor
     *
     * Initializes the node as a ROOT node with no children.
     */
    AST_NODE() : TYPE(NODE_ROOT), CHILD(nullptr) {}
};

/**
 * @brief Parser class for syntax analysis
 *
 * Responsible for parsing tokens produced by the lexer and
 * constructing an Abstract Syntax Tree (AST) representation
 * of the program.
 */
class Parser
{
public:
    /**
     * @brief Constructor for Parser class
     * @param tokens Vector of tokens from the lexer
     *
     * Initializes the parser with the token stream and sets up
     * the cursor to begin parsing from the first token.
     */
    Parser(std::vector<Token *> tokens)
    {
        this->tokens = tokens;
        cursor = 0;
        size = tokens.size();
        current = tokens[0];
    }

    /**
     * @brief Main parsing method
     * @return Pointer to the root AST node
     *
     * Parses the entire token stream and constructs an AST.
     */
    AST_NODE *parse();

private:
    std::vector<Token *> tokens; // Token stream
    size_t cursor;               // Current position in the token stream
    size_t size;                 // Total number of tokens
    Token *current;              // Current token being processed

    /**
     * @brief Advances to the next token if the current token matches the expected type
     * @param type Expected token type
     * @return Pointer to the matched token, or nullptr if no match
     *
     * Checks if the current token matches the expected type and advances
     * the cursor if it does.
     */
    Token *proceed(enum tokenType type);

    /**
     * @brief Gets the current token
     * @return Pointer to the current token
     */
    Token *getCurrentToken();

    /**
     * @brief Looks ahead at the next token without advancing the cursor
     * @return Pointer to the next token
     */
    Token *peakAhead();

    /**
     * @brief Advances the cursor to the next token
     *
     * Updates the current token pointer to point to the next token.
     */
    void advanceCursor();

    //---------------------------------------------------------------------
    // Parse methods for various language constructs
    //---------------------------------------------------------------------

    // Statement terminators and structure
    AST_NODE *parseKeywordEOF();
    AST_NODE *parseSemicolon();
    AST_NODE *parseBeginBlock();

    // I/O statements
    AST_NODE *parseKeywordPrint();

    // Type declarations
    AST_NODE *parseKeywordINT();
    AST_NODE *parseKeywordDouble();
    AST_NODE *parseKeywordChar();

    // Operators
    AST_NODE *parseIncrementOperator();
    AST_NODE *parseEquals();
    AST_NODE *parseAdd();
    AST_NODE *parseMult();
    AST_NODE *parseSubt();
    AST_NODE *greaterThan();
    AST_NODE *lessThan();
    AST_NODE *parseNewLine();

    // Literal values
    AST_NODE *parseIntegerValue();
    AST_NODE *parseDoubleValue();
    AST_NODE *parseStringValue();
    AST_NODE *parseCharValue();
    AST_NODE *parserIntegerValue(); // Note: possible typo in original

    // Identifiers
    AST_NODE *parseID();

    // Expressions and grouping
    AST_NODE *parseLeftParen();
    AST_NODE *parseRightParen();
    AST_NODE *parseExpression();
    AST_NODE *parseTerm();
    AST_NODE *parseLeftCurl();
    AST_NODE *parseRightCurl();

    // Control structures
    AST_NODE *parseKeywordIf();
    AST_NODE *parseKeywordElse();
    AST_NODE *parseKeywordFor();
    AST_NODE *parseArgs();

    // General statement parsing
    AST_NODE *parseStatement();

    //---------------------------------------------------------------------
    // Function-related parsing methods
    //---------------------------------------------------------------------

    /**
     * @brief Parses a function declaration
     * @return AST node representing the function declaration
     *
     * Handles the 'proc' keyword and function name.
     */
    AST_NODE *parseFunctionDecleration();

    /**
     * @brief Parses function parameters
     * @return AST node representing the parameter list
     *
     * Handles parameter lists like (int x, int y).
     */
    AST_NODE *parseFunctionParams();

    /**
     * @brief Parses an individual function parameter
     * @return AST node representing a single parameter
     *
     * Handles individual parameters like 'int x'.
     */
    AST_NODE *parseParameter();

    /**
     * @brief Parses a function body
     * @return AST node representing the function body
     *
     * Handles the block of statements enclosed in { }.
     */
    AST_NODE *parseFunctionBody();
};

/**
 * @brief Helper function to get string representation of node types
 * @param type The node type to convert to string
 * @return String representation of the node type
 *
 * Converts the enum node type to a human-readable string for debugging
 * and error reporting purposes.
 */
std::string getNodeTypeName(NODE_TYPE type);

#endif // PARSER_HPP