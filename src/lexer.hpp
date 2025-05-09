#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

//
// Enum listing all possible token types recognized by the lexer.
//
enum tokenType
{
    // Identifiers
    TOKEN_IDENTIFIER, // User-defined identifier

    // Literals
    TOKEN_INTEGER_VAL, // Integer literals
    TOKEN_DOUBLE_VAL,  // Double (floating-point) literals
    TOKEN_CHAR_VAL,    // Character literals
    TOKEN_STRING_VAL,  // String literals
    TOKEN_BOOL_VALUE,  // Bool literals

    // Basic Operators
    TOKEN_EQUALS,           // Assignment '='
    TOKEN_OPERATOR_ADD,     // Addition '+'
    TOKEN_OPERATOR_SUBT,    // Subtraction '-'
    TOKEN_OPERATOR_MULT,    // Multiplication '*'
    TOKEN_OPERATOR_DIV,     // Division '/'
    TOKEN_OPERATOR_MODULUS, // Modulus '%'

    // Punctuation
    TOKEN_SEMICOLON,   // Semicolon ';'
    TOKEN_LEFT_PAREN,  // Left parenthesis '('
    TOKEN_RIGHT_PAREN, // Right parenthesis ')'

    // Keywords
    TOKEN_KEYWORD_INT,    // 'int' keyword
    TOKEN_KEYWORD_DOUBLE, // 'double' keyword
    TOKEN_KEYWORD_CHAR,   // 'char' keyword
    TOKEN_KEYWORD_STR,    // 'str' keyword
    TOKEN_KEYWORD_PRINT,  // 'print' keyword
    TOKEN_KEYWORD_INPUT,  //'input' keyword
    TOKEN_INPUT_TYPE,     //<type>
    TOKEN_KEYWORD_BOOL,   //'bool' keyword
    TOKEN_KEYWORD_RESULT, //"result" keywords

    // newline operator
    TOKEN_NL_SYMBOL, //'...' new line

    // Special Tokens
    TOKEN_EOF, // End of file/input

    // Comparison Operators
    TOKEN_OPERATOR_LESSTHAN,      // '<'
    TOKEN_OPERATOR_GREATERTHAN,   // '>'
    TOKEN_OPERATOR_LESS_EQUAL,    // '<='
    TOKEN_OPERATOR_GREATER_EQUAL, // '>='
    TOKEN_OPERATOR_EQUALS,        // '='
    TOKEN_OPERATOR_DOESNT_EQUAL,  //'=/='

    // Braces
    TOKEN_RIGHT_CURL, // Right curly brace '}'
    TOKEN_LEFT_CURL,  // Left curly brace '{'

    // Control Flow
    TOKEN_KEYWORD_IF,    // 'if' keyword
    TOKEN_KEYWORD_ELSE,  // 'else' keyword
    TOKEN_KEYWORD_FOR,   // 'for' keyword
    TOKEN_KEYWORD_BEGIN, // 'begin' keyword (if you support block-starts)

    // Compound Assignment and Unary Operators
    TOKEN_OPERATOR_ADD_ASSIGN, // '+='
    TOKEN_OPERATOR_INCREMENT,  // '++'
    TOKEN_OPERATOR_DECREMENT,  // '--'

    // Miscellaneous
    TOKEN_OPERATOR_NEWLINE, // Newline as a token (if needed)
    TOKEN_SPACESHIP,        // '=>' (spaceship operator)
    TOKEN_KEYWORD_FUNCTION, // 'function' keyword
    TOKEN_FUNCTION_PARAMS,  // Special token for function parameters
    TOKEN_COMMA,            // PARAM seperator

    // Commenting
    TOKEN_SINGLELINE_COMMENT, // Single line comment
    TOKEN_MULTILINE_COMMENT,  // Multiline comments

    // While Loops
    TOKEN_KEYWORD_CHECK, // Keyword 'repeat'

    TOKEN_KEYWORD_ELEMENT,
    TOKEN_DOT,
    TOKEN_KEYWORD_REPEAT,
    TOKEN_KEYWORD_RANGE,
    TOKEN_ELEMENT_TYPE,
    TOKEN_ARRAY_INITIALIZER,
    TOKEN_OPERATOR_ARRAYRANGE,
    TOKEN_ARRAY_LENGTH,
    TOKEN_ARRAY_ACCESS,
    TOKEN_ARRAY_LAST_INDEX,
    TOKEN_ARRAY_INSERT,
    TOKEN_ARRAY_REMOVE,
    TOKEN_ARRAY_SORT_ASC,
    TOKEN_ARRAY_SORT_DESC,

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

    Token *processSingleLineComment();
    Token *processMultiLineComment();

    Token *processInputType();

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

    // Handles true/false statements
    Token *processBool();

    // Handles numbers (integers and doubles)
    Token *processNumber();

    // Handles keywords and identifiers
    Token *processKeyword();

    // Checks if the end of the input has been reached
    bool eof() const;

    // The main function to tokenize the entire input
    std::vector<Token *> tokenize();

private:
    std::string source; // The input source code as a string
    int cursor;         // Current position in the source
    char current;       // Current character
    int size;           // Total size of the input

    std::unordered_map<char, tokenType> singleCharMap;
    std::unordered_map<std::string, tokenType> MultiCharMap;
    std::unordered_map<std::string, tokenType> KeywordMap;
    std::unordered_map<std::string, tokenType> TokenMap;

    void initializeLexerMaps();

    bool isArrayType;
};

// Helper function: Returns the name of a token type (for debugging or output)
std::string getTokenTypeName(tokenType);

#endif
