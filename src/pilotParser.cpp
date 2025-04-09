#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "interperter.hpp"

void printTokens(const std::vector<Token *> &tokens)
{
    // Helper function for debugging
    for (const auto &token : tokens)
    {
        std::cout << "<" << getTokenTypeName(token->TYPE) << ">"
                  << "\t" << "<" << token->value << ">" << std::endl;
    }
}

void printNodes(AST_NODE *node, int depth = 0)
{
    if (!node)
        return;

    // Print indentation based on depth
    std::string indent(depth * 2, ' ');

    // Print node type and value
    std::cout << indent << "Node type: " << getNodeTypeName(node->TYPE);
    if (!node->VALUE.empty())
    {
        std::cout << ", Value: \"" << node->VALUE << "\"";
    }
    std::cout << std::endl;

    // Print child node if it exists
    if (node->CHILD)
    {
        std::cout << indent << "Child:" << std::endl;
        printNodes(node->CHILD, depth + 1);
    }

    // Print sub-statements if they exist
    if (!node->SUB_STATEMENTS.empty())
    {
        std::cout << indent << "Sub-statements (" << node->SUB_STATEMENTS.size() << "):" << std::endl;
        for (AST_NODE *subNode : node->SUB_STATEMENTS)
        {
            printNodes(subNode, depth + 1);
        }
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

    try
    {
        Lexer lexer(sourceCode);
        std::vector<Token *> tokens = lexer.tokenize();

        printTokens(tokens);

        Parser parser(tokens);
        AST_NODE *root = parser.parse();

        if (root)
        {
            printNodes(root);
        }
        else
        {
            std::cout << "Root node is null!" << std::endl;
        }

        Interperter interperter(root);
        interperter.execute();
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