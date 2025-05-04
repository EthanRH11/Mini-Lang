#include "Strategy/strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"

bool NumberProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return std::isdigit(stream.current());
}
Token *NumberProcessingStrategy::process(CharacterStream &stream) const
{
    std::string number;
    bool isDouble = false;
    size_t startPos = stream.getPosition();

    while (std::isdigit(stream.current()) || stream.current() == '.')
    {
        if (stream.current() == '.')
        {
            if (isDouble)
            {
                ErrorHandler::getInstance().reportLexicalError(
                    ErrorLevel::ERROR,
                    "Invalid number format with multiple decimals.",
                    "",
                    stream.getPosition());
                return nullptr;
            }
            isDouble = true;
        }
        number += stream.current();
        stream.advanceCursor();
    }

    return new Token{isDouble ? TokenType::DOUBLE_VAL : TokenType::INTEGER_VAL, number, startPos, 0};
}
