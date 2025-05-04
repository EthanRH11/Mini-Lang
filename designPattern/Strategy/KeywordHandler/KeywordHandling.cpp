#include "strategy_lexer.hpp"
#include "TokenType/token_type.hpp"
#include "ErrorHandler/ErrorHandler.hpp"

KeywordProcessingStrategy::KeywordProcessingStrategy()
{
    keywordMap = {
        {"if", TokenType::KEYWORD_IF},
        {"proc", TokenType::KEYWORD_PROC},
        {"for", TokenType::KEYWORD_FOR},
        {"input", TokenType::KEYWORD_INPUT},
        {"int", TokenType::KEYWORD_INT},
        {"double", TokenType::KEYWORD_DOUBLE},
        {"str", TokenType::KEYWORD_STR},
        {"check", TokenType::KEYWORD_CHECK},
        {"char", TokenType::KEYWORD_CHAR},
        {"out_to_console", TokenType::KEYWORD_PRINT},
        {"else", TokenType::KEYWORD_ELSE},
        {"end", TokenType::END_OF_FILE},
        {"bool", TokenType::KEYWORD_BOOL},
        {"result", TokenType::KEYWORD_RESULT},
        {"elements", TokenType::KEYWORD_ELEMENT},
    };
}

bool KeywordProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return std::isalpha(stream.current());
}

Token *KeywordProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();
    std::string identifier;

    while (std::isalnum(stream.current()) || stream.current() == '_')
    {
        identifier += stream.current();
        stream.advanceCursor();
    }

    auto it = keywordMap.find(identifier);
    if (it != keywordMap.end())
    {
        return new Token{it->second, identifier, startPos, stream.getPosition() - startPos};
    }

    // Not a keyword, rewind to the start position.

    stream.rewindTo(startPos);
    return nullptr;
}
