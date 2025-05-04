#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Strategy/SourceHandling/strategy_sourcestream.cpp"
#include "Strategy/strategy_lexer.hpp"
#include "TokenType/token_type.hpp"
#include "ErrorHandler/ErrorHandler.hpp"

// Function to read a file into a string
std::string readFileToString(const std::string &filename)
{
    std::ifstream inFile(filename);
    if (!inFile)
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}

// Function to convert token type to string for display
std::string tokenTypeToString(TokenType type)
{
    // Implement a switch statement to convert TokenType enum to string
    switch (type)
    {
    case TokenType::OPERATOR_ADD:
        return "OPERATOR_ADD";
    case TokenType::OPERATOR_EQUALS:
        return "OPERATOR_EQUALS";
    case TokenType::SINGLELINE_COMMENT:
        return "SINGLELINE_COMMENT";
    case TokenType::MULTILINE_COMMENT:
        return "MULTILINE_COMMENT";
    case TokenType::NL_SYMBOL:
        return "NL_SYMBOL";
    case TokenType::IF:
        return "IF";
    case TokenType::ELSE:
        return "ELSE";
    // Add other token types here
    default:
        return "UNKNOWN";
    }
}

int main(int argc, char *argv[])
{
    // Check if filename was provided
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string sourceCode = readFileToString(filename);

    // Create a character stream from the source code
    CharacterStream stream(sourceCode);

    // Create the lexer and initialize all strategies
    StrategyLexer lexer(stream);

    // Register all the token processing strategies
    lexer.registerStrategy(new NewlineProcessingStrategy());
    lexer.registerStrategy(new OperatorProcessingStrategy());
    lexer.registerStrategy(new SingleLineCommentProcessingStrategy());
    lexer.registerStrategy(new MultiLineCommentProcessingStrategy());
    lexer.registerStrategy(new KeywordProcessingStrategy());
    // Register additional strategies as needed

    // Perform the tokenization
    std::vector<Token> tokens = lexer.tokenize();

    // Check if there were any errors
    if (ErrorHandler::getInstance().hasErrors())
    {
        std::cerr << "Lexical analysis failed with "
                  << ErrorHandler::getInstance().getErrorCount()
                  << " errors." << std::endl;
        // Optionally, print out all errors
        for (const auto &error : ErrorHandler::getInstance().getErrors())
        {
            // Print error details
        }
        return 1;
    }

    // Print out all tokens
    std::cout << "Tokenization successful. Found " << tokens.size() << " tokens:" << std::endl;
    for (const auto &token : tokens)
    {
        std::cout << "Position: " << token.position
                  << ", Length: " << token.length
                  << ", Type: " << tokenTypeToString(token.type)
                  << ", Value: '" << token.value << "'" << std::endl;
    }

    return 0;
}