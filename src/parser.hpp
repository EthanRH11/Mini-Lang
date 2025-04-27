#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <vector>
#include <string>
#include <iostream>

// Node Types for the AST
enum NODE_TYPE
{
    NODE_ROOT,                 // 0
    NODE_VARIABLE,             // 1
    NODE_PRINT,                // 2
    NODE_RETURN,               // 3
    NODE_INT,                  // 4
    NODE_INT_LITERAL,          // 5
    NODE_EQUALS,               // 6
    NODE_SEMICOLON,            // 7
    NODE_IDENTIFIER,           // 8
    NODE_ADD,                  // 9
    NODE_DOUBLE_LITERAL,       // 10
    NODE_DOUBLE,               // 11
    NODE_CHAR_LITERAL,         // 12
    NODE_CHAR,                 // 13
    NODE_STRING_LITERAL,       // 14
    NODE_STRING,               // 15
    NODE_LEFT_PAREN,           // 16
    NODE_RIGHT_PAREN,          // 17
    NODE_PAREN_EXPR,           // 18
    NODE_LEFT_CURL,            // 19
    NODE_RIGHT_CURL,           // 20
    NODE_LESS_THAN,            // 21
    NODE_GREATER_THAN,         // 22
    NODE_BLOCK,                // 23
    NODE_IF,                   // 24
    NODE_MULT,                 // 25
    NODE_SUBT,                 // 26
    NODE_FOR,                  // 27
    NODE_FOR_ARGS,             // 28
    NODE_OPERATOR_INCREMENT,   // 29
    NODE_EOF,                  // 30
    NODE_NEWLINE,              // 31
    NODE_FUNCTION_DECLERATION, // 32
    NODE_FUNCTION_PARAMS,      // 33
    NODE_FUNCTION_BODY,        // 34
    NODE_PARAM,                // 35
    NODE_FUNCTION_SPACESHIP,   // 36
    NODE_BEGIN_BLOCK,          // 37
    NODE_FUNCTION,             // 38
    NODE_FUNCTION_CALL,        // 39
};

struct AST_NODE
{
    enum NODE_TYPE TYPE;
    std::string VALUE;
    AST_NODE *CHILD;
    std::vector<AST_NODE *> SUB_STATEMENTS;

    AST_NODE() : TYPE(NODE_ROOT), CHILD(nullptr) {}
};

class Parser
{
public:
    Parser(std::vector<Token *> tokens)
    {
        this->tokens = tokens;
        cursor = 0;
        size = tokens.size();
        current = tokens[0];
    }

    AST_NODE *parse();

private:
    std::vector<Token *> tokens;
    size_t cursor;
    size_t size;
    Token *current;

    // Proceed to next token, if current token matches
    Token *proceed(enum tokenType type);

    // Helper to get current token
    Token *getCurrentToken();

    Token *peakAhead();

    // Advance the cursor
    void advanceCursor();

    // Parse specific constructs
    AST_NODE *parseKeywordEOF();
    AST_NODE *parseKeywordPrint();
    AST_NODE *parseKeywordINT();
    AST_NODE *parseKeywordDouble();
    AST_NODE *parseKeywordChar();
    AST_NODE *parseIncrementOperator();

    AST_NODE *parseIntegerValue();
    AST_NODE *parseDoubleValue();
    AST_NODE *parseStringValue();
    AST_NODE *parseCharValue();
    AST_NODE *parserIntegerValue();
    AST_NODE *parseNewLine();

    AST_NODE *parseEquals();
    AST_NODE *parseSemicolon();
    AST_NODE *parseID();
    AST_NODE *parseAdd();
    AST_NODE *parseMult();
    AST_NODE *parseSubt();
    AST_NODE *parseLeftParen();
    AST_NODE *parseRightParen();
    AST_NODE *parseExpression();
    AST_NODE *parseTerm();

    AST_NODE *parseLeftCurl();
    AST_NODE *parseRightCurl();
    AST_NODE *greaterThan();
    AST_NODE *lessThan();

    AST_NODE *parseKeywordIf();
    AST_NODE *parseKeywordElse();
    AST_NODE *parseKeywordFor();
    AST_NODE *parseArgs();

    AST_NODE *parseStatement();
    AST_NODE *parseBeginBlock();

    // Function handling
    // For parsing function decleration
    AST_NODE *parseFunctionDecleration();
    // parses params(int x, int y);
    AST_NODE *parseFunctionParams();
    // parse individual params
    AST_NODE *parseParameter();
    // parsing function body {}
    AST_NODE *parseFunctionBody();
};

std::string getNodeTypeName(NODE_TYPE type);

#endif