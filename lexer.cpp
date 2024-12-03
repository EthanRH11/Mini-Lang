#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>

#include "lexer.hpp"

Lexer::Lexer(std::string sourceCode)
{
    source = sourceCode;
    cursor = 0;
    current = sourceCode.at(cursor);
    size = sourceCode.length();
}

char Lexer::advanceCursor()
{
    if (cursor < size)
    {
        current = source[cursor++];
        return current;
    }
    else
    {
        return '\0';
    }
}

bool Lexer::matchKeyword(const std::string &keyword)
{
    if (cursor + keyword.size() > size)
    {
        return false;
    }
    return source.substr(cursor, keyword.size()) == keyword;
}

void Lexer::consumeKeyword(const std::string &keyword)
{
    if (!matchKeyword(keyword))
    {
        throw std::runtime_error("Error: Keyword mismatch during consumeKeyword.");
    }
    cursor += keyword.length();
    current = (cursor < size) ? source[cursor] : '\0';
}

char Lexer::peakAhead(int offset)
{
    if (cursor + offset < size)
    {
        return source[cursor + offset];
    }
    else
    {
        return '\0';
    }
}

void Lexer::checkAndSkip()
{
    while (current == ' ' || current == '\n' || current == '\t' || current == '\r')
    {
        advanceCursor();
    }
}

Token *Lexer::tokenizeID(tokenType type, const std::string &keyword)
{
    consumeKeyword(keyword);
    std::stringstream buffer;
    while (isalnum(current) || current == '-')
    {
        buffer << current;
        advanceCursor();
    }

    Token *newToken = new Token;
    newToken->TYPE = type;
    newToken->value = buffer.str();
    return newToken;
}

bool Lexer::eof() const
{
    return cursor >= size;
}

Token *Lexer::processNumber()
{
    std::string number;
    bool isDouble = false;
    while (std::isdigit(current) || current == '.')
    {
        if (current == '.')
        {
            if (isDouble)
            {
                throw std::runtime_error("Error: Invalid number format with multiple decimals.");
            }
            isDouble = true;
        }
        number += current;
        advanceCursor();
    }
    return new Token{isDouble ? TOKEN_DOUBLE_VAL : TOKEN_INTEGER_VAL, number};
}
Token *Lexer::processOperator()
{
    char symbol = current;
    advanceCursor();

    switch (symbol)
    {
    case '+':
        return new Token{TOKEN_OPERATOR_ADD, "+"};
    case '-':
        return new Token{TOKEN_OPERATOR_SUBT, "-"};
    case '*':
        return new Token{TOKEN_OPERATOR_MULT, "*"};
    case '/':
        return new Token{TOKEN_OPERATOR_DIV, "/"};
    case '(':
        return new Token{TOKEN_LEFT_PAREN, "("};
    case ')':
        return new Token{TOKEN_RIGHT_PAREN, ")"};
    case ';':
        return new Token{TOKEN_SEMICOLON, ";"};
    case '=':
        return new Token{TOKEN_EQUALS, "="};
    default:
        throw std::runtime_error("Error: Unknown symbol or operator: " + std::string(1, symbol));
    }
}

Token *Lexer::processStringLiteral()
{
    if (current == '"')
    {
        advanceCursor();
        std::string value;
        while (current != '"' && !eof())
        {
            value += current;
            advanceCursor();
        }

        if (current != '"')
        {
            throw std::runtime_error("Error: Unterminated String Literal.");
        }
        advanceCursor();
        return new Token{TOKEN_STRING_VAL, value};
    }
    else
    {
        throw std::runtime_error("Error: Invalid string literal.");
    }
}
Token *Lexer::processCharLiteral()
{
    if (current == '\'' && peakAhead(1) != '\'' && peakAhead(2) == '\'')
    {
        advanceCursor();
        char charValue = current;
        advanceCursor();
        advanceCursor();

        return new Token{TOKEN_CHAR_VAL, std::string(1, charValue)};
    }
}

void Lexer::processKeyword(std::vector<Token *> &tokens)
{
    std::string keyword;

    // Build the keyword string as long as it's alphanumeric or a dash.
    while (std::isalnum(current) || current == '-')
    {
        keyword += current;
        advanceCursor();
    }

    // Check if the keyword matches one of the known keywords.
    if (keyword == "int")
    {
        tokens.push_back(new Token{TOKEN_KEYWORD_INT, "int"});
    }
    else if (keyword == "double")
    {
        tokens.push_back(new Token{TOKEN_KEYWORD_DOUBLE, "double"});
    }
    else if (keyword == "str")
    {
        tokens.push_back(new Token{TOKEN_KEYWORD_STR, "str"});
    }
    else if (keyword == "char")
    {
        tokens.push_back(new Token{TOKEN_KEYWORD_CHAR, "char"});
    }
    else
    {
        // If it doesn't match a keyword, treat it as an identifier
        tokens.push_back(new Token{TOKEN_IDENTIFIER, keyword});
    }
}

std::vector<Token *> Lexer::tokenize()
{
    std::vector<Token *> tokens;

    while (!eof())
    {
        checkAndSkip();

        if (std::isalpha(current)) // If it's an alphabetic character, it's a keyword or identifier
        {
            processKeyword(tokens);
        }
        else if (std::isdigit(current)) // If it's a digit, it's a number
        {
            tokens.push_back(processNumber());
        }
        else if (current == '\'') // Handle character literals
        {
            tokens.push_back(processCharLiteral());
        }
        else if (current == '"') // Handle string literals
        {
            tokens.push_back(processStringLiteral());
        }
        else if (std::ispunct(current)) // Handle operators and punctuation
        {
            tokens.push_back(processOperator());
        }
        else
        {
            throw std::runtime_error("Error: Unexpected character: " + std::string(1, current));
        }
    }

    return tokens;
}