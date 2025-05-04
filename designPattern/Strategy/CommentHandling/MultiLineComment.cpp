#include "strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

bool MultiLineCommentProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return (stream.current() == '<' && stream.peekAhead(1) == '<' && stream.peekAhead(2) == '$');
}

Token *MultiLineCommentProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();

    // Skip opening comment marker
    stream.advanceCursor(); // '<'
    stream.advanceCursor(); // '<'
    stream.advanceCursor(); // '$'

    std::string commentText = "<<$";

    // Looking for the closing marker '$>>'
    while (!stream.eof())
    {
        // Check for closing marker
        if (stream.current() == '$' &&
            stream.peekAhead(1) == '>' &&
            stream.peekAhead(2) == '>')
        {
            // Add closing marker to comment text
            commentText += "$>>";

            // Consume closing marker
            stream.advanceCursor(); // '$'
            stream.advanceCursor(); // '>'
            stream.advanceCursor(); // '>'

            // Return token with complete comment
            return new Token{TokenType::MULTILINE_COMMENT, commentText, startPos, stream.getPosition() - startPos};
        }

        // Add current character to comment text
        commentText += stream.current();
        stream.advanceCursor();
    }

    // If we get here, the comment was not properly closed
    ErrorHandler::getInstance().reportLexicalError(
        ErrorLevel::ERROR,
        "Unterminated multiline comment",
        commentText,
        startPos);

    // Return token with incomplete comment
    return new Token{TokenType::MULTILINE_COMMENT, commentText, startPos, stream.getPosition() - startPos};
}