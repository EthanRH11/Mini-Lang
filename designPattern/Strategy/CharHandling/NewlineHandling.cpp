#include "Strategy/strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

bool NewlineProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return (stream.current() == '\'' && stream.peekAhead(1) == '\\' && stream.peekAhead(2) == 'n');
}

Token *NewlineProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();
    // consume the first single quote
    stream.advanceCursor();

    if (stream.current() == '\\' && stream.peekAhead(1) == 'n')
    {
        stream.advanceCursor();
        stream.advanceCursor();
    }
    else
    {
        ErrorHandler::getInstance().reportLexicalError(
            ErrorLevel::ERROR,
            "Expected '\\n' after opening quote for newline character.",
            "",
            stream.getPosition());
        return nullptr;
    }

    if (stream.current() != '\'')
    {
        ErrorHandler::getInstance().reportLexicalError(
            ErrorLevel::ERROR,
            "Invalid Character decleration, no ending single quote",
            "",
            stream.getPosition());
        return nullptr;
    }
    stream.advanceCursor(); // Skip the final single quote

    return new Token{TokenType::NL_SYMBOL, "\n", startPos, stream.getPosition() - startPos};
}