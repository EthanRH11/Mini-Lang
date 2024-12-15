#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <memory>

#include "lexer.hpp"

/*Define AST node types*/
struct ASTnode
{
    virtual ~ASTnode() {}
};

struct Expression : ASTnode
{
};
struct BinaryExpression : Expression
{
    std::string op;
    std::unique_ptr<Expression> left, right;

    BinaryExpression(std::string o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};
struct Literal : Expression
{
    std::string value;
    Literal(const std::string &v) : value(v) {}
};
struct Statement : ASTnode
{
};
struct printStatement : Statement
{
    std::unique_ptr<Expression> expression;

    printStatement(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}
};

class Parser
{
public:
    Parser(std::vector<Token *> tokens);
    std::vector<std::unique_ptr<Statement>> parse();

private:
    std::vector<Token *> tokens;
    int current;

    Token *advance();
    Token *peak();
    bool match(tokenType type);
    bool check(tokenType type);
    void consume(tokenType type, const std::string &errorMessage);

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parsePrimary();
    std::unique_ptr<Expression> parseBinary(int precedence);
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parsePrintStatement();
};

// enum NODE_TYPE
// {
//     NODE_ROOT,
//     NODE_ID,
//     NODE_RETURN,
//     NODE_PRINT,
//     NODE_INT,
// };

// struct AST_NODE
// {

//     enum NODE_TYPE TYPE;
//     std::string *VALUE;
//     AST_NODE *child;
//     std::vector<AST_NODE *> SUB_STATEMENTS; // FOR ROOT NODE
// };

// class Parser
// {
// public:
//     Parser(std::vector<Token *> tokens)
//     {
//         parserToken = tokens;
//         index = 0;
//         current = parserToken.at(index);
//         int limit = parserToken.size();
//     }

//     /*Helper functions to check if the current token matches an expected type or value*/
//     bool match(tokenType type)
//     {
//         return current && current->TYPE == type;
//     }
//     bool expect(tokenType type)
//     {
//         if (match(type))
//         {
//             proceed();
//             return true;
//         }
//         return false;
//     }

//     Token *proceed()
//     {
//         if (index < limit - 1)
//         {
//             index++;
//             current = parserToken.at(index);
//             return current;
//         }
//         return nullptr; // If we've reached the end
//     }

//     AST_NODE *parseStatement();
//     AST_NODE *parseExpression();
//     AST_NODE* parseIfStatement();
//     AST_NODE* parseForloop();

//     AST_NODE *parse()
//     {
//         AST_NODE *ROOT = new AST_NODE();
//         ROOT->TYPE = NODE_ROOT;

//         while (index < limit)
//         {
//             AST_NODE *statement = parseStatement();
//             if (statement)
//             {
//                 ROOT->SUB_STATEMENTS.push_back(statement); // attach statement to root
//             }
//             else
//             {
//                 std::cerr << "Error: Failed to parse statement." << std::endl;
//                 break;
//             }
//         }
//         return ROOT;
//     }

// private:
//     int index;
//     int limit;
//     Token *current;
//     std::vector<Token *> parserToken;
// };

#endif