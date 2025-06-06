#ifndef FIELD_INFO_HPP
#define FIELD_INFO_HPP

#include <string>
#include "Value.hpp"

struct FieldInfo
{
    std::string name;     // e.g x or y
    std::string typeName; // e.g. "int" or "double"
    Value defaultVal;     // value is specified or null
};

#endif