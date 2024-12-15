#include "parser.hpp"
#include <stdexcept>
#include <iostream>

Parser::Parser(std::vector<Token *> tokens) : tokens(std::move(tokens)), current(0) {}

Token *Parser::advance()
{
    if (!check(TOKEN_EOF))
    {
        return tokens[current++];
    }
    return tokens[current];
    std::cout << "Processing current token: " << current << std::endl;
}

Token *Parser::peak()
{
    return tokens[current];
}

bool Parser::match(tokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(tokenType type)
{
    if (current >= tokens.size())
        return false;
    return tokens[current]->TYPE == type;
}

void Parser::consume(tokenType type, const std::string &errorMessage)
{
    if (!match(type))
    {
        throw std::runtime_error(errorMessage);
    }
}

// Parse primary expressions (e.g., literals)
std::unique_ptr<Expression> Parser::parsePrimary()
{
    if (match(TOKEN_INTEGER_VAL) || match(TOKEN_DOUBLE_VAL) || match(TOKEN_STRING_VAL) || match(TOKEN_CHAR_VAL))
    {
        return std::make_unique<Literal>(peak()->value);
    }
    throw std::runtime_error("Error: Expected a literal value.");
}

// Parse binary expressions with operator precedence
std::unique_ptr<Expression> Parser::parseBinary(int precedence)
{
    auto left = parsePrimary();

    while (true)
    {
        if (match(TOKEN_OPERATOR_ADD) || match(TOKEN_OPERATOR_SUBT) ||
            match(TOKEN_OPERATOR_MULT) || match(TOKEN_OPERATOR_DIV))
        {
            std::string op = peak()->value;
            auto right = parsePrimary();
            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }
        else
        {
            break;
        }
    }

    return left;
}

// Parse a print statement
std::unique_ptr<Statement> Parser::parsePrintStatement()
{
    consume(TOKEN_KEYWORD_PRINT, "Error: Expected 'print' keyword.");
    consume(TOKEN_LEFT_PAREN, "Error: Expected '(' after 'print'.");
    auto expr = parseExpression();
    consume(TOKEN_RIGHT_PAREN, "Error: Expected ')' after expression.");
    consume(TOKEN_SEMICOLON, "Error: Expected ';' after print statement.");
    return std::make_unique<printStatement>(std::move(expr));
}

// Parse general statements
std::unique_ptr<Statement> Parser::parseStatement()
{
    if (match(TOKEN_KEYWORD_PRINT))
    {
        return parsePrintStatement();
    }
    throw std::runtime_error("Error: Unknown statement type.");
}

// Parse an expression
std::unique_ptr<Expression> Parser::parseExpression()
{
    return parseBinary(0);
}

// Parse all tokens into AST
std::vector<std::unique_ptr<Statement>> Parser::parse()
{
    std::vector<std::unique_ptr<Statement>> statements;

    while (!check(TOKEN_EOF))
    {
        statements.push_back(parseStatement());
    }

    return statements;
}
