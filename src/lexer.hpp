#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <sstream>

//
// Enum listing all possible token types recognized by the lexer.
//
enum tokenType
{
    // Identifiers
    TOKEN_IDENTIFIER = 0, // User-defined identifier

    // Literals
    TOKEN_INTEGER_VAL = 1, // Integer literals
    TOKEN_DOUBLE_VAL = 2,  // Double (floating-point) literals
    TOKEN_CHAR_VAL = 3,    // Character literals
    TOKEN_STRING_VAL = 4,  // String literals

    // Basic Operators
    TOKEN_EQUALS = 5,            // Assignment '='
    TOKEN_OPERATOR_ADD = 6,      // Addition '+'
    TOKEN_OPERATOR_SUBT = 7,     // Subtraction '-'
    TOKEN_OPERATOR_MULT = 8,     // Multiplication '*'
    TOKEN_OPERATOR_DIV = 9,      // Division '/'
    TOKEN_OPERATOR_MODULUS = 38, // Modulus '%'

    // Punctuation
    TOKEN_SEMICOLON = 10,   // Semicolon ';'
    TOKEN_LEFT_PAREN = 11,  // Left parenthesis '('
    TOKEN_RIGHT_PAREN = 12, // Right parenthesis ')'

    // Keywords
    TOKEN_KEYWORD_INT = 13,    // 'int' keyword
    TOKEN_KEYWORD_DOUBLE = 14, // 'double' keyword
    TOKEN_KEYWORD_CHAR = 15,   // 'char' keyword
    TOKEN_KEYWORD_STR = 16,    // 'str' keyword
    TOKEN_KEYWORD_PRINT = 17,  // 'print' keyword
    TOKEN_KEYWORD_BOOL = 41,   //'bool' keyword

    // newline operator
    TOKEN_NL_SYMBOL = 40, //'...' new line

    // Special Tokens
    TOKEN_EOF = 18, // End of file/input

    // Comparison Operators
    TOKEN_OPERATOR_LESSTHAN = 19,      // '<'
    TOKEN_OPERATOR_GREATERTHAN = 20,   // '>'
    TOKEN_OPERATOR_LESS_EQUAL = 21,    // '<='
    TOKEN_OPERATOR_GREATER_EQUAL = 22, // '>='
    TOKEN_OPERATOR_EQUALS = 23,        // '='
    TOKEN_OPERATOR_DOESNT_EQUAL = 39,  //'=/='

    // Braces
    TOKEN_RIGHT_CURL = 24, // Right curly brace '}'
    TOKEN_LEFT_CURL = 25,  // Left curly brace '{'

    // Control Flow
    TOKEN_KEYWORD_IF = 26,    // 'if' keyword
    TOKEN_KEYWORD_ELSE = 27,  // 'else' keyword
    TOKEN_KEYWORD_FOR = 28,   // 'for' keyword
    TOKEN_KEYWORD_BEGIN = 29, // 'begin' keyword (if you support block-starts)

    // Compound Assignment and Unary Operators
    TOKEN_OPERATOR_ADD_ASSIGN = 30, // '+='
    TOKEN_OPERATOR_INCREMENT = 31,  // '++'
    TOKEN_OPERATOR_DECREMENT = 32,  // '--'

    // Miscellaneous
    TOKEN_OPERATOR_NEWLINE = 33, // Newline as a token (if needed)
    TOKEN_SPACESHIP = 34,        // '=>' (spaceship operator)
    TOKEN_KEYWORD_FUNCTION = 35, // 'function' keyword
    TOKEN_FUNCTION_PARAMS = 36,  // Special token for function parameters
    TOKEN_COMMA = 37             // PARAM seperator
};

//
// Token structure representing a lexeme found in the input.
//
struct Token
{
    enum tokenType TYPE; // The type of token
    std::string value;   // The actual text (lexeme) corresponding to the token
};

//
// Lexer class
// Responsible for reading the source input and splitting it into tokens.
//
class Lexer
{
public:
    // Constructor: Takes the input source string to be tokenized
    Lexer(std::string);

    // Skips any whitespace and updates the current character
    void checkAndSkip();

    // Advances the cursor by one character and returns the next character
    char advanceCursor();

    // Looks ahead in the source without advancing the cursor
    char peakAhead(int);

    // Checks if the current position matches a specific keyword
    bool matchKeyword(const std::string &);

    // Consumes the keyword if it matches
    void consumeKeyword(const std::string &);

    // Processors for different token types:

    // Handles string literals ("example")
    Token *processStringLiteral();

    // Handles special one-character tokens (like '(', ')', ';', etc.)
    Token *tokenizeSPECIAL(enum tokenType);

    // Handles 'print' statements
    Token *processPrint();

    // Handles character literals ('a')
    Token *processCharLiteral();

    // Handles operators (+, -, *, /, etc.)
    Token *processOperator();

    // Handles numbers (integers and doubles)
    Token *processNumber();

    // Handles keywords and identifiers
    Token *processKeyword(std::vector<Token *> &);

    // Checks if the end of the input has been reached
    bool eof() const;

    // The main function to tokenize the entire input
    std::vector<Token *> tokenize();

private:
    std::string source; // The input source code as a string
    int cursor;         // Current position in the source
    char current;       // Current character
    int size;           // Total size of the input
};

// Helper function: Returns the name of a token type (for debugging or output)
std::string getTokenTypeName(tokenType);

#endif
