#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

// Include the main headers
#include "../designPattern/Strategy/strategy_lexer.hpp"
#include "../designPattern/TokenType/token_type.hpp"
#include "../designPattern/ErrorHandler/ErrorHandler.hpp"
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

    // Register the source with the error handler
    ErrorHandler::getInstance().registerSource(filename, sourceCode);

    // Create the lexer - this will automatically initialize default strategies
    StrategyLexer lexer(sourceCode);

    // Perform the tokenization
    std::vector<Token *> tokens = lexer.tokenize();

    // Check if there were any errors
    if (ErrorHandler::getInstance().hasErrors())
    {
        std::cerr << "Lexical analysis failed with errors." << std::endl;
        // Errors have already been printed by the ErrorHandler
        return 1;
    }

    // Print out all tokens
    std::cout << "Tokenization successful. Found " << tokens.size() << " tokens:" << std::endl;
    for (const auto &token : tokens)
    {
        std::cout << "Type: " << getTokenTypeName(token->getType())
                  << ", Value: \"" << token->getValue() << "\"";

        // If your Token class has line and column info
        if (token->getLine() > 0)
        {
            std::cout << ", Line: " << token->getLine();
        }
        if (token->getColumn() > 0)
        {
            std::cout << ", Column: " << token->getColumn();
        }

        std::cout << std::endl;
    }

    // Clean up tokens (since they're raw pointers)
    for (auto token : tokens)
    {
        delete token;
    }

    return 0;
}