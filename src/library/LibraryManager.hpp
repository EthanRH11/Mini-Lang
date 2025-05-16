#ifndef LIBRARY_MANAGER_HPP
#define LIBRARY_MANAGER_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "../parser.hpp"

class LibraryManager
{
private:
    // Library names to their AST root
    std::unordered_map<std::string, AST_NODE *> libraries;
    // Function names to their definitions
    std::unordered_map<std::string, AST_NODE *> functionRegistry;
    // loaded libraries, preventing duplicates
    std::unordered_set<std::string> loadedLibraries;
    // Paths for finding libraries
    std::vector<std::string> libraryPaths;

    // Find a library in search paths
    std::string findLibraryFile(const std::string &name);

    // Parse a library file into an AST
    AST_NODE *parseLibraryFile(const std::string &filePath);

    // Register all functions in a specific library
    void registerLibraryFunctions(const std::string &libName, AST_NODE *ast);

public:
    LibraryManager();
    ~LibraryManager();

    void addSearchPath(const std::string &path);

    bool loadLibrary(const std::string &name);

    AST_NODE *findFunction(const std::string &name);

    void registerBuiltinFunction(const std::string &name, AST_NODE *node);

    bool isLibraryLoaded(const std::string &name) const;

    std::vector<std::string> getAvailableLibraries() const;
};

#endif // LIBRARY_MANAGER_HPP