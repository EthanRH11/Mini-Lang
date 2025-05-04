#include "strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

bool SingleLineCommentProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    return (stream.current() == '>' && stream.peekAhead(1) == '>' && stream.peekAhead(2) == '$');
}

Token *SingleLineCommentProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();

    // Skip comment marker
    stream.advanceCursor();
    stream.advanceCursor();
    stream.advanceCursor();

    std::string commentText = ">>$";

    while (!stream.eof() && stream.current() != '\n' && stream.current() != '\r')
    {
        commentText += stream.current();
        stream.advanceCursor();
    }

    return new Token{TokenType::SINGLELINE_COMMENT, commentText, startPos, stream.getPosition() - startPos};
}