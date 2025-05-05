#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/**
 * @class ErrorHandler
 * @brief Handlers error reporting and tracking for minilang interpreter
 *
 * This class provides methods for reporting errors in lexical analysis,
 * syntax parsing, and interpretation phases. It tracks errors and provides
 * utilities for displaying them to the user.
 */
class ErrorHandler
{
public:
    /**
     * @brief Enum representing different types of errors
     */
    enum ErrorType
    {
        LEXICAL_ERROR,
        SYNTAX_ERROR,
        SEMANTIC_ERROR,
        RUNTIME_ERROR
    };

    /**
     * @brief Structure to hold information about an error
     */
    struct Error
    {
        ErrorType type;
        std::string message;
        int line;
        int column;

        Error(ErrorType t, const std::string &msg, int ln = -1, int col = -1)
            : type(t), message(msg), line(ln), column(col) {}
    };

    /**
     * @brief constructor
     */
    ErrorHandler() : hasErrors(false) {}

    /**
     * @brief Reports a lexical error
     * @param message (The error message)
     * @param line (Line where error occured)
     * @param column (Column where error occured)
     */
    void reportLexicalError(const std::string &msg, int line, int column)
    {
        addError(LEXICAL_ERROR, msg, line, column);
        std::cerr << "Lexical Error at line: " << line << ", column: " << column
                  << ": " << msg << std::endl;
    }

    /**
     * @brief Reports a syntax error
     * @param message The error message
     * @param line Line number where the error occurred
     * @param column Column number where the error occurred
     */
    void reportSyntaxError(const std::string &message, int line, int column)
    {
        addError(SYNTAX_ERROR, message, line, column);
        std::cerr << "Syntax Error at line " << line << ", column " << column
                  << ": " << message << std::endl;
    }

    /**
     * @brief Reports a semantic error
     * @param message The error message
     * @param line Line number where the error occurred
     * @param column Column number where the error occurred
     */
    void reportSemanticError(const std::string &message, int line, int column)
    {
        addError(SEMANTIC_ERROR, message, line, column);
        std::cerr << "Semantic Error at line " << line << ", column " << column
                  << ": " << message << std::endl;
    }

    /**
     * @brief Reports a runtime error
     * @param message The error message
     * @param line Line number where the error occurred
     * @param column Column number where the error occurred
     */
    void reportRuntimeError(const std::string &message, int line = -1, int column = -1)
    {
        addError(RUNTIME_ERROR, message, line, column);

        std::cerr << "Runtime Error";
        if (line >= 0)
        {
            std::cerr << " at line " << line;
            if (column >= 0)
            {
                std::cerr << ", column " << column;
            }
        }
        std::cerr << ": " << message << std::endl;
    }

    /**
     * @brief Checks if any errors have been reported
     * @return true if there are errors, false otherwise
     */
    bool hasError() const
    {
        return hasErrors;
    }

    /**
     * @brief Gets all reported errors
     * @return A vector of Error Objects
     */
    const std::vector<Error> &getErrors() const
    {
        return errors;
    }

    /**
     * @brief Gets a string representation of all errors
     * @return A formatted string of all errors
     */
    std::string getErrorReport() const
    {
        std::stringstream ss;

        if (errors.empty())
        {
            ss << "No Errors Detected.";
            return ss.str();
        }

        ss << "Found " << errors.size() << " error(s): " << std::endl;

        for (size_t i = 0; i < errors.size(); ++i)
        {
            const Error &error = errors[i];

            ss << (i + 1) << ". ";

            switch (error.type)
            {
            case LEXICAL_ERROR:
                ss << "Lexical Error: ";
                break;
            case SYNTAX_ERROR:
                ss << "Syntax Error: ";
                break;
            case SEMANTIC_ERROR:
                ss << "Semantic Error: ";
                break;
            case RUNTIME_ERROR:
                ss << "Runtime Error: ";
                break;
            }

            ss << error.message;

            if (error.line >= 0)
            {
                ss << " (Line: " << error.line;
                if (error.column >= 0)
                {
                    ss << ", Column: " << error.column;
                }
                ss << ")";
            }
            ss << std::endl;
        }
        return ss.str();
    }

    /**
     * @brief Clears all errors
     */
    void clear()
    {
        errors.clear();
        hasErrors = false;
    }

private:
    std::vector<Error> errors;
    bool hasErrors;

    /**
     * @brief Adds an error to the error list
     * @param type The type of error
     * @param message The error message
     * @param line Line number where the error occurred
     * @param column Column number where the error occurred
     */
    void addError(ErrorType type, const std::string &message, int line, int column)
    {
        errors.push_back(Error(type, message, line, column));
        hasErrors = true;
    }
};

#endif // ERROR_HANDLER_HPP
