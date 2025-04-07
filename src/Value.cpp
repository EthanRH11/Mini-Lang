#include "Value.hpp"
#include <string>
#include <stdexcept>

#include "Value.hpp"
#include <stdexcept>
#include <iomanip>

// Constructors
Value::Value(int val) : type(Type::INTEGER)
{
    data.intValue = val;
}

Value::Value(const std::string &val) : type(Type::STRING), stringValue(val) {}

Value::Value(double val) : type(Type::DOUBLE)
{
    data.doubleValue = val;
}

Value::Value(float val) : type(Type::FLOAT)
{
    data.floatValue = val;
}

Value::Value(bool val) : type(Type::BOOL)
{
    data.boolValue = val;
}

// Type checks
bool Value::isInteger() const
{
    return type == Type::INTEGER;
}

bool Value::isString() const
{
    return type == Type::STRING;
}

bool Value::isDouble() const
{
    return type == Type::DOUBLE;
}

bool Value::isFloat() const
{
    return type == Type::FLOAT;
}

bool Value::isBool() const
{
    return type == Type::BOOL;
}

// Getters
int Value::getInteger() const
{
    if (!isInteger())
        throw std::runtime_error("Value is not an integer");
    return data.intValue;
}

std::string Value::getString() const
{
    if (!isString())
        throw std::runtime_error("Value is not a string");
    return stringValue;
}

double Value::getDouble() const
{
    if (!isDouble())
        throw std::runtime_error("Value is not a double");
    return data.doubleValue;
}

float Value::getFloat() const
{
    if (!isFloat())
        throw std::runtime_error("Value is not a float");
    return data.floatValue;
}

bool Value::getBool() const
{
    if (!isBool())
        throw std::runtime_error("Value is not a bool");
    return data.boolValue;
}

// String representation
std::string Value::toString() const
{
    std::ostringstream oss;
    switch (type)
    {
    case Type::INTEGER:
        oss << data.intValue;
        break;
    case Type::STRING:
        oss << stringValue;
        break;
    case Type::DOUBLE:
        oss << std::fixed << std::setprecision(6) << data.doubleValue;
        break;
    case Type::FLOAT:
        oss << std::fixed << std::setprecision(6) << data.floatValue;
        break;
    case Type::BOOL:
        oss << (data.boolValue ? "true" : "false");
        break;
    }
    return oss.str();
}

// Stream output
std::ostream &operator<<(std::ostream &os, const Value &v)
{
    switch (v.type)
    {
    case Value::Type::INTEGER:
        os << v.data.intValue;
        break;
    case Value::Type::STRING:
        os << v.stringValue;
        break;
    case Value::Type::DOUBLE:
        os << std::fixed << std::setprecision(6) << v.data.doubleValue;
        break;
    case Value::Type::FLOAT:
        os << std::fixed << std::setprecision(6) << v.data.floatValue;
        break;
    case Value::Type::BOOL:
        os << (v.data.boolValue ? "true" : "false");
        break;
    }
    return os;
}
