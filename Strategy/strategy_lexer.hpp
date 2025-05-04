#ifndef STRATEGY_LEXER_HPP
#define STRATEGY_LEXER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "token_type.hpp"

/**
 * @brief Character stream class to handle cursor movement and character access
 *
 * Encapsulates the source code string and provides methods for traversing it
 */

class CharacterStream
{
public:
    CharacterStream(const std::string &source);

    char current() const;
    char advanceCursor();
    char peekAhead(int offset = 1) const;
    void skipWhiteSpace();
    bool eof() const;
    bool matchKeyword(const std::string &keyword) const;
    void consumeKeyword(const std::string &keyword);
    size_t getPosition() const;

private:
    std::string source;
    size_t cursor;
    size_t size;
};

/**
 * @brief Abstract Base Class for token processing strategies
 *
 * Defines the interface that all concrete token processors mutst implement
 */
class TokenProcessingStrategy
{
public:
    virtual ~TokenProcessingStrategy() = default;

    /**
     * @brief Check if this strategy can process the current character
     * @param stream the character stream (program input)
     * @return true if the strategy can handle the current character
     */
    virtual bool canProcess(const CharacterStream &stream) const = 0;

    /**
     * @brief process the current token
     * @param stream The character stream (will be modified as token is consumed)
     * @return pointer to the character that was processed
     */
    virtual Token *process(CharacterStream &stream) const = 0;
};

/***********************************************/
/******** Literal Processing Strategies ********/
/***********************************************/

/**
 * @brief Strategy for processing numerical literals (int and double)
 */
class NumberProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/**
 * @brief strategy for processing string literals ("hello world")
 */
class StringLiteralProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/**
 * @brief strategy for processing character literals ('c')
 */
class CharLiteralProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/**
 * @brief strategy for processing boolean literals
 */
class BoolProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/***********************************************/
/******** Keyword Processing Strategies ********/
/***********************************************/
/**
 * @brief Strategy for processing keyword and identifiers
 */
class KeywordProcessingStrategy : public TokenProcessingStrategy
{
public:
    KeywordProcessingStrategy();
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;

private:
    std::unordered_map<std::string, TokenType> keywordMap;
};

/***********************************************/
/******** Operators Processing Strategies ********/
/***********************************************/

/**
 * @brief Strategy for processing operators and punctuation
 */
class OperatorProcessingStrategy : public TokenProcessingStrategy
{
public:
    OperatorProcessingStrategy();
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;

private:
    std::unordered_map<char, TokenType> singleCharOperatorMap;
    std::unordered_map<std::string, TokenType> multiCharOperatorMap;
};

/***********************************************/
/******** Input Processing Strategies ********/
/***********************************************/
/**
 * @brief Strategy for processing input type specifications
 */
class InputTypeProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/***********************************************/
/******** Comment Processing Strategies ********/
/***********************************************/
/**
 * @brief Strategy for processing single line comments
 */
class SingleLineCommentProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/**
 * @brief Strategy for processing Arrays
 */
class ArrayTypeProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/**
 * @brief Strategy for processing multi-line comments
 */
class MultiLineCommentProcessingStrategy : public TokenProcessingStrategy
{
public:
    bool canProcess(const CharacterStream &stream) const override;
    Token *process(CharacterStream &stream) const override;
};

/***********************************************/
/******** Lexer Class Using Strategy ********/
/***********************************************/
/**
 * @brief Lexer class using the Strategy pattern
 */
class StrategyLexer
{
public:
    StrategyLexer(const std::string &sourceCode);
    ~StrategyLexer();

    /**
     * @brief Register a new token processing strategy
     */
    void registerStrategy(std::unique_ptr<TokenProcessingStrategy> strategy);

    /**
     * @brief Tokenize the source code
     */
    std::vector<Token *> tokenize();

private:
    CharacterStream stream;
    std::vector<std::unique_ptr<TokenProcessingStrategy>> strategies;

    /**
     * @brief Initialize default strategies
     */
    void initializeDefaultStrategies();

    /**
     * @brief Creates an EOF token
     */
    Token *createEOFToken() const;
};

// Helper Function: Returns the name of a token type (debugging)
std::string getTokenTypeName(TokenType);

#endif // STRATEGY_LEXER_HPP