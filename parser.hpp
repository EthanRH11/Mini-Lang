#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <vector>
#include <string>
#include <iostream>

enum NODE_TYPE
{
    NODE_ROOT,
    NODE_VARIABLE,
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
    AST_NODE *parseKeywordEof()
    {
        /*Program is done executing*/
        }
    AST_NODE *parseKeywordPrint()
    {
        /*Must be with in a parenthesis to print*/
    }

    AST_NODE *parseKeywordInt()
    {
        /*Could set it up in 2 seperate ways
          set it up to check if there is a value assigned to
          the variable. or if the variable is just declared.*/
        proceed(TOKEN_KEYWORD_INT);

        std::string *variableName = &current->value;
        proceed(TOKEN_IDENTIFIER);

        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_INT;
        newNode->VALUE = variableName;

        if (current->TYPE == TOKEN_EQUALS)
        {
            proceed(TOKEN_EQUALS);

            if (current->TYPE != TOKEN_INTEGER_VAL)
            {
                std::cerr << "< Syntax Error > Expected an integer after '='\n";
                exit(1);
            }

            AST_NODE *childNode = new AST_NODE();
            childNode->TYPE = NODE_INT;
            childNode->VALUE = &current->value;

            newNode->CHILD = childNode;
            proceed(TOKEN_INTEGER_VAL);
        }
        else
        {
            newNode->CHILD = nullptr;
        }
        return newNode;
    }

    AST_NODE *parseID()
    {
        std::string *buffer = &current->value;
        proceed(TOKEN_IDENTIFIER);
        proceed(TOKEN_EQUALS);

        AST_NODE *newNode = new AST_NODE();
        newNode->TYPE = NODE_VARIABLE;
    }

    AST_NODE *parse()
    {
        AST_NODE *ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;

        while (current->TYPE != EOF)
        {
            switch (current->TYPE)
            {
            case TOKEN_IDENTIFIER:
            {
                ROOT->SUB_STATEMENTS.push_back(parseID());
                break;
            }
            case TOKEN_KEYWORD_INT: // COME BACK, WE HAVE MULTIPLE KEYWORDS
            {
                ROOT->SUB_STATEMENTS.push_back(parseKeywordInt());
                break;
            }
            case TOKEN_KEYWORD_PRINT:
                ROOT->SUB_STATEMENTS.push_back(parseKeywordPrint());
                break;
            case TOKEN_EOF:
                ROOT->SUB_STATEMENTS.push_back(parseKeywordEof());
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