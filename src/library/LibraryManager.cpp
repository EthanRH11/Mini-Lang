#include "LibraryManager.hpp"
#include "../ErrorHandler.hpp"
#include "../lexer.hpp"
#include "../parser.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace fs = std::filesystem;

LibraryManager::LibraryManager()
{
    libraryDirectory = "./libraries/";
}
LibraryManager::~LibraryManager() {}

bool LibraryManager::loadPreCompiledLibrary(const std::string &name, AST_NODE *node)
{
    if (loadedLibraries.find(name) != loadedLibraries.end())
    {
        return true;
    }
    if (node == nullptr)
    {
        ErrorHandler::getInstance().reportRuntimeError("Cannot load null library: " + name);
        return false;
    }

    libraries[name] = node;
    registerLibraryFunctions(name, node);
    loadedLibraries.insert(name);

    return true;
}

bool LibraryManager::loadLibrary(const std::string &name)
{
    // Step 1: check if library already loaded (only need to load once);
    if (loadedLibraries.find(name) != loadedLibraries.end())
    {
        return true;
    }

    std::string filePath = findLibraryFile(name);
    if (filePath.empty())
    {
        ErrorHandler::getInstance().reportRuntimeError("Library, not found");
        return false; // Not found
    }
    // Tokenize and parse the library file.
    AST_NODE *libraryAST = parseLibraryFile(filePath);
    if (libraryAST == nullptr)
    {
        ErrorHandler::getInstance().reportRuntimeError("Library AST empty");
        return false;
    }

    // Register the library
    libraries[name] = libraryAST;
    registerLibraryFunctions(name, libraryAST);
    loadedLibraries.insert(name);

    return true;
}

AST_NODE *LibraryManager::findFunction(const std::string &name)
{
    auto it = functionRegistry.find(name);
    if (it == functionRegistry.end())
    {
        ErrorHandler::getInstance().reportRuntimeError("Could not find function in function registry: " + name);
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

void LibraryManager::registerBuiltinFunction(const std::string &name, AST_NODE *node)
{
    functionRegistry.insert({name, node});
}

bool LibraryManager::isLibraryLoaded(const std::string &name) const
{
    return loadedLibraries.find(name) != loadedLibraries.end();
}

std::vector<std::string> LibraryManager::getAvailableLibraries() const
{
    std::vector<std::string> available;

    if (fs::exists(libraryDirectory) && fs::is_directory(libraryDirectory))
    {
        for (const auto &entry : fs::directory_iterator(libraryDirectory))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".mllib")
            {
                available.push_back(entry.path().stem().string());
            }
        }
    }
    return available;
}

//     // Find a library file
std::string LibraryManager::findLibraryFile(const std::string &libraryName)
{
    std::string fullPath = libraryDirectory + libraryName + ".mllib";

    if (fs::exists(fullPath))
    {
        return fullPath;
    }

    return "";
}

//     // Parse a library file
AST_NODE *LibraryManager::parseLibraryFile(const std::string &filePath)
{
    if (!fs::exists(filePath))
    {
        ErrorHandler::getInstance().reportRuntimeError("Library file not found: " + filePath);
        return nullptr;
    }

    std::ifstream libraryFile(filePath);
    if (!libraryFile.is_open())
    {
        ErrorHandler::getInstance().reportRuntimeError("Unable to open library file: " + filePath);
        return nullptr;
    }

    std::string sourceCode((std::istreambuf_iterator<char>(libraryFile)),
                           std::istreambuf_iterator<char>());
    libraryFile.close();

    Lexer lexer(sourceCode);
    std::vector<Token *> tokens = lexer.tokenize();

    AST_NODE *ast = nullptr;
    Parser parser(tokens);
    ast = parser.parse();

    return ast;
}

//     // Register all functions in a library
void LibraryManager::registerLibraryFunctions(const std::string &libraryName, AST_NODE *libraryAST)
{
    if (libraryAST == nullptr)
    {
        return;
    }

    walkASTForFunctions(libraryAST);
}

void LibraryManager::walkASTForFunctions(AST_NODE *node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->TYPE == NODE_FUNCTION)
    {
        std::string functionName = node->VALUE;

        functionRegistry[functionName] = node;
    }

    if (node->CHILD)
    {
        walkASTForFunctions(node->CHILD);
    }

    if (!node->SUB_STATEMENTS.empty())
    {
        for (AST_NODE *child : node->SUB_STATEMENTS)
        {
            walkASTForFunctions(child);
        }
    }
}

AST_NODE *LibraryManager::generateRandomAST()
{
    AST_NODE *root = new AST_NODE();
    root->TYPE = ROOT_LIBRARY;
    root->VALUE = "Random";

    AST_NODE *randomInt = new AST_NODE();
    randomInt->TYPE = NODE_RANDOMINT;
    randomInt->VALUE = "randomInt";

    AST_NODE *min = new AST_NODE();
    min->TYPE = NODE_PARAM;
    min->VALUE = "min";

    AST_NODE *max = new AST_NODE();
    max->TYPE = NODE_PARAM;
    max->VALUE = "max";

    randomInt->SUB_STATEMENTS.push_back(min);
    randomInt->SUB_STATEMENTS.push_back(max);

    AST_NODE *coinFlip = new AST_NODE();
    coinFlip->TYPE = NODE_COINFLIP;
    coinFlip->VALUE = "coinFlip";

    AST_NODE *diceRoll = new AST_NODE();
    diceRoll->TYPE = NODE_DICEROLL;
    diceRoll->VALUE = "diceRoll";

    AST_NODE *side = new AST_NODE();
    side->TYPE = NODE_PARAM;
    side->VALUE = "sides";
    side->CHILD = nullptr;

    diceRoll->SUB_STATEMENTS.push_back(side);

    AST_NODE *generatePin = new AST_NODE();
    generatePin->TYPE = NODE_GENERATEPIN;
    generatePin->VALUE = "generatePin";

    AST_NODE *digits = new AST_NODE();
    digits->TYPE = NODE_PARAM;
    digits->VALUE = "digits";
    digits->CHILD = nullptr;

    generatePin->SUB_STATEMENTS.push_back(digits);

    root->SUB_STATEMENTS.push_back(randomInt);
    root->SUB_STATEMENTS.push_back(coinFlip);
    root->SUB_STATEMENTS.push_back(generatePin);
    root->SUB_STATEMENTS.push_back(diceRoll);

    return root;
}