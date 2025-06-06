#ifndef SECURE_INFO_HPP
#define SECURE_INFO_HPP

#include <vector>
#include <string>
#include "parser.hpp"

struct SecureInfo
{
    std::vector<std::pair<std::string, std::string>> params;
    AST_NODE *secureBodyAST;
};

#endif