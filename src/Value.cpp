#include "Value.hpp"
#include <stdexcept>
#include <iomanip>

Value::Value(int val) : type(Type::INTEGER)
{
    data.intValue = val;
}

Value::Value(const std::string &val) : type(Type::STRING), stringValue(val)
{
}

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

Value::Value(char val) : type(Type::CHAR)
{
    data.charValue = val;
}

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

bool Value::isChar() const
{
    return type == Type::CHAR;
}

int Value::getInteger() const
{
    if (type != Type::INTEGER)
    {
        throw std::runtime_error("Value is not an integer");
    }
    return data.intValue;
}

std::string Value::getString() const
{
    if (type != Type::STRING)
    {
        throw std::runtime_error("Value is not a string");
    }
    return stringValue;
}

double Value::getDouble() const
{
    if (type != Type::DOUBLE)
    {
        throw std::runtime_error("Value is not a double");
    }
    return data.doubleValue;
}

float Value::getFloat() const
{
    if (type != Type::FLOAT)
    {
        throw std::runtime_error("Value is not a float");
    }
    return data.floatValue;
}

bool Value::getBool() const
{
    if (type != Type::BOOL)
    {
        throw std::runtime_error("Value is not a boolean");
    }
    return data.boolValue;
}

char Value::getChar() const
{
    if (type != Type::CHAR)
    {
        throw std::runtime_error("Value is not a char");
    }
    return data.charValue;
}

std::string Value::toString() const
{
    std::stringstream ss;
    switch (type)
    {
    case Type::INTEGER:
        ss << data.intValue;
        break;
    case Type::STRING:
        ss << stringValue;
        break;
    case Type::DOUBLE:
        ss << std::fixed << std::setprecision(1) << data.doubleValue;
        break;
    case Type::FLOAT:
        ss << data.floatValue;
        break;
    case Type::BOOL:
        ss << (data.boolValue ? "true" : "false");
        break;
    case Type::CHAR:
        ss << data.charValue;
        break;
    }
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Value &v)
{
    os << v.toString();
    return os;
}

Value Value::operator+(const Value &other) const
{

    if (isString() || other.isString())
    {
        return Value(toString() + other.toString());
    }

    // Numeric addition
    if (isDouble() && other.isDouble())
    {
        return Value(getDouble() + other.getDouble());
    }

    if (isFloat() && other.isFloat())
    {
        return Value(getFloat() + other.getFloat());
    }

    if (isDouble() && other.isInteger())
    {
        return Value(getDouble() + other.getInteger());
    }

    if (isInteger() && other.isDouble())
    {
        return Value(getInteger() + other.getDouble());
    }

    if (isFloat() && other.isInteger())
    {
        return Value(getFloat() + other.getInteger());
    }

    if (isInteger() && other.isFloat())
    {
        return Value(getInteger() + other.getFloat());
    }

    if (isInteger() && other.isInteger())
    {
        return Value(getInteger() + other.getInteger());
    }

    if (isChar() && other.isChar())
    {
        return Value(static_cast<int>(getChar()) + static_cast<int>(other.getChar()));
    }

    if (isChar() && other.isInteger())
    {
        return Value(static_cast<int>(getChar()) + other.getInteger());
    }

    if (isInteger() && other.isChar())
    {
        return Value(getInteger() + static_cast<int>(other.getChar()));
    }

    return Value(toString() + other.toString());
}