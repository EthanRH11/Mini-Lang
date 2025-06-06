#ifndef OBJECT_INSTANCE_HPP
#define OBJECT_INSTANCE_HPP

#include <unordered_map>
#include <string>
#include "ClassInfo.hpp"
#include "Value.hpp"

class ObjectInstance
{
public:
    ObjectInstance(ClassInfo *info) : classInfo_(info)
    {
        for (auto &fld : classInfo_->fields)
        {
            fieldValues_[fld.name] = fld.defaultVal;
        }
    }

    Value getField(const std::string &fieldName) const
    {
        auto it = fieldValues_.find(fieldName);
        if (it == fieldValues_.end())
        {
            throw std::runtime_error("No such field: " + fieldName);
        }
        return it->second;
    }

    void setField(const std::string &fieldName, const Value &val)
    {
        auto it = fieldValues_.find(fieldName);
        if (it == fieldValues_.end())
        {
            throw std::runtime_error("No such field: " + fieldName);
        }
        it->second = val;
    }

    Value callMethod(const std::string &methodName, const std::vector<Value> &args);

private:
    ClassInfo *classInfo_;
    std::unordered_map<std::string, Value> fieldValues_;
};

#endif