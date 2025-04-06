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
    cursor++;
    if (cursor < size)
    {
        current = source[cursor];
        return current;
    }
    else
    {
        current = '\0';
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
    while (!eof() && (current == ' ' || current == '\n' || current == '\t' || current == '\r'))
    {
        advanceCursor();
    }
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

Token *Lexer::processPrint()
{
    std::string printStatement;
    while (std::isalpha(current) || current == '_')
    {
        printStatement += current;
        advanceCursor();
    }
    if (printStatement == "out_to_console")
    {
        return new Token{TOKEN_KEYWORD_PRINT, printStatement};
    }
    else
    {
        return nullptr;
    }
}

Token *Lexer::processOperator()
{
    std::string op(1, current);
    advanceCursor();

    // Handle two-character operators
    if (op == "=" && current == '=')
    {
        advanceCursor();
        return new Token{TOKEN_OPERATOR_EQUALS, "=="};
    }
    else if (op == "+" && current == '=')
    {
        advanceCursor();
        return new Token{TOKEN_OPERATOR_ADD_ASSIGN, "+="};
    }
    else if (op == "+" && current == '+')
    {
        advanceCursor();
        return new Token{TOKEN_OPERATOR_INCREMENT, "++"};
    }
    else if (op == "-" && current == '-')
    {
        advanceCursor();
        return new Token{TOKEN_OPERATOR_DECREMENT, "--"};
    }
    else if (op == "<" && current == '=')
    {
        advanceCursor();
        return new Token{TOKEN_OPERATOR_LESS_EQUAL, "<="};
    }
    else if (op == ">" && current == '=')
    {
        advanceCursor();
        return new Token{TOKEN_OPERATOR_GREATER_EQUAL, ">="};
    }

    // Single-character operators
    if (op == "+")
        return new Token{TOKEN_OPERATOR_ADD, "+"};
    if (op == "-")
        return new Token{TOKEN_OPERATOR_SUBT, "-"};
    if (op == "*")
        return new Token{TOKEN_OPERATOR_MULT, "*"};
    if (op == "/")
        return new Token{TOKEN_OPERATOR_DIV, "/"};
    if (op == "=")
        return new Token{TOKEN_EQUALS, "="};
    if (op == ";")
        return new Token{TOKEN_SEMICOLON, ";"};
    if (op == "(")
        return new Token{TOKEN_LEFT_PAREN, "("};
    if (op == ")")
        return new Token{TOKEN_RIGHT_PAREN, ")"};
    if (op == "<")
        return new Token{TOKEN_OPERATOR_LESSTHAN, "<"};
    if (op == ">")
        return new Token{TOKEN_OPERATOR_GREATERTHAN, ">"};
    if (op == "{")
        return new Token{TOKEN_LEFT_CURL, "{"};
    if (op == "}")
        return new Token{TOKEN_RIGHT_CURL, "}"};
    if (op == ".")
        return new Token{TOKEN_DOT, "."};

    throw std::runtime_error("Error: Unknown operator: " + op);
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
        advanceCursor(); // Skip first single quote
        char charValue = current;
        advanceCursor(); // Move to second quote

        if (current != '\'')
        {
            throw std::runtime_error("Error: Invalid character literal.");
        }

        advanceCursor(); // Move past closing quote
        return new Token{TOKEN_CHAR_VAL, std::string(1, charValue)};
    }
    throw std::runtime_error("Error: Invalid character literal format.");
}
Token *Lexer::processKeyword(std::vector<Token *> &tokens)
{
    std::string keyword;

    // Collect the full keyword or identifier
    while (std::isalpha(current) || current == '_')
    {
        keyword += current;
        advanceCursor();
    }
    // std::cout << "Processing Keyword: " << keyword << std::endl;
    //  Explicitly map keywords to their specific token types
    if (keyword == "int")
    {
        return new Token{TOKEN_KEYWORD_INT, keyword};
    }
    else if (keyword == "double")
    {
        return new Token{TOKEN_KEYWORD_DOUBLE, keyword};
    }
    else if (keyword == "str")
    {
        return new Token{TOKEN_KEYWORD_STR, keyword};
    }
    else if (keyword == "char")
    {
        return new Token{TOKEN_KEYWORD_CHAR, keyword};
    }
    else if (keyword == "out_to_print")
    {
        return new Token{TOKEN_KEYWORD_PRINT, keyword};
    }
    else if (keyword == "if")
    {
        return new Token{TOKEN_KEYWORD_IF, keyword};
    }
    else if (keyword == "else")
    {
        return new Token{TOKEN_KEYWORD_ELSE, keyword};
    }
    else if (keyword == "for")
    {
        return new Token{TOKEN_KEYWORD_FOR, keyword};
    }
    else if (keyword == "end")
    {
        return new Token{TOKEN_EOF, keyword};
    }
    else
    {
        // If it doesn't match a known keyword, treat it as an identifier
        return new Token{TOKEN_IDENTIFIER, keyword};
    }
}
std::vector<Token *> Lexer::tokenize()
{
    std::vector<Token *> tokens;

    while (!eof())
    {
        checkAndSkip();
        // Debug print
        // std::cout << "Processing character: '" << current << "'" << std::endl;

        if (std::isalpha(current))
        {

            Token *token = processKeyword(tokens);
            tokens.push_back(token);
        }
        else if (std::isdigit(current))
        {
            Token *token = processNumber();
            tokens.push_back(token);
        }
        else if (std::ispunct(current))
        {
            if (current == '"')
            {
                Token *token = processStringLiteral();
                tokens.push_back(token);
            }
            else if (current == '\'')
            {
                Token *token = processCharLiteral();
                tokens.push_back(token);
            }
            else
            {
                Token *token = processOperator();
                tokens.push_back(token);
            }
        }
        else
        {
            throw std::runtime_error("Error: Unexpected character: " + std::string(1, current));
        }
    }

    return tokens;
}

// helper function
std::string getTokenTypeName(tokenType type)
{
    switch (type)
    {
    case TOKEN_ID:
        return "TOKEN_ID";
    case TOKEN_INTEGER_VAL:
        return "TOKEN_INTEGER_VAL";
    case TOKEN_DOUBLE_VAL:
        return "TOKEN_DOUBLE_VAL";
    case TOKEN_CHAR_VAL:
        return "TOKEN_CHAR_VAL";
    case TOKEN_STRING_VAL:
        return "TOKEN_STRING_VAL";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_OPERATOR_ADD:
        return "TOKEN_OPERATOR_ADD";
    case TOKEN_OPERATOR_SUBT:
        return "TOKEN_OPERATOR_SUBT";
    case TOKEN_OPERATOR_MULT:
        return "TOKEN_OPERATOR_MULT";
    case TOKEN_OPERATOR_DIV:
        return "TOKEN_OPERATOR_DIV";
    case TOKEN_DOT:
        return "TOKEN_DOT";
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
    case TOKEN_LEFT_PAREN:
        return "TOKEN_LEFT_PAREN";
    case TOKEN_RIGHT_PAREN:
        return "TOKEN_RIGHT_PAREN";
    case TOKEN_KEYWORD_INT:
        return "TOKEN_KEYWORD_INT";
    case TOKEN_KEYWORD_DOUBLE:
        return "TOKEN_KEYWORD_DOUBLE";
    case TOKEN_KEYWORD_CHAR:
        return "TOKEN_KEYWORD_CHAR";
    case TOKEN_KEYWORD_STR:
        return "TOKEN_KEYWORD_STR";
    case TOKEN_KEYWORD_PRINT:
        return "TOKEN_KEYWORD_PRINT";
    case TOKEN_IDENTIFIER:
        return "TOKEN_IDENTIFIER";
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_OPERATOR_LESSTHAN:
        return "TOKEN_OPERATOR_LESSTHAN";
    case TOKEN_OPERATOR_GREATERTHAN:
        return "TOKEN_OPERATOR_GREATERTHAN";
    case TOKEN_RIGHT_CURL:
        return "TOKEN_RIGHT_CURL";
    case TOKEN_LEFT_CURL:
        return "TOKEN_LEFT_CURL";
    case TOKEN_KEYWORD_IF:
        return "TOKEN_KEYWORD_IF";
    case TOKEN_KEYWORD_ELSE:
        return "TOKEN_KEYWORD_ELSE";
    case TOKEN_OPERATOR_LESS_EQUAL:
        return "TOKEN_OPERATOR_LESS_EQUAL";
    case TOKEN_OPERATOR_GREATER_EQUAL:
        return "TOKEN_OPERATOR_GREATER_EQUAL";
    case TOKEN_OPERATOR_EQUALS:
        return "TOKEN_OPERATOR_EQUALS";
    case TOKEN_OPERATOR_ADD_ASSIGN:
        return "TOKEN_OPERATOR_ADD_ASSIGN";
    case TOKEN_KEYWORD_FOR:
        return "TOKEN_OPERATOR_FOR";
    case TOKEN_OPERATOR_INCREMENT:
        return "TOKEN_OPERATOR_INCREMENT";
    case TOKEN_OPERATOR_DECREMENT:
        return "TOKEN_OPERATOR_DECREMENT";
    default:
        return "Error, unknown token identifier";
    }
}
