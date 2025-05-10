#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include <functional>
#include <memory>
#include <variant>

// Forward declerations
class Interpreter;
class AST_NODE;

using ValueType = std::variant<
    int,
    double,
    std::string,
    char,
    bool>;

/**
 * @brief This class represents a dynamic array
 *
 * better array handling then just initializing basic arrays and trying to hanndle them
 */

class DynamicArray
{
public:
    /**
     * @brief Constructor for the DynamicArray class
     * @param elementType The type of elements this array will store
     *
     * Initializes a new dynamic array with the specified element type.
     */
    DynamicArray(const std::string &elementType);

    /**
     * @brief Destructor for the DynamicArray class
     *
     * Cleans up any resources used by the array.
     */
    ~DynamicArray();

    /**
     * @brief Get the type of elements stored in this array
     * @return String representation of the element type
     */
    std::string getElementType() const;

    /**
     * @brief Initialize the array with a list of values
     * @param values Vector of values to initialize the array with
     *
     * Sets the initial content of the array to the provided values.
     */
    void initialize(const std::vector<ValueType> &values);

    /**
     * @brief Initialize the array with values in a range
     * @param start Starting value of the range (inclusive)
     * @param end Ending value of the range (inclusive)
     *
     * Creates an array containing all values from start to end.
     */
    void initializeRange(int start, int end);

    /**
     * @brief Initialize the array by repeating a value
     * @param value The value to repeat
     * @param count The number of times to repeat the value
     *
     * Creates an array containing 'count' copies of 'value'.
     */
    void initializeRepeat(const ValueType &value, int count);

    /**
     * @brief Retrieve an element at the specified index
     * @param index The index of the element to retrieve
     * @return The value at the specified index
     * @throws std::out_of_range if the index is out of bounds
     *
     * Gets the element at the specified index.
     */
    ValueType getElement(int index) const;

    /**
     * @brief Retrieve the last element in the array
     * @return The last element in the array
     * @throws std::out_of_range if the array is empty
     *
     * Gets the last element in the array.
     */
    ValueType getLastElement() const;

    /**
     * @brief Set an element at the specified index
     * @param index The index where to set the element
     * @param value The value to set
     * @throws std::out_of_range if the index is out of bounds
     *
     * Sets the element at the specified index to the given value.
     */
    void setElement(int index, const ValueType &value);

    /**
     * @brief Insert an element at the specified index
     * @param index The index where to insert the element
     * @param value The value to insert
     * @throws std::out_of_range if the index is invalid
     *
     * Inserts the value at the specified index, shifting elements as needed.
     */
    void insertElement(int index, const ValueType &value);

    /**
     * @brief Remove an element at the specified index
     * @param index The index of the element to remove
     * @throws std::out_of_range if the index is out of bounds
     *
     * Removes the element at the specified index, shifting elements as needed.
     */
    void removeElement(int index);

    /**
     * @brief Get the number of elements in the array
     * @return The number of elements
     *
     * Returns the current length of the array.
     */
    size_t getLength() const;

    /**
     * @brief Sort the array in ascending order
     *
     * Sorts all elements in the array from smallest to largest.
     */
    void sortAscending();

    /**
     * @brief Sort the array in descending order
     *
     * Sorts all elements in the array from largest to smallest.
     */
    void sortDescending();

    /**
     * @brief Append an element to the end of the array
     * @param value The value to append
     *
     * Adds a new element to the end of the array.
     */
    void append(const ValueType &value);

    /**
     * @brief Concatenate another array to this one
     * @param other The array to concatenate
     * @throws std::invalid_argument if the element types don't match
     *
     * Adds all elements from another array to the end of this array.
     */
    void concatenate(const DynamicArray &other);

    /**
     * @brief Clear all elements from the array
     *
     * Removes all elements from the array, resulting in an empty array.
     */
    void clear();

    /**
     * @brief Check if the array contains a specific value
     * @param value The value to search for
     * @return true if the value is found, false otherwise
     *
     * Returns whether the array contains the specified value.
     */
    bool contains(const ValueType &value) const;

    /**
     * @brief Find the first index of a value in the array
     * @param value The value to search for
     * @return The index of the first occurrence, or -1 if not found
     *
     * Returns the index of the first occurrence of the value in the array.
     */
    int indexOf(const ValueType &value) const;

    /**
     * @brief Get a slice of the array
     * @param start The starting index (inclusive)
     * @param end The ending index (exclusive)
     * @return A new array containing the slice
     * @throws std::out_of_range if indices are invalid
     *
     * Returns a new array containing elements from start to end-1.
     */
    DynamicArray slice(int start, int end) const;

    /**
     * @brief Map a function over the array
     * @param mapFunction The function to apply to each element
     * @return A new array with the results
     *
     * Returns a new array where each element is the result of applying
     * the function to the corresponding element in this array.
     */
    DynamicArray map(std::function<ValueType(const ValueType &)> mapFunction) const;

    /**
     * @brief Filter the array based on a predicate
     * @param filterFunction The predicate function
     * @return A new array with elements that satisfy the predicate
     *
     * Returns a new array containing only elements for which the
     * predicate function returns true.
     */
    DynamicArray filter(std::function<bool(const ValueType &)> filterFunction) const;

    /**
     * @brief Convert the array to a string representation
     * @return String representation of the array
     *
     * Returns a string representation of the array, useful for debugging.
     */
    std::string toString() const;

private:
    std::string elementType;         // Type of elements in the array
    std::vector<ValueType> elements; // Actual storage for elements

    /**
     * @brief Validate that a value matches the array's element type
     * @param value The value to validate
     * @throws std::invalid_argument if the value has the wrong type
     *
     * Checks that the value's type matches the array's element type.
     */
    void validateType(const ValueType &value) const;

    /**
     * @brief Compare two values for sorting
     * @param a First value
     * @param b Second value
     * @param ascending Sort direction (true for ascending, false for descending)
     * @return Comparison result based on sort direction
     *
     * Helper method to compare elements during sorting.
     */
    bool compareElements(const ValueType &a, const ValueType &b, bool ascending) const;

    /**
     * @brief Convert a value to the array's element type
     * @param value The value to convert
     * @return The converted value
     * @throws std::invalid_argument if conversion is not possible
     *
     * Attempts to convert the value to the array's element type.
     */
    ValueType convertToElementType(const ValueType &value) const;

    // Friend classes
    friend class Interpreter;
};
#endif