#include "dynamic_array.hpp"
#include "interperter.hpp"

#include <vector>
#include <algorithm>
#include <cassert>
#include <list>

/**
 * @brief Constructor for the DynamicArray class
 * @param elementType The type of elements this array will store
 *
 * Initializes a new dynamic array with the specified element type.
 */
DynamicArray::DynamicArray(const std::string &elementType)
{
    this->elementType = elementType;
}

/**
 * @brief Destructor for the DynamicArray class
 *
 * Cleans up any resources used by the array.
 */
DynamicArray::~DynamicArray() {}

/**
 * @brief Get the type of elements stored in this array
 * @return String representation of the element type
 */
std::string DynamicArray::getElementType() const
{
    return elementType;
}

/**
 * @brief Initialize the array with a list of values
 * @param values Vector of values to initialize the array with
 *
 * Sets the initial content of the array to the provided values.
 */
void DynamicArray::initialize(const std::vector<ValueType> &values)
{
    elements.clear();

    for (const auto &value : values)
    {
        validateType(value);
    }

    elements = values;
}

/**
 * @brief Initialize the array with values in a range
 * @param start Starting value of the range (inclusive)
 * @param end Ending value of the range (inclusive)
 *
 * Creates an array containing all values from start to end.
 */
void DynamicArray::initializeRange(int start, int end)
{
    elements.clear();

    if (start > end)
    {
        std::cerr << "The start range is greater than the end range";
        exit(1);
    }

    if (elementType == "int")
    {
        for (int i = start; i <= end; i++)
        {
            elements.push_back(i);
        }
    }
    else if (elementType == "double")
    {
        for (int i = start; i <= end; i++)
        {
            elements.push_back(static_cast<double>(i));
        }
    }
    else
    {
        std::cerr << "Range initialization is only support for numeric types";
        exit(1);
    }
}

/**
 * @brief Initialize the array by repeating a value
 * @param value The value to repeat
 * @param count The number of times to repeat the value
 *
 * Creates an array containing 'count' copies of 'value'.
 */
void DynamicArray::initializeRepeat(const ValueType &value, int count)
{
    elements.clear();

    validateType(value);

    if (count < 0)
    {
        std::cerr << "Error: Cannot repeat a value a negative number of times." << std::endl;
        exit(1);
    }

    for (int i = 0; i <= count; i++)
    {
        elements.push_back(value);
    }
}

/**
 * @brief Retrieve an element at the specified index
 * @param index The index of the element to retrieve
 * @return The value at the specified index
 * @throws std::out_of_range if the index is out of bounds
 *
 * Gets the element at the specified index.
 */
ValueType DynamicArray::getElement(int index) const
{
    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds: " + std::to_string(index));
    }

    return elements[index];
}

/**
 * @brief Retrieve the last element in the array
 * @return The last element in the array
 * @throws std::out_of_range if the array is empty
 *
 * Gets the last element in the array.
 */
ValueType DynamicArray::getLastElement() const
{
    return elements.back();
}

/**
 * @brief Set an element at the specified index
 * @param index The index where to set the element
 * @param value The value to set
 * @throws std::out_of_range if the index is out of bounds
 *
 * Sets the element at the specified index to the given value.
 */
void DynamicArray::setElement(int index, const ValueType &value)
{
    validateType(value);

    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds: " + std::to_string(index));
    }

    elements[index] = value;
}

/**
 * @brief Insert an element at the specified index
 * @param index The index where to insert the element
 * @param value The value to insert
 * @throws std::out_of_range if the index is invalid
 *
 * Inserts the value at the specified index, shifting elements as needed.
 */
void DynamicArray::insertElement(int index, const ValueType &value)
{
    validateType(value);

    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds: " + std::to_string(index));
    }

    elements.insert(elements.begin() + index, value);
}

/**
 * @brief Remove an element at the specified index
 * @param index The index of the element to remove
 * @throws std::out_of_range if the index is out of bounds
 *
 * Removes the element at the specified index, shifting elements as needed.
 */
void DynamicArray::removeElement(int index)
{
    if (index < 0 || index >= static_cast<int>(elements.size()))
    {
        throw std::out_of_range("Array index out of bounds: " + std::to_string(index));
    }

    elements.erase(elements.begin() + index);
}

/**
 * @brief Get the number of elements in the array
 * @return The number of elements
 *
 * Returns the current length of the array.
 */
size_t DynamicArray::getLength() const
{
    return elements.size();
}

/**
 * @brief Sort the array in ascending order
 *
 * Sorts all elements in the array from smallest to largest.
 */
void DynamicArray::sortAscending()
{
    std::sort(elements.begin(), elements.end(),
              [this](const ValueType &a, const ValueType &b)
              {
                  return this->compareElements(a, b, true);
              });
}

/**
 * @brief Sort the array in descending order
 *
 * Sorts all elements in the array from largest to smallest.
 */
void DynamicArray::sortDescending()
{
    std::sort(elements.begin(), elements.end(),
              [this](const ValueType &a, ValueType &b)
              {
                  return this->compareElements(a, b, false);
              });
}

/**
 * @brief Append an element to the end of the array
 * @param value The value to append
 *
 * Adds a new element to the end of the array.
 */
void DynamicArray::append(const ValueType &value)
{
    validateType(value);

    elements.push_back(value);
}

/**
 * @brief Concatenate another array to this one
 * @param other The array to concatenate
 * @throws std::invalid_argument if the element types don't match
 *
 * Adds all elements from another array to the end of this array.
 */
void DynamicArray::concatenate(const DynamicArray &other)
{
    if (elementType != other.elementType)
    {
        throw std::invalid_argument("Cannot concatenate arrays of different types: " +
                                    elementType + " and " + other.elementType);

        elements.insert(elements.end(), other.elements.begin(), other.elements.end());
    }
}

/**
 * @brief Clear all elements from the array
 *
 * Removes all elements from the array, resulting in an empty array.
 */
void DynamicArray::clear()
{
    elements.clear();
}

/**
 * @brief Check if the array contains a specific value
 * @param value The value to search for
 * @return true if the value is found, false otherwise
 *
 * Returns whether the array contains the specified value.
 */
bool DynamicArray::contains(const ValueType &value) const
{
    validateType(value);

    return std::find(elements.begin(), elements.end(), value) != elements.end();
}

/**
 * @brief Find the first index of a value in the array
 * @param value The value to search for
 * @return The index of the first occurrence, or -1 if not found
 *
 * Returns the index of the first occurrence of the value in the array.
 */
int DynamicArray::indexOf(const ValueType &value) const
{
    validateType(value);

    auto it = std::find(elements.begin(), elements.end(), value);

    if (it != elements.end())
    {
        return static_cast<int>(std::distance(elements.begin(), it));
    }

    return -1;
}

/**
 * @brief Get a slice of the array
 * @param start The starting index (inclusive)
 * @param end The ending index (exclusive)
 * @return A new array containing the slice
 * @throws std::out_of_range if indices are invalid
 *
 * Returns a new array containing elements from start to end-1.
 */
DynamicArray DynamicArray::slice(int start, int end) const
{
    if (start < 0 || end > static_cast<int>(elements.size()) || start > end)
    {
        throw std::out_of_range("Invalid slice indices: start=" +
                                std::to_string(start) + ", end=" +
                                std::to_string(end));
    }

    DynamicArray result(this->elementType);

    std::copy(elements.begin() + start, elements.begin() + end, std::back_inserter(result.elements));

    return result;
}

/**
 * @brief Map a function over the array
 * @param mapFunction The function to apply to each element
 * @return A new array with the results
 *
 * Returns a new array where each element is the result of applying
 * the function to the corresponding element in this array.
 */
DynamicArray DynamicArray::map(std::function<ValueType(const ValueType &)> mapFunction) const
{
    DynamicArray result(this->elementType);

    for (const auto &value : elements)
    {
        ValueType newValue = mapFunction(value);
        result.validateType(newValue);
        result.elements.push_back(newValue);
    }
    return result;
}

/**
 * @brief Filter the array based on a predicate
 * @param filterFunction The predicate function
 * @return A new array with elements that satisfy the predicate
 *
 * Returns a new array containing only elements for which the
 * predicate function returns true.
 */
DynamicArray DynamicArray::filter(std::function<bool(const ValueType &)> filterFunction) const
{
    DynamicArray result(this->elementType);

    for (const auto &value : elements)
    {
        if (filterFunction(value))
        {
            result.elements.push_back(value);
        }
    }
    return result;
}

/**
 * @brief Convert the array to a string representation
 * @return String representation of the array
 *
 * Returns a string representation of the array, useful for debugging.
 */
std::string DynamicArray::toString() const
{
    std::stringstream ss;
    ss << "[";

    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (i > 0)
        {
            ss << ", ";
        }

        const ValueType &value = elements[i];

        // Handle different types in the variant
        if (elementType == "int")
        {
            ss << std::get<int>(value);
        }
        else if (elementType == "double")
        {
            ss << std::get<double>(value);
        }
        else if (elementType == "string" || elementType == "str")
        {
            ss << "\"" << std::get<std::string>(value) << "\"";
        }
        else if (elementType == "char")
        {
            ss << "'" << std::get<char>(value) << "'";
        }
        else if (elementType == "bool")
        {
            ss << (std::get<bool>(value) ? "true" : "false");
        }
        else
        {
            ss << "?"; // Unknown type
        }
    }

    ss << "]";
    return ss.str();
}

/**
 * @brief Validate that a value matches the array's element type
 * @param value The value to validate
 * @throws std::invalid_argument if the value has the wrong type
 *
 * Checks that the value's type matches the array's element type.
 */
void DynamicArray::validateType(const ValueType &value) const
{
    bool typeMatches = false;

    // Check if the variant holds the expected type
    if (elementType == "int")
    {
        typeMatches = std::holds_alternative<int>(value);
    }
    else if (elementType == "double")
    {
        typeMatches = std::holds_alternative<double>(value);
    }
    else if (elementType == "string" || elementType == "str")
    {
        typeMatches = std::holds_alternative<std::string>(value);
    }
    else if (elementType == "char")
    {
        typeMatches = std::holds_alternative<char>(value);
    }
    else if (elementType == "bool")
    {
        typeMatches = std::holds_alternative<bool>(value);
    }

    if (!typeMatches)
    {
        throw std::invalid_argument("Value type does not match array type " + elementType);
    }
}

/**
 * @brief Compare two values for sorting
 * @param a First value
 * @param b Second value
 * @param ascending Sort direction (true for ascending, false for descending)
 * @return Comparison result based on sort direction
 *
 * Helper method to compare elements during sorting.
 */
bool DynamicArray::compareElements(const ValueType &a, const ValueType &b, bool ascending) const
{
    bool result = false;

    // Compare based on the element type
    if (elementType == "int")
    {
        result = std::get<int>(a) < std::get<int>(b);
    }
    else if (elementType == "double")
    {
        result = std::get<double>(a) < std::get<double>(b);
    }
    else if (elementType == "string" || elementType == "str")
    {
        result = std::get<std::string>(a) < std::get<std::string>(b);
    }
    else if (elementType == "char")
    {
        result = std::get<char>(a) < std::get<char>(b);
    }
    else if (elementType == "bool")
    {
        result = !std::get<bool>(a) && std::get<bool>(b); // false comes before true
    }

    // For descending order, invert the result
    return ascending ? result : !result;
}

/**
 * @brief Convert a value to the array's element type
 * @param value The value to convert
 * @return The converted value
 * @throws std::invalid_argument if conversion is not possible
 *
 * Attempts to convert the value to the array's element type.
 */
ValueType DynamicArray::convertToElementType(const ValueType &value) const
{ // Try to convert between compatible types
    try
    {
        if (elementType == "int")
        {
            if (std::holds_alternative<double>(value))
            {
                return static_cast<int>(std::get<double>(value));
            }
            else if (std::holds_alternative<char>(value))
            {
                return static_cast<int>(std::get<char>(value));
            }
            else if (std::holds_alternative<bool>(value))
            {
                return std::get<bool>(value) ? 1 : 0;
            }
        }
        else if (elementType == "double")
        {
            if (std::holds_alternative<int>(value))
            {
                return static_cast<double>(std::get<int>(value));
            }
        }
        else if (elementType == "string" || elementType == "str")
        {
            std::stringstream ss;
            if (std::holds_alternative<int>(value))
            {
                ss << std::get<int>(value);
                return ss.str();
            }
            else if (std::holds_alternative<double>(value))
            {
                ss << std::get<double>(value);
                return ss.str();
            }
            else if (std::holds_alternative<char>(value))
            {
                ss << std::get<char>(value);
                return ss.str();
            }
            else if (std::holds_alternative<bool>(value))
            {
                ss << (std::get<bool>(value) ? "true" : "false");
                return ss.str();
            }
        }
        else if (elementType == "char")
        {
            if (std::holds_alternative<int>(value))
            {
                int intValue = std::get<int>(value);
                if (intValue >= 0 && intValue <= 127)
                { // ASCII range
                    return static_cast<char>(intValue);
                }
            }
        }
        else if (elementType == "bool")
        {
            if (std::holds_alternative<int>(value))
            {
                return std::get<int>(value) != 0;
            }
        }
    }
    catch (const std::bad_variant_access &)
    {
        // Thrown if we try to get the wrong type from the variant
    }

    // If we can't convert, or if the value already has the right type
    validateType(value); // This will throw if the type doesn't match
    return value;        // Return the original value if it has the right type
}