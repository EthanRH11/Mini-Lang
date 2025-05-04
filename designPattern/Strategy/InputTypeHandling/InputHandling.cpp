#include "strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

bool InputTypeProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return stream.current() == '<';
}

Token *InputTypeProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();

    if (stream.current() != '<')
    {
        ErrorHandler::getInstance().reportLexicalError(
            ErrorLevel::ERROR,
            "Expected '<' for input type specification",
            "",
            startPos);
        return nullptr;
    }

    // consume '<'
    stream.advanceCursor();

    std::string typeName;

    while (!stream.eof() && stream.current() != '>')
    {
        typeName += stream.current();
        stream.advanceCursor();
    }

    // Check that there is closing '>'
    if (stream.eof() || stream.current() != '>')
    {
        ErrorHandler::getInstance().reportLexicalError(
            ErrorLevel::ERROR,
            "Unterminated input type specification." + typeName,
            "",
            startPos);
        return nullptr;
    }

    stream.advanceCursor(); // consuming >

    // Validate type name
    if (typeName == "int" ||
        typeName == "str" ||
        typeName == "double" ||
        typeName == "char" ||
        typeName == "bool")
    {
        return new Token{TokenType::INPUT_TYPE, typeName, startPos, stream.getPosition() - startPos};
    }
    else
    {
        ErrorHandler::getInstance().reportLexicalError(
            ErrorLevel::ERROR,
            "Invalid Input type: " + typeName,
            "",
            startPos);
        return nullptr;
    }
}