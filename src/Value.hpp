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
    enum class Type
    {
        INTEGER,
        STRING,
        DOUBLE,
        BOOL,
        FLOAT
    };

    Value(int val);
    Value(const std::string &val);
    Value(double val);
    Value(float val);
    Value(bool val);

    Type getType() const { return type; }

    friend std::ostream &operator<<(std::ostream &os, const Value &v);

    bool isInteger() const;
    bool isString() const;
    bool isDouble() const;
    bool isFloat() const;
    bool isBool() const;

    // Getters
    int getInteger() const;
    std::string getString() const;
    double getDouble() const;
    float getFloat() const;
    bool getBool() const;

    std::string toString() const;

private:
    Type type;

    union
    {
        int intValue;
        bool boolValue;
        double doubleValue;
        float floatValue;
    } data;

    std::string stringValue;
};

#endif // VALUE_HPP