#include "strategy_lexer.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "TokenType/token_type.hpp"

OperatorProcessingStrategy::OperatorProcessingStrategy()
{
    // Initalize single character operators
    singleCharOperatorMap = {
        {'+', TokenType::OPERATOR_ADD},
        {'-', TokenType::OPERATOR_ADD},
        {'*', TokenType::OPERATOR_ADD},
        {'/', TokenType::OPERATOR_ADD},
        {'<', TokenType::OPERATOR_ADD},
        {'>', TokenType::OPERATOR_ADD},
        {'(', TokenType::OPERATOR_ADD},
        {')', TokenType::OPERATOR_ADD},
        {'{', TokenType::OPERATOR_ADD},
        {'}', TokenType::OPERATOR_ADD},
        {'=', TokenType::OPERATOR_ADD},
        {';', TokenType::OPERATOR_ADD},
        {',', TokenType::OPERATOR_ADD},
        {'%', TokenType::OPERATOR_ADD},
    };
    // Initialize Multi character operators
    multiCharOperatorMap = {
        {"==", TokenType::OPERATOR_EQUALS},
        {">>$", TokenType::SINGLELINE_COMMENT},
        {"<<$", TokenType::MULTILINE_COMMENT},
        {"+=", TokenType::OPERATOR_ADD_ASSIGN},
        {"++", TokenType::OPERATOR_INCREMENT},
        {"--", TokenType::OPERATOR_DECREMENT},
        {"<=", TokenType::OPERATOR_LESS_EQUAL},
        {">=", TokenType::OPERATOR_GREATER_EQUAL},
        {"=>", TokenType::SPACESHIP},
        {"=/=", TokenType::OPERATOR_DOESNT_EQUAL},
        {"...", TokenType::NL_SYMBOL},
    };
}

bool OperatorProcessingStrategy::canProcess(const CharacterStream &stream) const
{
    char currentChar = stream.current();

    if (singleCharOperatorMap.find(currentChar) != singleCharOperatorMap.end())
    {
        return true;
    }

    for (const auto &pair : multiCharOperatorMap)
    {
        if (pair.first[0] == currentChar)
        {
            return true;
        }
    }
    return false;
}

Token *OperatorProcessingStrategy::process(CharacterStream &stream) const
{
    size_t startPos = stream.getPosition();
    char currentChar = stream.current();

    // First match multicharacter operators
    for (const auto &pair : multiCharOperatorMap)
    {
        const std::string &op = pair.first;
        bool isMatch = true;

        for (size_t i = 0; i < op.length(); ++i)
        {
            if (stream.peekAhead(i) != op[i])
            {
                isMatch = false;
                break;
            }
        }

        if (isMatch)
        {
            // found match, advance cursor past operator
            for (size_t i = 0; i < op.length(); i++)
            {
                stream.advanceCursor();
            }
            return new Token{pair.second, op, startPos, op.length()};
        }
    }

    // If reached, checking for single char operators now
    auto it = singleCharOperatorMap.find(currentChar);
    if (it != singleCharOperatorMap.end())
    {
        stream.advanceCursor(); // move past char
        return new Token{it->second, std::string(1, currentChar), startPos, 1};
    }
    // no op found
    return nullptr;
}
