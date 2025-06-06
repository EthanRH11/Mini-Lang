#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include "ClassInfo.hpp"
#include "ObjectManager.hpp"
#include "ObjectInstance.hpp"

class Object
{
public:
    // register a new object, returns false if a class with that name already exists.
    bool registerClass(std::unique_ptr<ClassInfo> info)
    {
        auto name = info->className;
        if (classes_.count(name))
            return false;
        classes_[name] = std::move(info);
        return true;
    }

    // Lookup a classinfo by name. returns null if not found
    ClassInfo *getClassInfo(const std::string &name) const
    {
        auto it = classes_.find(name);
        return (it == classes_.end() ? nullptr : it->second.get());
    }

    // at runtime, create a new instance of that class
    std::unique_ptr<ObjectInstance> instantiate(const std::string &name) const
    {
        ClassInfo *ci = getClassInfo(name);
        if (!ci)
            return nullptr;
        return std::make_unique<ObjectInstance>(ci);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<ClassInfo>> classes_;
};

#endif // OBJECT_MANAGER_HPP