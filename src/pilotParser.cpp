#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "lexer.hpp"
#include "parser.hpp"

void printTokens(const std::vector<Token *> &tokens)
{
    // Helper function for debugging
    for (const auto &token : tokens)
    {
        std::cout << "<" << getTokenTypeName(token->TYPE) << ">"
                  << "\t" << "<" << token->value << ">" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    // Read the entire file into a string
    std::string sourceCode((std::istreambuf_iterator<char>(inputFile)),
                           std::istreambuf_iterator<char>());
    inputFile.close();

    // try
    // {
    //     Lexer lexer(sourceCode);
    //     std::vector<Token *> tokens = lexer.tokenize();
    //     printTokens(tokens);
    //     // Clean up dynamically allocated tokens
    //     for (auto &token : tokens)
    //     {
    //         delete token;
    //     }
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "Error during tokenization: " << e.what() << std::endl;
    //     return 1;
    // }

    try
    {
        Lexer lexer(sourceCode);
        std::vector<Token *> tokens = lexer.tokenize();
        printTokens(tokens);
        Parser parse(tokens);
        auto statements = parse.parse();

        for (auto &token : tokens)
        {
            delete token;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during parsing: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}