#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>

#include "lexer.hpp"

/**
 * @brief Constructor for Lexer class
 * @param sourceCode The source code to be tokenized
 *
 * Initializes the lexer with the provided source code and sets up the cursor
 * to begin tokenization from the start of the input.
 */
Lexer::Lexer(std::string sourceCode)
{
    source = sourceCode;
    cursor = 0;
    current = sourceCode.at(cursor);
    size = sourceCode.length();
}

/**
 * @brief Advances the cursor position by one character
 * @return The new current character, or '\0' if end of file is reached
 *
 * Moves the cursor one position forward in the source code and updates
 * the current character. Returns '\0' if cursor moves past the end of the source.
 */
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

/**
 * @brief Checks if the upcoming characters match a given keyword
 * @param keyword The keyword to match against
 * @return true if the keyword matches, false otherwise
 *
 * Compares characters starting from the current cursor position
 * with the given keyword without advancing the cursor.
 */
bool Lexer::matchKeyword(const std::string &keyword)
{
    if (cursor + keyword.size() > static_cast<size_t>(size))
    {
        return false;
    }
    return source.substr(cursor, keyword.size()) == keyword;
}

/**
 * @brief Consumes a keyword if it matches the upcoming characters
 * @param keyword The keyword to consume
 * @throws std::runtime_error if the keyword doesn't match
 *
 * Verifies that the upcoming characters match the keyword and advances
 * the cursor past the keyword if it matches. Throws an error otherwise.
 */
void Lexer::consumeKeyword(const std::string &keyword)
{
    if (!matchKeyword(keyword))
    {
        throw std::runtime_error("Error: Keyword mismatch during consumeKeyword.");
    }
    cursor += keyword.length();
    current = (cursor < size) ? source[cursor] : '\0';
}

/**
 * @brief Looks ahead in the source code without advancing the cursor
 * @param offset Number of characters to look ahead
 * @return Character at cursor+offset position, or '\0' if out of bounds
 *
 * Safely peeks at a character ahead of the current cursor position.
 */
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

/**
 * @brief Skips whitespace characters (spaces, tabs, newlines)
 *
 * Advances the cursor until a non-whitespace character is found
 * or the end of file is reached.
 */
void Lexer::checkAndSkip()
{
    while (!eof() && (current == ' ' || current == '\n' || current == '\t' || current == '\r'))
    {
        advanceCursor();
    }
}

/**
 * @brief Checks if the cursor has reached the end of the source code
 * @return true if end of file reached, false otherwise
 */
bool Lexer::eof() const
{
    return cursor >= size;
}

/**
 * @brief Processes true or false statements
 * @return Token pointer representing the true or false value
 * @throws std::runtime error if true or false is invalid
 */
Token *Lexer::processBool()
{
    std::string boolStr = "";
    while (isalpha(current) && current != ';')
    {
        boolStr += current;
        advanceCursor();
    }

    if (boolStr == "true")
    {
        return new Token{TOKEN_BOOL_VALUE, boolStr};
    }
    else if (boolStr == "false")
    {
        return new Token{TOKEN_BOOL_VALUE, boolStr};
    }
    else
    {
        throw std::runtime_error("Invalid boolean literal: " + boolStr);
    }
}

/**
 * @brief Processes numeric literals (integers and floating-point numbers)
 * @return Token pointer representing the numeric value
 * @throws std::runtime_error if the number format is invalid
 *
 * Parses integer and floating-point literals from the source code.
 * Handles decimal points and validates the number format.
 */
Token *Lexer::processNumber()
{
    std::string number;
    bool isDouble = false;

    // Collect digits and at most one decimal point
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

    // Create appropriate token based on whether a decimal point was found
    return new Token{isDouble ? TOKEN_DOUBLE_VAL : TOKEN_INTEGER_VAL, number};
}

/**
 * @brief Processes print keyword
 * @return Token pointer for print keyword or nullptr if not a print statement
 *
 * Checks if the current identifier is the print keyword ("out_to_console").
 */
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

/**
 * @brief Processes operators and punctuation
 * @return Token pointer representing the operator
 * @throws std::runtime_error if the operator is unknown
 *
 * Handles both single-character operators (like +, -, *, /) and
 * multi-character operators (like ==, +=, ++, --).
 */
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
    else if (op == "=" && current == '>')
    {
        advanceCursor();
        return new Token{TOKEN_SPACESHIP, "=>"};
    }
    else if (op == "=" && current == '/' && peakAhead(1) == '=')
    {
        advanceCursor();
        advanceCursor();
        return new Token{TOKEN_OPERATOR_DOESNT_EQUAL, "=/="};
    }
    else if (op == "." && current == '.' && peakAhead(1) == '.')
    {
        advanceCursor();
        advanceCursor();
        return new Token{TOKEN_NL_SYMBOL, "..."};
    }

    // Single-character operators
    // Using a more structured approach with a lookup table could be cleaner,
    // but this switch-like approach works well for clarity
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
    if (op == ",")
        return new Token{TOKEN_COMMA, ","};
    if (op == "%")
        return new Token{TOKEN_OPERATOR_MODULUS, "%"};

    throw std::runtime_error("Error: Unknown operator: " + op);
}

/**
 * @brief Processes string literals enclosed in double quotes
 * @return Token pointer representing the string value
 * @throws std::runtime_error if the string is unterminated
 *
 * Handles string literals by collecting characters between double quotes.
 */
Token *Lexer::processStringLiteral()
{
    if (current == '"')
    {
        advanceCursor(); // Skip opening quote
        std::string value;

        // Collect characters until closing quote or EOF
        while (current != '"' && !eof())
        {
            value += current;
            advanceCursor();
        }

        // Verify string was properly terminated
        if (current != '"')
        {
            throw std::runtime_error("Error: Unterminated String Literal.");
        }
        advanceCursor(); // Skip closing quote
        return new Token{TOKEN_STRING_VAL, value};
    }
    else
    {
        throw std::runtime_error("Error: Invalid string literal.");
    }
}

/**
 * @brief Processes character literals enclosed in single quotes
 * @return Token pointer representing the character value
 * @throws std::runtime_error if the character literal is invalid
 *
 * Handles both regular character literals ('a') and special
 * escape sequences like '\n' for newline.
 */
Token *Lexer::processCharLiteral()
{
    // Regular character literal: 'c'
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
    // Special case for newline: '\n'
    else if (current == '\'' && peakAhead(1) == '\\' && peakAhead(2) == 'n' && peakAhead(3) == '\'')
    {
        advanceCursor();             // Skip first single quote
        char newLineValue = current; // Backslash character
        advanceCursor();             // Skip 'n'
        advanceCursor();             // Move to closing quote

        if (current != '\'')
        {
            throw std::runtime_error("Error: Invalid character literal.");
        }
        advanceCursor(); // Move past closing quote
        return new Token{TOKEN_OPERATOR_NEWLINE, std::string(1, newLineValue)};
    }

    throw std::runtime_error("Error: Invalid character literal format.");
}

/**
 * @brief Processes keywords and identifiers
 * @param tokens Vector of tokens processed so far (unused)
 * @return Token pointer representing the keyword or identifier
 *
 * Identifies language keywords and user-defined identifiers.
 * Special handling for "begin:" with the colon as part of the token.
 */
Token *Lexer::processKeyword(std::vector<Token *> &tokens)
{
    (void)tokens; // Suppress unused parameter warning
    std::string keyword;

    // Collect identifier characters
    while (std::isalpha(current) || current == '_')
    {
        keyword += current;
        advanceCursor();
    }

    // Handle special case for "begin:"
    if (keyword == "begin" && current == ':')
    {
        advanceCursor(); // consume colon
        return new Token{TOKEN_KEYWORD_BEGIN, "begin"};
    }

    // Match against known keywords
    // Could be refactored to use a map or switch for better maintainability
    if (keyword == "proc")
    {
        return new Token{TOKEN_KEYWORD_FUNCTION, keyword};
    }
    else if (keyword == "int")
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
    else if (keyword == "out_to_console")
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
    else if (keyword == "bool")
    {
        return new Token{TOKEN_KEYWORD_BOOL, keyword};
    }
    else
    {
        return new Token{TOKEN_IDENTIFIER, keyword};
    }
}

/**
 * @brief Main tokenization method that processes the entire source code
 * @return Vector of Token pointers representing the tokenized source
 * @throws std::runtime_error if unexpected characters are encountered
 *
 * Processes the entire source code and returns a vector of tokens
 * that can be used by a parser for syntax analysis.
 */
std::vector<Token *> Lexer::tokenize()
{
    std::vector<Token *> tokens;

    while (!eof())
    {
        // Skip whitespace before processing next token
        checkAndSkip();

        // Process different token types based on the current character
        if (std::isalpha(current))
        {
            if ((current == 't' && matchKeyword("true")) ||
                (current == 'f' && matchKeyword("false")))
            {
                Token *token = processBool();
                tokens.push_back(token);
            }
            else
            {
                // Process keywords and identifiers
                Token *token = processKeyword(tokens);
                tokens.push_back(token);
            }
        }
        else if (std::isdigit(current))
        {
            // Process numeric literals
            Token *token = processNumber();
            tokens.push_back(token);
        }
        else if (std::ispunct(current))
        {
            // Process punctuation and operators
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
        else if (!eof())
        {
            // Handle unexpected characters
            throw std::runtime_error("Error: Unexpected character: " + std::string(1, current));
        }
    }

    return tokens;
}

/**
 * @brief Helper function to get string representation of token types
 * @param type The token type to convert to string
 * @return String representation of the token type
 *
 * Converts the enum token type to a human-readable string for debugging
 * and error reporting purposes.
 */
std::string getTokenTypeName(tokenType type)
{
    switch (type)
    {
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
        return "TOKEN_KEYWORD_FOR";
    case TOKEN_OPERATOR_INCREMENT:
        return "TOKEN_OPERATOR_INCREMENT";
    case TOKEN_OPERATOR_DECREMENT:
        return "TOKEN_OPERATOR_DECREMENT";
    case TOKEN_OPERATOR_NEWLINE:
        return "TOKEN_OPERATOR_NEWLINE";
    case TOKEN_KEYWORD_BEGIN:
        return "TOKEN_KEYWORD_BEGIN";
    case TOKEN_SPACESHIP:
        return "TOKEN_SPACESHIP";
    case TOKEN_KEYWORD_FUNCTION:
        return "TOKEN_KEYWORD_FUNCTION";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_OPERATOR_DOESNT_EQUAL:
        return "TOKEN_OPERATOR_DOESNT_EQUAL";
    case TOKEN_OPERATOR_MODULUS:
        return "TOKEN_OPERATOR_MODULUS";
    case TOKEN_NL_SYMBOL:
        return "TOKEN_NL_SYMBOL";
    case TOKEN_KEYWORD_BOOL:
        return "TOKEN_KEYWORD_BOOL";
    case TOKEN_BOOL_VALUE:
        return "TOKEN_BOOL_VALUE";
    default:
        return "Error, unknown token identifier";
    }
}