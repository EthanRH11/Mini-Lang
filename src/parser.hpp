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
    NODE_BOOL,       // bool type decleration

    // Input nodes
    NODE_KEYWORD_INPUT,
    NODE_INPUT_TYPE,
    NODE_INPUT_PROMPT,

    // Literal value nodes
    NODE_INT_LITERAL,    // Integer literal value
    NODE_DOUBLE_LITERAL, // Double literal value
    NODE_CHAR_LITERAL,   // Character literal value
    NODE_STRING_LITERAL, // String literal value
    NODE_BOOL_LITERAL,   // Bool literal value

    // NODES FOR RETURN STATEMENTS
    NODE_RESULTSTATEMENT,   // Top Level node of entire return statement
    NODE_RESULT,            // Represent the result keyword
    NODE_RESULT_EXPRESSION, // Represent the expression in curly brackets

    // Operator nodes
    NODE_EQUALS,             // Assignment operator
    NODE_ADD,                // Addition operator
    NODE_MULT,               // Multiplication operator
    NODE_SUBT,               // Subtraction operator
    NODE_LESS_THAN,          // Less than operator
    NODE_GREATER_THAN,       // Greater than operator
    NODE_OPERATOR_INCREMENT, // Increment operator (++)
    NODE_OPERATOR_DECREMENT, // Decrement operator (--)
    NODE_NEWLINE,            // Newline character
    NODE_MODULUS,            // Modulus character
    NODE_DIVISION,           // Division character
    NODE_NOT_EQUAL,          // not equal character '=/='
    NODE_LESS_EQUAL,         // Less than or equal to operator

    // Control structure nodes
    NODE_IF,       // If statement
    NODE_FOR,      // For loop
    NODE_FOR_ARGS, // For loop arguments

    // I/O nodes
    NODE_PRINT, // Print statement (out_to_console)

    // NEWLINE CHAR
    NODE_NEWLINE_SYMBOL,

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
    NODE_RETURN,               // Return statement

    NODE_CHECK, // Check loop

    NODE_ELEMENT_TYPE,

    NODE_ARRAY_DECLARATION, // For declarations like: elements<int> arr;
    NODE_ARRAY_ACCESS,      // For accessing elements: arr@(index)
    NODE_ARRAY_ASSIGN,      // For assignment: arr@(index) = value;
    NODE_ARRAY_INIT,        // For initialization: arr |= (1, 2, 3);
    NODE_ARRAY_RANGE,       // For range init: arr |= range(1..10);
    NODE_ARRAY_REPEAT,      // For repeat init: arr |= repeat(0, 10);
    NODE_ARRAY_LENGTH,      // For getting length: #arr
    NODE_ARRAY_INSERT,      // For insertion: +> arr(index, value)
    NODE_ARRAY_REMOVE,      // For removal: -< arr(index)
    NODE_ARRAY_SORT_ASC,    // For ascending sort: ~> arr
    NODE_ARRAY_SORT_DESC,   // For descending sort: <~ arr
    NODE_ARRAY_LAST_INDEX,
    NODE_DOT,
    NODE_ARRAY_INDEX,

    NODE_COMMENT,
    NODE_RANGE_OPERATOR,

    NODE_READ_HEADER,
    NODE_OBJECT,
    NODE_AVAILABLE,
    NODE_SECURE,
    NODE_COLON_COLON,
    NODE_OBJECT_DEFAULT,
    NODE_OBJECT_FACTORY,
    NODE_OBJECT_METHOD,
    NODE_ARROW_OP,
    NODE_END_HEADER,
    NODE_COLON_ACCESSOR,

    NODE_HEADER,
    NODE_NEEDS_BLOCK,
    NODE_CONST_NUM,

    NODE_RANDOMINT,
    NODE_COINFLIP,
    NODE_DICEROLL,
    NODE_GENERATEPIN,
    ROOT_LIBRARY,
    NODE_IMPORT_LIBRARY,
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
    AST_NODE *parseArrayLastIndex();
    AST_NODE *parseSingleLineComment();
    AST_NODE *parseMultiLineComment();
    /**
     * @brief Constructor for Parser class
     * @param tokens Vector of tokens from the lexer
     *
     * Initializes the parser with the token stream and sets up
     * the cursor to begin parsing from the first token.
     */
    Parser(std::vector<Token *> tokens, bool isHeader = false)
    {
        this->tokens = tokens;
        cursor = 0;
        size = tokens.size();
        current = tokens[0];
        this->isHeader = isHeader;

        initializeParserMaps();
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
    bool isHeader = false;
    bool isConst = false;

    std::string arrayIDforDot; // helper string for dot function
    using ParseFunction = AST_NODE *(Parser::*)();

    std::unordered_map<tokenType, ParseFunction> statementDispatch;
    std::unordered_map<tokenType, ParseFunction> expressionDispatch;
    std::unordered_map<tokenType, int> operatorPrecedence;
    std::unordered_map<tokenType, NODE_TYPE> tokenToNodeType;

    /**
     * @brief Initialize dispatch tables and maps for token processing
     *
     * Sets up maps for token-to-function dispatching, operator precedence,
     * and token-to-node-type conversion to improve parsing efficency
     */
    void initializeParserMaps();

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
    // Parsing Header Files

    AST_NODE *parseHeaderFile();
    bool processHeaderFile(AST_NODE *headerNode, const std::string &headerPath);
    //---------------------------------------------------------------------
    // Parse methods for various language constructs
    //---------------------------------------------------------------------

    // Statement terminators and structure
    AST_NODE *parseKeywordEOF();
    AST_NODE *parseSemicolon();
    AST_NODE *parseBeginBlock();
    AST_NODE *parseKeywordConst();
    // I/O statements
    AST_NODE *parseKeywordPrint();

    // Type declarations
    AST_NODE *parseKeywordINT();
    AST_NODE *parseKeywordDouble();
    AST_NODE *parseKeywordChar();
    AST_NODE *parseKeywordBool();

    AST_NODE *parseKeywordResult();

    // Input
    AST_NODE *parseKeywordInput();
    AST_NODE *parseInputType();
    AST_NODE *parseKeywordElement();
    AST_NODE *parseElementType();

    // Operators
    AST_NODE *parseIncrementOperator();
    AST_NODE *parseDecrementOperator();
    AST_NODE *parseEquals();
    AST_NODE *parseAdd();
    AST_NODE *parseMult();
    AST_NODE *parseSubt();
    AST_NODE *parseDivi();
    AST_NODE *parseModulus();
    AST_NODE *greaterThan();
    AST_NODE *lessThan();
    AST_NODE *parseDoesntEqual();
    AST_NODE *parseNewLine();
    AST_NODE *parseNewLineCharacter();

    // Literal values
    AST_NODE *parseIntegerValue();
    AST_NODE *parseDoubleValue();
    AST_NODE *parseStringValue();
    AST_NODE *parseCharValue();
    AST_NODE *parserIntegerValue(); // Note: possible typo in original
    AST_NODE *parseBoolValue();

    // Identifiers
    AST_NODE *parseID();

    // Expressions and grouping
    AST_NODE *parseLeftParen();
    AST_NODE *parseRightParen();
    AST_NODE *parseExpression();
    AST_NODE *parseResultExpression();
    AST_NODE *parseTerm();
    AST_NODE *parseLeftCurl();
    AST_NODE *parseRightCurl();

    // Control structures
    AST_NODE *parseKeywordCheck();
    AST_NODE *parseKeywordIf();
    AST_NODE *parseKeywordElse();
    AST_NODE *parseKeywordFor();
    AST_NODE *parseArgs();

    // Return statement
    AST_NODE *parseResultStatement();

    // General statement parsing
    AST_NODE *parseStatement();

    // Array parsing
    AST_NODE *parseKeywordRange();
    AST_NODE *parseKeywordRepeat();

    // Array-related methods
    AST_NODE *parseArrayDeclaration();
    AST_NODE *parseArrayInit();
    AST_NODE *parseArrayRange();
    AST_NODE *parseArrayRepeat();
    AST_NODE *parseArrayLength();
    AST_NODE *parseArrayInsert();
    AST_NODE *parseArrayRemove();
    AST_NODE *parseArraySortAsc();
    AST_NODE *parseArraySortDesc();
    AST_NODE *parseDot(/*const std::string &*/);
    AST_NODE *parseDotExpression();

    AST_NODE *parseReadHeader();
    AST_NODE *parseEndHeader();

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

    AST_NODE *parseByTokenType(const std::unordered_map<tokenType, ParseFunction> &dispatchTable);
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