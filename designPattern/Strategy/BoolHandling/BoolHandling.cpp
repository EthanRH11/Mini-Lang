#include "strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

#include <string>
#include <stdexcept>

bool BoolProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return stream.current() == 't' || stream.current() == 'f';
}

Token *BoolProcessingStrategy::process(CharacterStream &stream) const
{
    std::string boolStr = "";

    while (isalpha(stream.current()) && stream.current() != ';')
    {
        boolStr += stream.current();
        stream.advanceCursor();
    }

    if (boolStr == "true" || boolStr == "false")
    {
        return new Token{TokenType::BOOL_VALUE, boolStr};
    }
    else
    {
        ErrorHandler::getInstance().reportLexicalError(
            ErrorLevel::ERROR,
            "Expected boolean value.",
            "",
            stream.getPosition());
        return nullptr;
    }
}