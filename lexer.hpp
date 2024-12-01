#ifndef _LEXER_HPP
#define _LEXER_HPP
#include<string>

enum tokenType{
    TOKEN_ID,
    TOKEN_INTEGER_ID,
    TOKEN_DOUBLE_ID,
    TOKEN_CHAR_ID,
    TOKEN_STR_ID,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,


};

struct Token{
    enum tokenType TYPE;
    std::string value;
};

class Lexer{
public:
    Lexer(std::string);
    
    void checkAndSkip();
    char advanceCursor();
    char peakAhead(int);
    bool matchKeyword(const std::string&);
    void consumeKeyword(std::string);


    Token* tokenizeIntegerID();
    Token* tokenizeDoubleID();
    Token* tokenizeCharID();
    Token* tokenizeStrID();

    std::vector<Token*> tokenize();
    
private:
    std::string source;
    int cursor;
    char current;
    int size;
};


#endif