#ifndef LIBRARY_MANAGER_HPP
#define LIBRARY_MANAGER_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "../parser.hpp"

class LibraryManager
{
private:
    //--------------------------
    // Structures within manager
    //--------------------------
    // Maps library names to their AST roots
    std::unordered_map<std::string, AST_NODE *> libraries;

    // Maps function names to their definitions
    std::unordered_map<std::string, AST_NODE *> functionRegistry;

    // Set of loaded libraries
    std::unordered_set<std::string> loadedLibraries;

    // Single directory for libraries
    std::string libraryDirectory;

    // Find a library file
    std::string findLibraryFile(const std::string &libraryName);

    // Parse a library file
    AST_NODE *parseLibraryFile(const std::string &filePath);

    // Register all functions in a library
    void registerLibraryFunctions(const std::string &libraryName, AST_NODE *libraryAST);

public:
    LibraryManager();
    ~LibraryManager();

    void addSearchPath(const std::string &path);
    bool LibraryManager::loadPreCompiledLibrary(const std::string &name, AST_NODE *node);
    bool loadLibrary(const std::string &name);

    AST_NODE *findFunction(const std::string &name);

    void registerBuiltinFunction(const std::string &name, AST_NODE *node);

    bool isLibraryLoaded(const std::string &name) const;

    std::vector<std::string> getAvailableLibraries() const;

    void walkASTForFunctions(AST_NODE *node);

    AST_NODE *generateRandomAST();

    static LibraryManager &getInstance()
    {
        static LibraryManager instance;
        return instance;
    }
};

#endif // LIBRARY_MANAGER_HPP