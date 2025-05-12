#include <stdexcept>
#include <sstream>
#include <iomanip>

#include "dynamic_array.hpp"
#include "Value.hpp"

Value::Value() : type(Type::NONE) {}

Value::Value(int v) : type(Type::INTEGER), data(v) {}
Value::Value(double v) : type(Type::DOUBLE), data(v) {}
Value::Value(char v) : type(Type::CHAR), data(v) {}
Value::Value(bool v) : type(Type::BOOL), data(v) {}
Value::Value(const std::string &v) : type(Type::STRING), data(v) {}
Value::Value(std::shared_ptr<DynamicArray> arr) : type(Type::ARRAY), data(arr) {}

Value::Value(const Value &other) : type(other.type), data(other.data) {}

Value::Value(Value &&other) noexcept : type(other.type), data(std::move(other.data))
{
    other.type = Type::NONE;
}

// Copy assignment operator
Value &Value::operator=(const Value &other)
{
    if (this != &other)
    {
        type = other.type;
        data = other.data;
    }
    return *this;
}

// Move assignment operator
Value &Value::operator=(Value &&other) noexcept
{
    if (this != &other)
    {
        type = other.type;
        data = std::move(other.data);
        other.type = Type::NONE;
    }
    return *this;
}

// Type info
Value::Type Value::getType() const
{
    return type;
}

// Type checks
bool Value::isInt() const
{
    return type == Type::INTEGER;
}

bool Value::isDouble() const
{
    return type == Type::DOUBLE;
}

bool Value::isBool() const
{
    return type == Type::BOOL;
}

bool Value::isChar() const
{
    return type == Type::CHAR;
}

bool Value::isString() const
{
    return type == Type::STRING;
}

bool Value::isArray() const
{
    return type == Type::ARRAY;
}

bool Value::isNone() const
{
    return type == Type::NONE;
}

// Getters
int Value::asInt() const
{
    if (!isInt())
    {
        throw std::runtime_error("Value is not an integer");
    }
    return std::get<int>(data);
}

double Value::asDouble() const
{
    if (!isDouble())
    {
        throw std::runtime_error("Value is not a double");
    }
    return std::get<double>(data);
}

bool Value::asBool() const
{
    if (!isBool())
    {
        throw std::runtime_error("Value is not a boolean");
    }
    return std::get<bool>(data);
}

char Value::asChar() const
{
    if (!isChar())
    {
        throw std::runtime_error("Value is not a char");
    }
    return std::get<char>(data);
}

const std::string &Value::asString() const
{
    if (!isString())
    {
        throw std::runtime_error("Value is not a string");
    }
    return std::get<std::string>(data);
}

std::shared_ptr<DynamicArray> Value::asArray() const
{
    if (!isArray())
    {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<std::shared_ptr<DynamicArray>>(data);
}

// Add these to Value.cpp (NOT in the header if they're not already declared there)
bool Value::isInteger() const { return isInt(); }
int Value::getInteger() const { return asInt(); }
double Value::getDouble() const { return asDouble(); }
bool Value::getBool() const { return asBool(); }
char Value::getChar() const { return asChar(); }
std::string Value::getString() const { return asString(); }

// String conversion
std::string Value::toString() const
{
    std::stringstream ss;

    switch (type)
    {
    case Type::INTEGER:
        ss << std::get<int>(data);
        break;

    case Type::DOUBLE:
        ss << std::fixed << std::setprecision(6) << std::get<double>(data);
        // Trim trailing zeros
        {
            std::string str = ss.str();
            size_t pos = str.find_last_not_of('0');
            if (pos != std::string::npos && str[pos] == '.')
                pos--; // Keep one trailing zero after decimal point
            str.erase(pos + 1);
            // Clear and reset the stringstream
            ss.str("");
            ss << str;
        }
        break;

    case Type::BOOL:
        ss << (std::get<bool>(data) ? "true" : "false");
        break;

    case Type::CHAR:
        ss << std::get<char>(data);
        break;

    case Type::STRING:
        ss << std::get<std::string>(data);
        break;

    case Type::ARRAY:
        if (auto arr = std::get<std::shared_ptr<DynamicArray>>(data))
        {
            // Assuming DynamicArray has a toString method
            ss << arr->toString();
        }
        else
        {
            ss << "[]"; // Empty or null array
        }
        break;

    case Type::NONE:
        ss << "none";
        break;
    }

    return ss.str();
}

// Operators
Value Value::operator+(const Value &rhs) const
{
    // String concatenation has highest precedence
    if (isString() || rhs.isString())
    {
        return Value(toString() + rhs.toString());
    }

    // Array concatenation
    if (isArray() && rhs.isArray())
    {
        auto result = std::make_shared<DynamicArray>(*asArray());
        result->concatenate(*rhs.asArray());
        return Value(result);
    }

    // Numeric addition - prioritize higher precision types
    if (isDouble() || rhs.isDouble())
    {
        double left = isDouble() ? asDouble() : (isInt() ? asInt() : 0.0);
        double right = rhs.isDouble() ? rhs.asDouble() : (rhs.isInt() ? rhs.asInt() : 0.0);
        return Value(left + right);
    }

    // Integer addition (includes chars)
    if (isInt() || isChar())
    {
        int left = isInt() ? asInt() : static_cast<int>(asChar());
        int right;

        if (rhs.isInt())
            right = rhs.asInt();
        else if (rhs.isChar())
            right = static_cast<int>(rhs.asChar());
        else
            return Value(toString() + rhs.toString()); // Fallback to string concat

        return Value(left + right);
    }

    // Boolean addition (logical OR)
    if (isBool() && rhs.isBool())
    {
        return Value(asBool() || rhs.asBool());
    }

    // Fallback - stringify both and concatenate
    return Value(toString() + rhs.toString());
}

// Stream output operator
std::ostream &operator<<(std::ostream &os, const Value &v)
{
    os << v.toString();
    return os;
}