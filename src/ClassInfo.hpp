#ifndef CLASS_INFO_HPP
#define CLASS_INFO_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "FieldInfo.hpp"
#include "MethodInfo.hpp"
#include "FactoryInfo.hpp"
#include "SecureInfo.hpp"

struct ClassInfo
{
    std::string className;
    std::vector<FieldInfo> fields;
    std::unordered_map<std::string, MethodInfo> methods;
    std::unordered_map<std::string, FactoryInfo> factories;
    SecureInfo *secureBlock = nullptr;
};

#endif