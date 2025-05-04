#include "Strategy/strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"

bool StringLiteralProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    // Check if the character is double quote
    return stream.current() == '"';
}

Token *StringLiteralProcessingStrategy::process(CharacterStream &stream) const
{
    if (stream.current() != '"')
    {
        return nullptr;
    }

    size_t startPos = stream.getPosition();

    // Consume opening quote
    stream.advanceCursor();

    std::string value;

    while (!stream.eof())
    {
        char c = stream.current();

        if (c == '"')
        {
            stream.advanceCursor(); // consume closing quote
            return new Token(TokenType::STRING_VAL, value, startPos, 0);
        }

        value += c;
        stream.advanceCursor();
    }

    ErrorHandler::getInstance().reportLexicalError(
        ErrorLevel::ERROR,
        "Unterminated String Literal",
        "",
        startPos);

    return nullptr;
}