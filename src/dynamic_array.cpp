#include <algorithm>
#include <sstream>

#include "dynamic_array.hpp"
#include "ErrorHandler.hpp"
#include "Value.hpp"

DynamicArray::DynamicArray() = default;

DynamicArray::DynamicArray(const std::vector<Value> &values) : elements(values) {}

void DynamicArray::initialize(const std::vector<Value> &values)
{
    elements = values;
}

void DynamicArray::initializeRange(int start, int end)
{
    elements.clear();

    // Determine direction (ascending or descending)
    int step = (start <= end) ? 1 : -1;

    // Create range of values [start, end]
    for (int i = start; (step > 0) ? (i <= end) : (i >= end); i += step)
    {
        elements.push_back(Value(i));
    }
}

void DynamicArray::initializeRepeat(const Value &value, int count)
{
    if (count < 0)
    {
        throw std::invalid_argument("Count cannot be negative for repeated initialization");
    }

    elements.clear();
    elements.resize(count, value);
}

// Element access and manipulation
Value DynamicArray::getElement(int index) const
{
    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds");
    }

    return elements[index];
}

Value DynamicArray::getLastElement() const
{
    if (elements.empty())
    {
        throw std::out_of_range("Cannot get last element of empty array");
    }

    return elements.back();
}

void DynamicArray::setElement(int index, const Value &value)
{
    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds");
    }

    elements[index] = value;
}

void DynamicArray::insertElement(int index, const Value &value)
{
    if (index < 0 || index > static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Invalid array index for insertion");
    }

    elements.insert(elements.begin() + index, value);
}

void DynamicArray::removeElement(int index)
{
    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds");
    }

    elements.erase(elements.begin() + index);
}

// Size information
size_t DynamicArray::getLength() const
{
    return elements.size();
}

// Sorting
void DynamicArray::sortAscending()
{
    // Lambda function for comparing Values
    std::sort(elements.begin(), elements.end(), [](const Value &a, const Value &b)
              {
        // Try numeric comparison first
        if (a.isInt() && b.isInt())
        {
            return a.asInt() < b.asInt();
        }
        else if ((a.isInt() || a.isDouble()) && (b.isInt() || b.isDouble()))
        {
            double aVal = a.isInt() ? a.asInt() : a.asDouble();
            double bVal = b.isInt() ? b.asInt() : b.asDouble();
            return aVal < bVal;
        }
        // Fall back to string comparison
        return a.toString() < b.toString(); });
}

void DynamicArray::sortDescending()
{
    // Lambda function for comparing Values
    std::sort(elements.begin(), elements.end(), [](const Value &a, const Value &b)
              {
        // Try numeric comparison first
        if (a.isInt() && b.isInt())
        {
            return a.asInt() > b.asInt();
        }
        else if ((a.isInt() || a.isDouble()) && (b.isInt() || b.isDouble()))
        {
            double aVal = a.isInt() ? a.asInt() : a.asDouble();
            double bVal = b.isInt() ? b.asInt() : b.asDouble();
            return aVal > bVal;
        }
        // Fall back to string comparison
        return a.toString() > b.toString(); });
}

// Modification
void DynamicArray::append(const Value &value)
{
    elements.push_back(value);
}

void DynamicArray::concatenate(const DynamicArray &other)
{
    // Reserve space for efficiency
    elements.reserve(elements.size() + other.elements.size());

    // Append all elements from other array
    for (size_t i = 0; i < other.elements.size(); ++i)
    {
        elements.push_back(other.elements[i]);
    }
}

void DynamicArray::clear()
{
    elements.clear();
}

// Search operations
bool DynamicArray::contains(const Value &value) const
{
    return indexOf(value) != -1;
}

int DynamicArray::indexOf(const Value &value) const
{
    for (size_t i = 0; i < elements.size(); ++i)
    {
        // Compare values
        // This is simplified and might need more complex equality logic
        if (elements[i].toString() == value.toString())
        {
            return static_cast<int>(i);
        }
    }
    return -1; // Not found
}

// Transformations
DynamicArray DynamicArray::slice(int start, int end) const
{
    // Adjust negative indices (Python-like behavior)
    if (start < 0)
        start = static_cast<int>(elements.size()) + start;
    if (end < 0)
        end = static_cast<int>(elements.size()) + end;

    // Bounds checking
    if (start < 0)
        start = 0;
    if (end > static_cast<int>(elements.size()))
        end = static_cast<int>(elements.size());

    // Invalid range
    if (start >= end || start >= static_cast<int>(elements.size()))
    {
        return DynamicArray(); // Return empty array
    }

    // Create result array
    std::vector<Value> sliced(elements.begin() + start, elements.begin() + end);
    return DynamicArray(sliced);
}

DynamicArray DynamicArray::map(std::function<Value(const Value &)> mapFunction) const
{
    std::vector<Value> result;
    result.reserve(elements.size());

    for (const auto &element : elements)
    {
        result.push_back(mapFunction(element));
    }

    return DynamicArray(result);
}

DynamicArray DynamicArray::filter(std::function<bool(const Value &)> filterFunction) const
{
    std::vector<Value> result;

    for (const auto &element : elements)
    {
        if (filterFunction(element))
        {
            result.push_back(element);
        }
    }

    return DynamicArray(result);
}

// String representation
std::string DynamicArray::toString() const
{
    std::stringstream ss;
    ss << "[";

    for (size_t i = 0; i < elements.size(); ++i)
    {
        ss << elements[i].toString();

        if (i < elements.size() - 1)
        {
            ss << ", ";
        }
    }

    ss << "]";
    return ss.str();
}