#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <vector>
#include <string>
#include <iostream>

enum NODE_TYPE
{
    NODE_ROOT,
    NODE_ID,
    NODE_PRINT,
    NODE_RETURN,
    NODE_INT,
};

struct AST_NODE
{
    enum NODE_TYPE TYPE;
    std::string *VALUE;
    AST_NODE *CHILD;
    std::vector<AST_NODE *> SUB_STATEMENTS; // This is only for the root mode
};

class Parser
{
public:
    Parser(std::vector<Token *> tokens)
    {
        parserTokens = tokens;
        index = 0;
        limit = parserTokens.size();
        current = parserTokens.at(index);
    }

    Token *proceed(enum tokenType type)
    {
        if (current->TYPE != type)
        {
            std::cerr << "< Syntax Error >  " << std::endl;
            exit(1);
        }
        else
        {
            index++;
            current = parserTokens.at(index);
            return current;
        }
    }

    AST_NODE *parseID()
    {
    }

    AST_NODE *parse()
    {
        AST_NODE *ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;

        while (current->TYPE != EOF)
        {
            switch (current->TYPE)
            {
            case TOKEN_ID:
            {
                ROOT->SUB_STATEMENTS.push_back(parseID());
                break;
            }
            case TOKEN_KEYWORD:
            {
                ROOT->SUB_STATEMENTS.push_back(parseKeyword());
                break;
            }
            default:
            {
                std::cerr << "< Syntax Error >" << std::endl;
                exit(1);
            }
            }
            proceed(TOKEN_SEMICOLON);
        }

        return ROOT;
    }

private:
    int limit;
    int index;
    Token *current;
    std::vector<Token *> parserTokens;
};

#endif