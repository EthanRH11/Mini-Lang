#ifndef INTERPERTER_HPP
#define INTERPERTER_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <variant>

#include "parser.hpp"
#include "Value.hpp"

class Interperter
{
public:
    Interperter(AST_NODE *root) : root(root)
    {
        setupOutputFile();
    }

    ~Interperter()
    {
        if (outputFile.is_open())
        {
            outputFile.close();
        }
    }

    void setupOutputFile();

    void execute()
    {
        executeNode(root);
    }

private:
    AST_NODE *root;
    std::map<std::string, Value> variables;
    std::ofstream outputFile;

    void executeNode(AST_NODE *node);         // Main recursive function
    Value evaluateExpression(AST_NODE *node); // Evaluates expressions such as (10 + x)
    void executeStatement(AST_NODE *node);

    void printToOutput(const Value &value)
    {
        // Print to console
        if (value.isInteger())
        {
            std::cout << value.getInteger();
            outputFile << value.getInteger();
        }
        else if (value.isDouble())
        {
            std::cout << value.getDouble();
            outputFile << value.getDouble();
        }
        else if (value.isBool())
        {
            std::cout << (value.getBool() ? "true" : "false");
            outputFile << (value.getBool() ? "true" : "false");
        }
        else if (value.isString())
        {
            // Check if the string contains newline characters
            std::string str = value.getString();
            bool containsNewline = str.find('\n') != std::string::npos;

            std::cout << str;
            outputFile << str;

            // Only flush if the string contains a newline
            if (containsNewline)
            {
                std::cout.flush();
                outputFile.flush();
            }
        }
        else if (value.isChar())
        {
            char ch = value.getChar();
            std::cout << ch;
            outputFile << ch;

            // Only flush if the character is a newline
            if (ch == '\n')
            {
                std::cout.flush();
                outputFile.flush();
            }
        }
        else
        {
            std::cout << "NULL";
            outputFile << "NULL";
        }
    }

    // May need to implement more handling
};

#endif // INTERPERTER_HPP