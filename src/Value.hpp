#ifndef VALUE_HPP
#define VALUE_HPP

#include <variant>
#include <string>

class Value
{
public:
    Value(int val);
    Value(const std::string &val);

    bool isInteger() const;
    bool isString() const;

    int getInteger() const;
    std::string getString() const;

    std::string toString() const;

private:
    enum ValueType
    {
        INTEGER,
        STRING,
        BOOL,
        DOUBLE,
        FLOAT
    } type;

    // storage for the different types
    union
    {
        int intValue;
        bool boolValue;
        double doubleValue;
        float floatValue;
    };

    std::string stringValue;
};

#endif // VALUE_HPP