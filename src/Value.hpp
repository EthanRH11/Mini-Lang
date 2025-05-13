#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

// Forward declaration
class DynamicArray;

class Value
{
public:
    enum class Type
    {
        INTEGER,
        DOUBLE,
        BOOL,
        CHAR,
        STRING,
        ARRAY,
        NONE
    };

    // Constructors
    Value(); // NONE
    Value(int v);
    Value(double v);
    Value(bool v);
    Value(char v);
    Value(const std::string &v);
    Value(std::shared_ptr<DynamicArray> arr);

    // Copy/move
    Value(const Value &);
    Value(Value &&) noexcept;
    Value &operator=(const Value &);
    Value &operator=(Value &&) noexcept;

    // Type info
    Type getType() const;

    // Type checks
    bool isInt() const;
    bool isDouble() const;
    bool isBool() const;
    bool isChar() const;
    bool isString() const;
    bool isArray() const;
    bool isNone() const;
    bool isInitialize() const { return !isNone(); }

    // Add these to Value.hpp in the public section
    bool isInteger() const;
    int getInteger() const;
    double getDouble() const;
    bool getBool() const;
    char getChar() const;
    std::string getString() const;

        // Getters
    int asInt() const;
    double asDouble() const;
    bool asBool() const;
    char asChar() const;
    const std::string &asString() const;
    std::shared_ptr<DynamicArray> asArray() const;

    // String conversion
    std::string toString() const;

    // Operators
    Value operator+(const Value &rhs) const;
    // ... (other operators as needed)

    friend std::ostream &operator<<(std::ostream &os, const Value &v);

private:
    Type type;
    std::variant<
        int,
        double,
        bool,
        char,
        std::string,
        std::shared_ptr<DynamicArray>>
        data;
};

#endif