#ifndef FACTORY_INFO_HPP
#define FACTORY_INFO_HPP

#include <string>
#include "parser.hpp"

struct FactoryInfo
{
    std::string name;
    AST_NODE *bodyAST;
};

#endif