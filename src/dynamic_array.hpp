#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <stdexcept>

// Forward declerations
class Interpreter;
class AST_NODE;
class Value;

class DynamicArray
{
public:
    DynamicArray();
    DynamicArray(const std::vector<Value> &values);

    void initialize(const std::vector<Value> &values);
    void initializeRange(int start, int end); // Only for int/Value(int)
    void initializeRepeat(const Value &value, int count);

    Value getElement(int index) const;
    Value getLastElement() const;
    void setElement(int index, const Value &value);
    void insertElement(int index, const Value &value);
    void removeElement(int index);

    size_t getLength() const;

    void sortAscending();
    void sortDescending();

    void append(const Value &value);
    void concatenate(const DynamicArray &other);

    void clear();

    bool contains(const Value &value) const;
    int indexOf(const Value &value) const;

    DynamicArray slice(int start, int end) const;
    DynamicArray map(std::function<Value(const Value &)> mapFunction) const;
    DynamicArray filter(std::function<bool(const Value &)> filterFunction) const;

    std::string toString() const;

private:
    std::vector<Value> elements;
};

#endif