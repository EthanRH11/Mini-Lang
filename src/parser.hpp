#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <vector>
#include <string>
#include <iostream>

// Node Types for the AST
enum NODE_TYPE
{
    NODE_ROOT,           // 0
    NODE_VARIABLE,       // 1
    NODE_PRINT,          // 2
    NODE_RETURN,         // 3
    NODE_INT,            // 4
    NODE_INT_LITERAL,    // 5
    NODE_EQUALS,         // 6
    NODE_SEMICOLON,      // 7
    NODE_IDENTIFIER,     // 8
    NODE_ADD,            // 9
    NODE_DOUBLE_LITERAL, // 10
    NODE_DOUBLE,         // 11
    NODE_CHAR_LITERAL,   // 12
    NODE_CHAR,           // 13
    NODE_STRING_LITERAL, // 14
    NODE_STRING,         // 15
    NODE_LEFT_PAREN,     // 16
    NODE_RIGHT_PAREN,    // 17
    NODE_PAREN_EXPR,     // 18
    NODE_EOF,            // 19
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
    AST_NODE *parseIntegerValue();
    AST_NODE *parseKeywordDouble();
    AST_NODE *parseKeywordString();
    AST_NODE *parseDoubleValue();
    AST_NODE *parseStringValue();
    AST_NODE *parseKeywordChar();
    AST_NODE *parseCharValue();
    AST_NODE *parseEquals();
    AST_NODE *parserIntegerValue();
    AST_NODE *parseSemicolon();
    AST_NODE *parseID();
    AST_NODE *parseAdd();
    AST_NODE *parseLeftParen();
    AST_NODE *parseRightParen();
    AST_NODE *parseExpression();
    AST_NODE *parseTerm();
};

#endif