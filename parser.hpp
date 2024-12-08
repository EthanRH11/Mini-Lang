#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>

#include "lexer.hpp"

enum NODE_TYPE
{
    NODE_ROOT,
    NODE_ID,
    NODE_RETURN,
    NODE_PRINT,
    NODE_INT,
};

struct AST_NODE
{

    enum NODE_TYPE TYPE;
    std::string *VALUE;
    AST_NODE *child;
    std::vector<AST_NODE *> SUB_STATEMENTS; // FOR ROOT NODE
};

class Parser
{
public:
    Parser(std::vector<Token *> tokens)
    {
        parserToken = tokens;
        index = 0;
        current = parserToken.at(index);
        int limit = parserToken.size();
    }

    Token *proceed(/*TODO: Figure out the argument for the proceed function*/) {}

    AST_NODE *parse()
    {
        AST_NODE *ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;

        return ROOT;
    }

private:
    int index;
    int limit;
    Token *current;
    std::vector<Token *> parserToken;
};

#endif