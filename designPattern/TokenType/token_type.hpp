#ifndef TOKEN_TYPES_HPP
#define TOKEN_TYPES_HPP

#include <string>
#include <memory>

// Token type enumeration
enum class TokenType
{
    // Identifiers
    IDENTIFIER, // User-defined identifier

    // Literals
    INTEGER_VAL, // Integer literals
    DOUBLE_VAL,  // Double (floating-point) literals
    CHAR_VAL,    // Character literals
    STRING_VAL,  // String literals
    BOOL_VALUE,  // Bool literals

    // Basic Operators
    EQUALS,           // Assignment '='
    OPERATOR_ADD,     // Addition '+'
    OPERATOR_SUBT,    // Subtraction '-'
    OPERATOR_MULT,    // Multiplication '*'
    OPERATOR_DIV,     // Division '/'
    OPERATOR_MODULUS, // Modulus '%'

    // Punctuation
    SEMICOLON,   // Semicolon ';'
    LEFT_PAREN,  // Left parenthesis '('
    RIGHT_PAREN, // Right parenthesis ')'

    // Keywords
    KEYWORD_INT,     // 'int' keyword
    KEYWORD_DOUBLE,  // 'double' keyword
    KEYWORD_CHAR,    // 'char' keyword
    KEYWORD_STR,     // 'str' keyword
    KEYWORD_PRINT,   // 'print' keyword
    KEYWORD_INPUT,   // 'input' keyword
    INPUT_TYPE,      // <type>
    KEYWORD_BOOL,    // 'bool' keyword
    KEYWORD_RESULT,  // "result" keywords
    KEYWORD_CHECK,   //'check' keywords
    KEYWORD_ELEMENT, //'elements' keywords

    ELEMENT_TYPE,

    // Newline operator
    NL_SYMBOL, // '...' new line

    // Special Tokens
    END_OF_FILE, // End of file/input

    // Comparison Operators
    OPERATOR_LESSTHAN,      // '<'
    OPERATOR_GREATERTHAN,   // '>'
    OPERATOR_LESS_EQUAL,    // '<='
    OPERATOR_GREATER_EQUAL, // '>='
    OPERATOR_EQUALS,        // '=='
    OPERATOR_DOESNT_EQUAL,  // '=/='

    // Braces
    RIGHT_CURL, // Right curly brace '}'
    LEFT_CURL,  // Left curly brace '{'

    // Control Flow
    KEYWORD_IF,    // 'if' keyword
    KEYWORD_ELSE,  // 'else' keyword
    KEYWORD_FOR,   // 'for' keyword
    KEYWORD_BEGIN, // 'begin' keyword

    // Compound Assignment and Unary Operators
    OPERATOR_ADD_ASSIGN, // '+='
    OPERATOR_INCREMENT,  // '++'
    OPERATOR_DECREMENT,  // '--'

    // Miscellaneous
    OPERATOR_NEWLINE, // Newline as a token
    SPACESHIP,        // '=>' (spaceship operator)
    KEYWORD_PROC,     // 'function' keyword
    FUNCTION_PARAMS,  // Special token for function parameters
    COMMA,            // PARAM separator

    // Commenting
    SINGLELINE_COMMENT, // Single line comment
    MULTILINE_COMMENT,  // Multiline comments
};

// Token class with better encapsulation
class Token
{
private:
    TokenType type;
    std::string value;

public:
    Token(TokenType type, const std::string &value, int line = 0, int column = 0);

    TokenType getType() const;
    const std::string &getValue() const;
    int getLine() const;
    int getColumn() const;

    // For debugging
    std::string toString() const;
};

// Helper function to convert TokenType to string
std::string getTokenTypeName(TokenType type);

#endif // TOKEN_TYPES_HPP