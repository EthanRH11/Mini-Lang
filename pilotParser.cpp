#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "lexer.hpp"

void printTokens(const std::vector<Token *> &tokens)
{
    for (const auto &token : tokens)
    {
        std::cout << "<" << getTokenTypeName(token->TYPE) << ">"
                  << "\n"
                  << "\t" << "<" << token->value << ">" << std::endl;
    }
}

// int main(int argc, char *argv[])
// {
//     if (argc > 2)
//     {
//         std::cerr << "Error: not enough file arguments. <sourceFile>.\n";
//         exit(1);
//     }
//     // Indicator
//     std::cout << "Reading from the file: " << argv[1] << std::endl;

//     std::ifstream sourceFileStream(argv[1]);

//     std::stringstream buffer;
//     char temp;

//     while (sourceFileStream.get(temp))
//     {
//         buffer << temp;
//     }
//     std::string sourceCode = buffer.str();
//     std::cout << "This is the source code: " << std::endl
//               << sourceCode;
//     Lexer lexer(sourceCode);
//     std::vector<Token *> tokens = lexer.tokenize();

//     std::cout << std::endl
//               << "This is the end of the program: " << std::endl
//               << sourceCode;
// }

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

    try
    {
        Lexer lexer(sourceCode);
        std::vector<Token *> tokens = lexer.tokenize();

        // Print the tokens
        printTokens(tokens);

        // Clean up dynamically allocated tokens
        for (auto &token : tokens)
        {
            delete token;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during tokenization: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}