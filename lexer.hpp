#ifndef _LEXER_HPP
#define _LEXER_HPP
#include <string>
#include <vector>
#include <sstream>

enum tokenType
{
    TOKEN_ID,
    TOKEN_INTEGER_VAL,
    TOKEN_DOUBLE_VAL,
    TOKEN_CHAR_VAL,
    TOKEN_STRING_VAL,

    TOKEN_EQUALS,
    TOKEN_OPERATOR_ADD,
    TOKEN_OPERATOR_SUBT,
    TOKEN_OPERATOR_MULT,
    TOKEN_OPERATOR_DIV,

    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,

    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_DOUBLE,
    TOKEN_KEYWORD_CHAR,
    TOKEN_KEYWORD_STR,

    TOKEN_IDENTIFIER,
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

    Token *processCharLiteral();
    Token *processOperator();
    Token *processNumber();
    void processKeyword(std::vector<Token *> &);
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