#ifndef METHOD_INFO_HPP
#define METHOD_INFO_HPP

#include <string>
#include <vector>
#include "parser.hpp"
#include "Value.hpp"

struct MethodInfo
{
    std::string name;                    // e.g. calcArea
    std::vector<std::string> paramTypes; // e.g. "int" or "double"
    AST_NODE *bodyAST;                   // pointer to the AST subtree for this
};

#endif