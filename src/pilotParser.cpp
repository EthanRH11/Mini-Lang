#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <memory>

#include "lexer.hpp"
#include "parser.hpp"
#include "interperter.hpp"

namespace fs = std::filesystem;

void printTokens(const std::vector<Token *> &tokens);
void printNodes(AST_NODE *node, int depth = 0);
void deleteASTTree(AST_NODE *node);
void printUsage(const char *programName);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 1;
    }

    std::string mode = (argc >= 3) ? argv[2] : "all";

    if (mode != "lex" && mode != "parse" && mode != "interpret" && mode != "all")
    {
        std::cerr << "Error: Invalid mode '" << mode << "'" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    fs::path inputPath(argv[1]);
    if (!fs::exists(inputPath))
    {
        std::cerr << "Error: Input file not found: " << inputPath << std::endl;
        return 1;
    }

    std::ifstream inputFile(inputPath);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open file " << inputPath << std::endl;
        return 1;
    }

    std::string sourceCode((std::istreambuf_iterator<char>(inputFile)),
                           std::istreambuf_iterator<char>());
    inputFile.close();

    try
    {
        // Stage 1: Tokenizing
        Lexer lexer(sourceCode);
        std::vector<Token *> tokens = lexer.tokenize();

        if (mode == "lex" || mode == "all")
        {
            std::cout << "\n===== LEXICAL ANALYSIS ======\n"
                      << std::endl;
            printTokens(tokens);
        }

        if (mode == "lex")
        {
            // Cleanup the tokens
            for (auto &token : tokens)
            {
                delete token;
            }
            return 0;
        }

        // Stage 2: Parsing
        AST_NODE *root = nullptr;
        Parser parser(tokens);
        root = parser.parse();

        if (mode == "parse" || mode == "all")
        {
            std::cout << "\n===== SYNTAX ANALYSIS =====\n"
                      << std::endl;
            if (root)
            {
                printNodes(root);
            }
            else
            {
                std::cerr << "Error: Parsing failed to produce an AST" << std::endl;

                for (auto &token : tokens)
                {
                    delete token;
                }
                return 1;
            }
        }

        if (mode == "parse")
        {
            for (auto &token : tokens)
            {
                delete token;
            }
            deleteASTTree(root);
            return 0;
        }

        // Stage 3: Interpretation
        if (mode == "interpret" || mode == "all")
        {
            std::cout << "\n===== PROGRAM OUTPUT =====\n"
                      << std::endl;

            Interperter interperter(root);
            interperter.execute();
        }

        for (auto &token : tokens)
        {
            delete token;
        }
        deleteASTTree(root);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Print Usage Information
void printUsage(const char *programName)
{
    std::cerr << "Usage: " << programName << " <input_file> [mode]" << std::endl;
    std::cerr << "Modes:" << std::endl;
    std::cerr << "  lex       - Run only lexical analysis" << std::endl;
    std::cerr << "  parse     - Run lexical and syntax analysis" << std::endl;
    std::cerr << "  interpret - Run only program output (minimal debug info)" << std::endl;
    std::cerr << "  all       - Run all stages with debug output (default)" << std::endl;
}

// Print token information
void printTokens(const std::vector<Token *> &tokens)
{
    std::cout << "Token Count: " << tokens.size() << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "TYPE               | VALUE" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    for (const auto &token : tokens)
    {
        std::cout << std::left << std::setw(18) << getTokenTypeName(token->TYPE);
        std::cout << " | " << token->value << std::endl;
    }
}

// Print AST nodes
void printNodes(AST_NODE *node, int depth)
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

// Recursively delete the AST
void deleteASTTree(AST_NODE *node)
{
    if (!node)
        return;

    // Delete child node
    if (node->CHILD)
    {
        deleteASTTree(node->CHILD);
    }

    // Delete all sub-statements
    for (auto subNode : node->SUB_STATEMENTS)
    {
        deleteASTTree(subNode);
    }

    // Delete the node itself
    delete node;
}