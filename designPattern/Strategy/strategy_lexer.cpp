#include "strategy_lexer.hpp"
#include "TokenType/token_type.hpp"

#include <iostream>

StrategyLexer::StrategyLexer(const std::string &sourceCode) : stream(sourceCode)
{
    initializeDefaultStrategies();
}

StrategyLexer::~StrategyLexer() { /*Unique Pointer*/ }

void StrategyLexer::registerStrategy(std::unique_ptr<TokenProcessingStrategy> strategy)
{
    strategies.push_back(std::move(strategy));
}

void StrategyLexer::initializeDefaultStrategies()
{
    // Register all the default token processing
    registerStrategy(std::make_unique<NumberProcessingStrategy>());
    registerStrategy(std::make_unique<StringLiteralProcessingStrategy>());
    registerStrategy(std::make_unique<CharLiteralProcessingStrategy>());
    registerStrategy(std::make_unique<BoolProcessingStrategy>());
    registerStrategy(std::make_unique<KeywordProcessingStrategy>());
    registerStrategy(std::make_unique<OperatorProcessingStrategy>());
    registerStrategy(std::make_unique<InputTypeProcessingStrategy>());
    registerStrategy(std::make_unique<SingleLineCommentProcessingStrategy>());
    registerStrategy(std::make_unique<MultiLineCommentProcessingStrategy>());
}

// Main tokenizing function
std::vector<Token *> StrategyLexer::tokenize()
{
    std::vector<Token *> tokens;

    // Skip any initial whitespace
    stream.skipWhiteSpace();

    while (!stream.eof())
    {
        bool processed = false;

        for (const auto &strategy : strategies)
        {
            if (strategy->canProcess(stream))
            {
                Token *token = strategy->process(stream);
                if (token)
                {
                    tokens.push_back(token);
                    processed = true;
                    break;
                }
            }
        }

        if (!processed)
        {
            // Handle errors, invalid chars
            std::cerr << "Error: Unrecognized token at position "
                      << stream.getPosition() << std::endl;

            stream.advanceCursor();
        }
        // skip any whitespace before the next token
        stream.skipWhiteSpace();
    }

    // Add the eof token
    tokens.push_back(createEOFToken());

    return tokens;
}

Token *StrategyLexer::createEOFToken() const
{
    return new Token{TokenType::END_OF_FILE, "EOF"};
}