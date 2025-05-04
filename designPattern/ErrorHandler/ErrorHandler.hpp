#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <unordered_map>

// Define error levels
enum class ErrorLevel
{
    INFO,    // Informational message
    WARNING, // Warning - program can continue
    ERROR,   // Error - may not be able to continue
    FATAL    // Fatal error - cannot continue
};

// Define error types
enum class ErrorType
{
    LEXICAL,  // Errors during lexical analysis
    SYNTAX,   // Errors during parsing
    SEMANTIC, // Errors during semantic analysis
    RUNTIME   // Errors during execution
};

// Error location in source code
struct SourceLocation
{
    std::string filename;
    size_t line;
    size_t column;
    size_t position;

    SourceLocation(const std::string &file = "", size_t l = 0, size_t col = 0, size_t pos = 0)
        : filename(file), line(l), column(col), position(pos) {}

    std::string toString() const
    {
        std::stringstream ss;
        ss << filename << ":" << line << ":" << column;
        return ss.str();
    }
};

// Base Error class
class Error
{
protected:
    ErrorLevel level;
    ErrorType type;
    std::string message;
    SourceLocation location;

public:
    Error(ErrorLevel lvl, ErrorType typ, const std::string &msg, const SourceLocation &loc)
        : level(lvl), type(typ), message(msg), location(loc) {}

    virtual ~Error() = default;

    ErrorLevel getLevel() const { return level; }
    ErrorType getType() const { return type; }
    const std::string &getMessage() const { return message; }
    const SourceLocation &getLocation() const { return location; }

    // Format error for display
    virtual std::string format() const
    {
        std::stringstream ss;

        // Error level prefix
        switch (level)
        {
        case ErrorLevel::INFO:
            ss << "[INFO] ";
            break;
        case ErrorLevel::WARNING:
            ss << "[WARNING] ";
            break;
        case ErrorLevel::ERROR:
            ss << "[ERROR] ";
            break;
        case ErrorLevel::FATAL:
            ss << "[FATAL] ";
            break;
        }

        // Error type
        switch (type)
        {
        case ErrorType::LEXICAL:
            ss << "Lexical error: ";
            break;
        case ErrorType::SYNTAX:
            ss << "Syntax error: ";
            break;
        case ErrorType::SEMANTIC:
            ss << "Semantic error: ";
            break;
        case ErrorType::RUNTIME:
            ss << "Runtime error: ";
            break;
        }

        ss << message << " at " << location.toString();
        return ss.str();
    }
};

// Specific error types
class LexicalError : public Error
{
public:
    LexicalError(ErrorLevel lvl, const std::string &msg, const SourceLocation &loc)
        : Error(lvl, ErrorType::LEXICAL, msg, loc) {}
};

class SyntaxError : public Error
{
public:
    SyntaxError(ErrorLevel lvl, const std::string &msg, const SourceLocation &loc)
        : Error(lvl, ErrorType::SYNTAX, msg, loc) {}
};

class SemanticError : public Error
{
public:
    SemanticError(ErrorLevel lvl, const std::string &msg, const SourceLocation &loc)
        : Error(lvl, ErrorType::SEMANTIC, msg, loc) {}
};

class RuntimeError : public Error
{
public:
    RuntimeError(ErrorLevel lvl, const std::string &msg, const SourceLocation &loc)
        : Error(lvl, ErrorType::RUNTIME, msg, loc) {}
};

// Source code manager to convert positions to line/column numbers
class SourceManager
{
private:
    std::unordered_map<std::string, std::vector<size_t>> lineStartIndices;

public:
    // Register a source file
    void registerSource(const std::string &filename, const std::string &source)
    {
        std::vector<size_t> indices;
        indices.push_back(0); // First line starts at index 0

        for (size_t i = 0; i < source.length(); ++i)
        {
            if (source[i] == '\n')
            {
                indices.push_back(i + 1);
            }
        }

        lineStartIndices[filename] = indices;
    }

    // Convert a position to line and column
    SourceLocation getLocationInfo(const std::string &filename, size_t position)
    {
        const auto &indices = lineStartIndices[filename];

        // Binary search to find the line
        size_t low = 0;
        size_t high = indices.size() - 1;

        while (low < high)
        {
            size_t mid = (low + high + 1) / 2;

            if (indices[mid] <= position)
            {
                low = mid;
            }
            else
            {
                high = mid - 1;
            }
        }

        size_t line = low + 1;                       // Lines are 1-indexed
        size_t column = position - indices[low] + 1; // Columns are 1-indexed

        return SourceLocation(filename, line, column, position);
    }
};

// The main error handler class
class ErrorHandler
{
private:
    static ErrorHandler instance;

    std::vector<std::unique_ptr<Error>> errors;
    SourceManager sourceManager;
    bool hasError = false;
    bool hasFatalError = false;
    std::ostream *outputStream = &std::cerr;

    // Private constructor for singleton
    ErrorHandler() {}

public:
    // Get the singleton instance
    static ErrorHandler &getInstance()
    {
        return instance;
    }

    // Register a source file
    void registerSource(const std::string &filename, const std::string &source)
    {
        sourceManager.registerSource(filename, source);
    }

    // Set the output stream
    void setOutputStream(std::ostream &os)
    {
        outputStream = &os;
    }

    // Report a generic error
    void reportError(std::unique_ptr<Error> error)
    {
        if (error->getLevel() == ErrorLevel::ERROR ||
            error->getLevel() == ErrorLevel::FATAL)
        {
            hasError = true;
        }

        if (error->getLevel() == ErrorLevel::FATAL)
        {
            hasFatalError = true;
        }

        // Output to stream immediately
        *outputStream << error->format() << std::endl;

        // Store for later reference
        errors.push_back(std::move(error));
    }

    // Helper methods for specific error types
    void reportLexicalError(ErrorLevel level, const std::string &message,
                            const std::string &filename, size_t position)
    {
        SourceLocation location = sourceManager.getLocationInfo(filename, position);
        reportError(std::make_unique<LexicalError>(level, message, location));
    }

    void reportSyntaxError(ErrorLevel level, const std::string &message,
                           const std::string &filename, size_t position)
    {
        SourceLocation location = sourceManager.getLocationInfo(filename, position);
        reportError(std::make_unique<SyntaxError>(level, message, location));
    }

    void reportSemanticError(ErrorLevel level, const std::string &message,
                             const std::string &filename, size_t position)
    {
        SourceLocation location = sourceManager.getLocationInfo(filename, position);
        reportError(std::make_unique<SemanticError>(level, message, location));
    }

    void reportRuntimeError(ErrorLevel level, const std::string &message,
                            const std::string &filename, size_t position)
    {
        SourceLocation location = sourceManager.getLocationInfo(filename, position);
        reportError(std::make_unique<RuntimeError>(level, message, location));
    }

    // Check if errors occurred
    bool hasErrors() const { return hasError; }
    bool hasFatalErrors() const { return hasFatalError; }

    // Get all errors
    const std::vector<std::unique_ptr<Error>> &getErrors() const
    {
        return errors;
    }

    // Clear all errors
    void clearErrors()
    {
        errors.clear();
        hasError = false;
        hasFatalError = false;
    }
};

// Initialize the static instance
ErrorHandler ErrorHandler::instance;

#endif // ERROR_HANDLER_HPP