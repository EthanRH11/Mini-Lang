#include<iostream>
#include<sstream>
#include<string>

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
    return input.substr(cursor, keyword.size()) == keyword;
}

void Lexer::consumeKeyword(const char* keyword){
    int len = strlen(keyword);
    for(int i = 0; i < len; ++i){
        advance();
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
        advance();
    }

    Token* newToken = new Token;
    newToken->type = TOKEN_INTEGER_ID;
    newToken->value = buffer.str();
    return newToken;
}

Token* Lexer::tokenizeDoubleID(){
    consumeKeyword("double");

    std::stringstream buffer;
    while(isalnum(current) || current == '_'){
        buffer << current;
        advance();
    }

    Token* newToken = new Token;
    newToken->type = TOKEN_DOUBLE_ID;
    newToken->value = buffer.str;
    return newToken;

}

Token* Lexer::tokenizeCharID(){
    consumeKeyword("char");

    std::stringstream buffer;
    while(isalnum(current) || current == '_'){
        buffer << current;
    }

    Token* newToken = new Token;
    newToken->TYPE = TOKEN_CHAR_ID;
    newToken->value = buffer.str();
    return newToken;
}

Token* Lexer::tokenizeStrID(){
    consumeKeyword("str");

    while(isalnum(current) || current == '_'){
        buffer << current;
    }

    Token* newToken = new Token;
    newToken->TYPE = TOKEN_STR_ID;
    newToken->value = buffer.str();
    return newToken;
}

std::vector<Token*> Lexer::tokenize(){
    std::vector<Token*> tokens;
    Token* token;

    while(cursor < size && !eof()){
        checkAndSkip();
        if(matchKeyword("int")){
                tokens.push_back(tokenizeIntegerID());
                continue;
            } else if(matchKeyword("double")){
                tokens.push_back(tokenizeDoubleID());
                continue;
            } else if(matchKeyword("char")){
                tokenizeCharID();
                continue;
            } else if(matchKeyword("str")){
                tokenizeStrID();
                continue;
            }
        }
    }
}
