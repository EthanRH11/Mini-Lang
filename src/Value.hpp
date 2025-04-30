#ifndef VALUE_HPP
#define VALUE_HPP

#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <variant>
#include <string>

class Value
{
public:
    Value() : type(Type::INTEGER)
    {
        data.intValue = 0;
    }
    enum class Type
    {
        INTEGER,
        STRING,
        DOUBLE,
        BOOL,
        FLOAT,
        CHAR
    };

    Value(int val);
    Value(const std::string &val);
    Value(double val);
    Value(float val);
    Value(bool val);
    Value(char val);

    Type getType() const { return type; }

    friend std::ostream &operator<<(std::ostream &os, const Value &v);
    // Addition operator for combining values
    Value operator+(const Value &other) const;

    bool isInteger() const;
    bool isString() const;
    bool isDouble() const;
    bool isFloat() const;
    bool isBool() const;
    bool isChar() const;

    // Getters
    int getInteger() const;
    std::string getString() const;
    double getDouble() const;
    float getFloat() const;
    bool getBool() const;
    char getChar() const;

    std::string toString() const;

    bool isInitialized() const
    {
        return initalized;
    }

private:
    Type type;

    union
    {
        int intValue;
        bool boolValue;
        double doubleValue;
        float floatValue;
        char charValue;
    } data;

    std::string stringValue;

    bool initalized = false;
};

#endif // VALUE_HPP