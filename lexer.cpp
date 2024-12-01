#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<vector>
#include<fstream>

#include"lexer.hpp"

Lexer::Lexer(std::string sourceCode){
    source = sourceCode;
    cursor = 0;
    current = sourceCode.at(cursor);
    size = sourceCode.length();
}

char Lexer::advanceCursor(){
    if(cursor < size){
        char temp = current;
        ++cursor;
        cursor = (cursor < size) ? source[cursor] : '\0';
    } else {
        return '\0';
    }
}

bool Lexer::matchKeyword(const std::string& keyword){
    if(cursor + keyword.size() > size){
        return false;
    }
    return source.substr(cursor, keyword.size()) == keyword;
}

void Lexer::consumeKeyword(const char* keyword){
    int len = strlen(keyword);
    for(int i = 0; i < len; ++i){
        advanceCursor();
    }
}

char Lexer::peakAhead(int offset){
    if(cursor + offset < size){
        return source[cursor + offset];
    } else {
        return '\0';
    }
}

void Lexer::checkAndSkip(){
    while(current == ' ' || current == '\n' || current == '\t' || current == '\r'){
        advanceCursor();
    }
}

Token* Lexer::tokenizeIntegerID(){
    consumeKeyword("int");

    std::stringstream buffer;
    while(isalnum(current) || current == '_'){
        buffer << current;
        advanceCursor();
    }

    Token* newToken = new Token;
    newToken->TYPE = TOKEN_INTEGER_ID;
    newToken->value = buffer.str();
    return newToken;
}

Token* Lexer::tokenizeDoubleID(){
    consumeKeyword("double");

    std::stringstream buffer;
    while(isalnum(current) || current == '_'){
        buffer << current;
        advanceCursor();
    }

    Token* newToken = new Token;
    newToken->TYPE = TOKEN_DOUBLE_ID;
    newToken->value = buffer.str();
    return newToken;

}

Token* Lexer::tokenizeCharID(){
    consumeKeyword("char");

    std::stringstream buffer;
    while(isalnum(current) || current == '_'){
        buffer << current;
        advanceCursor();
    }

    Token* newToken = new Token;
    newToken->TYPE = TOKEN_CHAR_ID;
    newToken->value = buffer.str();
    return newToken;
}

Token* Lexer::tokenizeStrID(){
    consumeKeyword("str");

    std::stringstream buffer;
    while(isalnum(current) || current == '_'){
        buffer << current;
        advanceCursor();
    }

    Token* newToken = new Token;
    newToken->TYPE = TOKEN_STR_ID;
    newToken->value = buffer.str();
    return newToken;
}

Token* Lexer::tokenizeIntegerVal(){
    std::stringstream buffer;
    while(isdigit(current)){
        buffer << current;
        advanceCursor();
    }
        Token* newToken = new Token;
        newToken->TYPE = TOKEN_INTEGER_VAL;
        newToken->value = buffer.str();
        return newToken;
}
Token* Lexer::tokenizeDoubleVal(){
    std::stringstream buffer;
    while(isdigit(current)){
        buffer << current;
        advanceCursor();
    }
    Token* newToken = new Token;
    newToken->TYPE = TOKEN_DOUBLE_VAL;
    newToken->value = buffer.str();
    return newToken;
}

Token* Lexer::tokenizeCharVal(){
    std::stringstream buffer;
    while(isalpha(current)){
        buffer << current;
        advanceCursor();
    }

    Token* charToken = new Token;
    charToken->TYPE = TOKEN_CHAR_VAL;
    charToken->value = buffer.str();
    return charToken;
}

Token* Lexer::tokenizeStrVal(){
    std::stringstream buffer;
    while(isalpha(current)){
        buffer << current;
        advanceCursor();
    }

    Token* strToken = new Token;
    strToken->TYPE = TOKEN_STR_VAL;
    strToken->value = buffer.str();
    return strToken;
}

Token* Lexer::tokenizeSPECIAL(enum tokenType TYPE){
    Token* newToken = new Token;
    newToken->TYPE = TYPE;
    newToken->value = std::string(1, advanceCursor());
    return newToken;
}

bool Lexer::eof() const{
    return cursor >= size;
}

std::vector<Token*> Lexer::tokenize(){
    std::vector<Token*> tokens;
    Token* token;

        while(cursor < size && !eof()){
            checkAndSkip();

            if(matchKeyword("int")){
                Token* intToken = new Token;
                intToken->TYPE = TOKEN_KEYWORD_INT;
                intToken->value = "int";
                tokens.push_back(intToken);

                checkAndSkip();

                tokens.push_back(tokenizeIntegerID());
                checkAndSkip();

                if(current == '='){
                    advanceCursor();
                    checkAndSkip();

                    if(isdigit(current)){
                        tokens.push_back(tokenizeIntegerVal());
                    } else {
                        throw std::runtime_error("Expected integer value after '='");
                    }
                }
                continue;
            } else if(matchKeyword("double")){
            Token* doubleToken = new Token;
            doubleToken->TYPE = TOKEN_KEYWORD_DOUBLE;
            doubleToken->value = "double";
            tokens.push_back(doubleToken);

            checkAndSkip();

            tokens.push_back(tokenizeDoubleID());
            checkAndSkip();

            if(current == '='){
                advanceCursor();
                checkAndSkip();

                if(isdigit(current)){
                    tokens.push_back(tokenizeDoubleVal());
                } else {
                    throw std::runtime_error("Error: expected double value after '='");
                }
            }
            continue;
        } else if(matchKeyword("char")){
            Token* charToken = new Token;
            charToken->TYPE = TOKEN_KEYWORD_CHAR;
            charToken->value = "char";
            tokens.push_back(charToken);
            checkAndSkip();
            tokens.push_back(tokenizeCharID());
            checkAndSkip();
            if(current == '='){
                tokens.push_back(tokenizeCharVal());
            } else {
                throw std::runtime_error("Error: Expected char value after '='");
            }
            continue;
        } else if(matchKeyword("str")){
            Token* strToken = new Token;
            strToken->TYPE = TOKEN_KEYWORD_STR;
            strToken->value = "str";
            tokens.push_back(strToken);
            
            checkAndSkip();

            tokens.push_back(tokenizeStrID());
            checkAndSkip();
            if(current == '='){
                tokens.push_back(tokenizeStrVal());
            } else {
                throw std::runtime_error("Error: Expected a string value after '='");
            }
            continue;
        }

            switch(current){
                case '=':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                    break;
                case '(':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_PAREN));
                    break;
                case ')':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PAREN));
                    break;
                case ';':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));
                    break;
                default:
                    std::cerr << "Character not recognized: " << current << std::endl;
            }
    }
}

