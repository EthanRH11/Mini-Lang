#include "Strategy/strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

bool CharLiteralProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return (stream.current() == '\'' && stream.peekAhead(1) != '\'' && stream.peekAhead(2) == '\'');
}
Token *CharLiteralProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();
    // consume the first single quote
    stream.advanceCursor();
    char charValue = stream.current();
    // Consume Char value and move to second quote
    stream.advanceCursor();

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

    std::string charLexeme(1, charValue);
    return new Token{TokenType::CHAR_VAL, charLexeme, startPos, 0};
}