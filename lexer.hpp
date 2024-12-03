#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>
#include <vector>
#include <sstream>

enum tokenType
{
    TOKEN_ID = 0,
    TOKEN_INTEGER_VAL = 1,
    TOKEN_DOUBLE_VAL = 2,
    TOKEN_CHAR_VAL = 3,
    TOKEN_STRING_VAL = 4,

    TOKEN_EQUALS = 5,
    TOKEN_OPERATOR_ADD = 6,
    TOKEN_OPERATOR_SUBT = 7,
    TOKEN_OPERATOR_MULT = 8,
    TOKEN_OPERATOR_DIV = 9,

    TOKEN_SEMICOLON = 10,
    TOKEN_LEFT_PAREN = 11,
    TOKEN_RIGHT_PAREN = 12,

    TOKEN_KEYWORD_INT = 13,
    TOKEN_KEYWORD_DOUBLE = 14,
    TOKEN_KEYWORD_CHAR = 15,
    TOKEN_KEYWORD_STR = 16,

    TOKEN_IDENTIFIER = 17,
};

struct Token
{
    enum tokenType TYPE;
    std::string value;
};

class Lexer
{
public:
    Lexer(std::string);

    void checkAndSkip();
    char advanceCursor();
    char peakAhead(int);
    bool matchKeyword(const std::string &);
    void consumeKeyword(const std::string &);

    Token *processStringLiteral();
    Token *tokenizeSPECIAL(enum tokenType);

    std::string getTokenTypeName(tokenType);
    Token *processCharLiteral();
    Token *processOperator();
    Token *processNumber();
    Token *processKeyword(std::vector<Token *> &);
    bool eof() const;
    Token *tokenizeID(tokenType, const std::string &);

    std::vector<Token *> tokenize();

private:
    std::string source;
    int cursor;
    char current;
    int size;
};

#endif