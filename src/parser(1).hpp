#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>

enum NODE_TYPE
{
    // Program structure nodes
    NODE_ROOT,
    NODE_BLOCK,
    NODE_BEGIN_BLOCK,
    NODE_SEMICOLON,
    NODE_EOF,

    // Variable and type declaration nodes
    NODE_VARIABLE,
    NODE_IDENTIFIER,
    NODE_INT,
    NODE_DOUBLE,
    NODE_CHAR,
    NODE_STRING,
    NODE_BOOL,

    // Input nodes
    NODE_KEYWORD_INPUT,
    NODE_INPUT_TYPE,
    NODE_INPUT_PROMPT,

    // Literal value nodes
    NODE_INT_LITERAL,
    NODE_DOUBLE_LITERAL,
    NODE_CHAR_LITERAL,
    NODE_STRING_LITERAL,
    NODE_BOOL_LITERAL,

    // Return statement nodes
    NODE_RESULTSTATEMENT,
    NODE_RESULT,
    NODE_RESULT_EXPRESSION,

    // Operator nodes
    NODE_EQUALS,
    NODE_ADD,
    NODE_MULT,
    NODE_SUBT,
    NODE_LESS_THAN,
    NODE_GREATER_THAN,
    NODE_OPERATOR_INCREMENT,
    NODE_OPERATOR_DECREMENT,
    NODE_NEWLINE,
    NODE_MODULUS,
    NODE_DIVISION,
    NODE_NOT_EQUAL,
    NODE_LESS_EQUAL,

    // Control structure nodes
    NODE_IF,
    NODE_FOR,
    NODE_FOR_ARGS,
    NODE_CHECK,

    // I/O nodes
    NODE_PRINT,

    // Special character nodes
    NODE_NEWLINE_SYMBOL,

    // Expression grouping nodes
    NODE_LEFT_PAREN,
    NODE_RIGHT_PAREN,
    NODE_PAREN_EXPR,
    NODE_LEFT_CURL,
    NODE_RIGHT_CURL,

    // Function-related nodes
    NODE_FUNCTION,
    NODE_FUNCTION_DECLERATION,
    NODE_FUNCTION_PARAMS,
    NODE_FUNCTION_BODY,
    NODE_PARAM,
    NODE_FUNCTION_SPACESHIP,
    NODE_FUNCTION_CALL,
    NODE_RETURN,

    // Element nodes
    NODE_ELEMENT,
    NODE_ELEMENTTYPE,
};

// Forward Declerations
class Lexer;

struct AST_NODE
{
    NODE_TYPE TYPE;
    std::string VALUE;

    // Use unique_ptr for ownership
    std::unique_ptr<AST_NODE> CHILD;

    // For sub-statements, use vector of unique_ptrs
    std::vector<std::unique_ptr<AST_NODE>> SUB_STATEMENTS;

    // Default constructor
    AST_NODE() : TYPE(NODE_ROOT), CHILD(nullptr) {}

    // Add move constructor and assignment operator for efficient transfer of ownership
    AST_NODE(AST_NODE &&other) noexcept;
    AST_NODE &operator=(AST_NODE &&other) noexcept;

    // Add destructor (not strictly necessary with smart pointers but good practice)
    ~AST_NODE() = default;

    // Prevent copying to avoid deep copies
    AST_NODE(const AST_NODE &) = delete;
    AST_NODE &operator=(const AST_NODE &) = delete;
};

class Parser
{
public:
    Parser(const std::vector<Token *> &tokens);
    std::unique_ptr<AST_NODE> parse();

private:
    const std::vector<Token *> &tokens;
    size_t cursor;
    size_t size;
    Token *current;

    using ParseFunction = std::unique_ptr<AST_NODE> (Parser::*)();

    // Tables to replace large switch statements
    std::unordered_map<tokenType, ParseFunction> statementDispatch;
    std::unordered_map<tokenType, ParseFunction> expressionDispatch;

    // Initialize tables
    void initializeDispatchTables();

    Token *proceed(tokenType type);
    Token *getCurrentToken() const { return current; }
    Token *peakAhead() const;
    void advanceCursor();

    // Generic Parse Method that uses dispatch tables
    std::unique_ptr<AST_NODE> parseByTokenType(const std::unordered_map<tokenType, ParseFunction> &dispatchTable);

    // Parse methods - return unique_ptr for ownership transfer
    std::unique_ptr<AST_NODE> parseKeywordEOF();
    std::unique_ptr<AST_NODE> parseSemicolon();
    std::unique_ptr<AST_NODE> parseBeginBlock();
    std::unique_ptr<AST_NODE> parseKeywordPrint();
    std::unique_ptr<AST_NODE> parseKeywordINT();
    std::unique_ptr<AST_NODE> parseKeywordDouble();
    std::unique_ptr<AST_NODE> parseKeywordChar();
    std::unique_ptr<AST_NODE> parseKeywordBool();
    std::unique_ptr<AST_NODE> parseKeywordResult();
    std::unique_ptr<AST_NODE> parseKeywordInput();
    std::unique_ptr<AST_NODE> parseInputType();
    std::unique_ptr<AST_NODE> parseKeywordElement();
    std::unique_ptr<AST_NODE> parseElementType();
    std::unique_ptr<AST_NODE> parseIncrementOperator();
    std::unique_ptr<AST_NODE> parseDecrementOperator();
    std::unique_ptr<AST_NODE> parseEquals();
    std::unique_ptr<AST_NODE> parseAdd();
    std::unique_ptr<AST_NODE> parseMult();
    std::unique_ptr<AST_NODE> parseSubt();
    std::unique_ptr<AST_NODE> parseDivi();
    std::unique_ptr<AST_NODE> parseModulus();
    std::unique_ptr<AST_NODE> greaterThan();
    std::unique_ptr<AST_NODE> lessThan();
    std::unique_ptr<AST_NODE> parseDoesntEqual();
    std::unique_ptr<AST_NODE> parseNewLine();
    std::unique_ptr<AST_NODE> parseNewLineCharacter();
    std::unique_ptr<AST_NODE> parseIntegerValue();
    std::unique_ptr<AST_NODE> parseDoubleValue();
    std::unique_ptr<AST_NODE> parseStringValue();
    std::unique_ptr<AST_NODE> parseCharValue();
    std::unique_ptr<AST_NODE> parserIntegerValue(); // Note: possible typo in original
    std::unique_ptr<AST_NODE> parseBoolValue();
    std::unique_ptr<AST_NODE> parseID();
    std::unique_ptr<AST_NODE> parseLeftParen();
    std::unique_ptr<AST_NODE> parseRightParen();
    std::unique_ptr<AST_NODE> parseExpression();
    std::unique_ptr<AST_NODE> parseResultExpression();
    std::unique_ptr<AST_NODE> parseTerm();
    std::unique_ptr<AST_NODE> parseLeftCurl();
    std::unique_ptr<AST_NODE> parseRightCurl();
    std::unique_ptr<AST_NODE> parseKeywordCheck();
    std::unique_ptr<AST_NODE> parseKeywordIf();
    std::unique_ptr<AST_NODE> parseKeywordElse();
    std::unique_ptr<AST_NODE> parseKeywordFor();
    std::unique_ptr<AST_NODE> parseArgs();
    std::unique_ptr<AST_NODE> parseResultStatement();
    std::unique_ptr<AST_NODE> parseFunctionDecleration();
    std::unique_ptr<AST_NODE> parseFunctionParams();
    std::unique_ptr<AST_NODE> parseParameter();
    std::unique_ptr<AST_NODE> parseFunctionBody();
    std::unique_ptr<AST_NODE> parseStatement();
};

std::string getTokenTypeName(tokenType type);
std::string getNodeTypeName(NODE_TYPE type);

#endif