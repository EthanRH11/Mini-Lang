#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/**
 * @class ErrorHandler
 * @brief Singleton error handler for minilang interpreter
 */
class ErrorHandler
{
public:
    // Error types enum
    enum ErrorType
    {
        LEXICAL_ERROR,
        SYNTAX_ERROR,
        SEMANTIC_ERROR,
        RUNTIME_ERROR
    };

    // Error struct
    struct Error
    {
        ErrorType type;
        std::string message;

        Error(ErrorType t, const std::string &msg)
            : type(t), message(msg) {}
    };

    /**
     * @brief Get the singleton instance
     * @return Reference to the global ErrorHandler instance
     */
    static ErrorHandler &getInstance()
    {
        static ErrorHandler instance; // Guaranteed to be initialized only once
        return instance;
    }

    // Delete copy constructor and assignment operator
    ErrorHandler(const ErrorHandler &) = delete;
    void operator=(const ErrorHandler &) = delete;

    /**
     * @brief Reports a lexical error
     * @param message The error message
     */
    void reportLexicalError(const std::string &msg)
    {
        addError(LEXICAL_ERROR, msg);
        std::cerr << "[ Lexical Error ] " << msg << std::endl;
    }

    /**
     * @brief Reports a syntax error
     * @param message The error message
     */
    void reportSyntaxError(const std::string &message)
    {
        addError(SYNTAX_ERROR, message);
        std::cerr << "< Syntax Error > " << message << std::endl;
    }

    /**
     * @brief Reports a semantic error
     * @param message The error message
     */
    void reportSemanticError(const std::string &message)
    {
        addError(SEMANTIC_ERROR, message);
        std::cerr << "{ Semantic Error } " << message << std::endl;
    }

    /**
     * @brief Reports a runtime error
     * @param message The error message
     */
    void reportRuntimeError(const std::string &message)
    {
        addError(RUNTIME_ERROR, message);
        std::cerr << "RUNTIME ERROR";
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
        // Same implementation as before
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
    // Private constructor for singleton pattern
    ErrorHandler() : hasErrors(false) {}

    std::vector<Error> errors;
    bool hasErrors;

    /**
     * @brief Adds an error to the error list
     * @param type The type of error
     * @param message The error message
     */
    void addError(ErrorType type, const std::string &message)
    {
        errors.push_back(Error(type, message));
        hasErrors = true;
    }
};

#endif // ERROR_HANDLER_HPP