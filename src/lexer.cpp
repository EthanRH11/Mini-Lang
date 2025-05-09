#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>

#include "lexer.hpp"

/**
 * @brief Constructor for Lexer class
 * @param sourceCode The source code to be tokenized
 *
 * Initializes the lexer with the provided source code and sets up the cursor
 * to begin tokenization from the start of the input.
 */
Lexer::Lexer(std::string sourceCode)
{
    source = sourceCode;
    cursor = 0;
    current = sourceCode.at(cursor);
    size = sourceCode.length();

    isArrayType = false;
    initializeLexerMaps();
}

/**
 * @brief Initializes maps to make operator and keyword access quicker
 * @param unordered_map The maps of the different types of operators & keywords
 *
 * Creates necessary maps of operators and keywords
 */
void Lexer::initializeLexerMaps()
{
    singleCharMap = {
        {'+', TOKEN_OPERATOR_ADD},
        {'-', TOKEN_OPERATOR_SUBT},
        {'*', TOKEN_OPERATOR_MULT},
        {'/', TOKEN_OPERATOR_DIV},
        {'=', TOKEN_EQUALS},
        {';', TOKEN_SEMICOLON},
        {'(', TOKEN_LEFT_PAREN},
        {')', TOKEN_RIGHT_PAREN},
        {'<', TOKEN_OPERATOR_LESSTHAN},
        {'>', TOKEN_OPERATOR_GREATERTHAN},
        {'{', TOKEN_LEFT_CURL},
        {'}', TOKEN_RIGHT_CURL},
        {',', TOKEN_COMMA},
        {'#', TOKEN_ARRAY_LENGTH},
        {'@', TOKEN_ARRAY_ACCESS},
        {'$', TOKEN_ARRAY_LAST_INDEX},
        {'.', TOKEN_DOT},
        {'%', TOKEN_OPERATOR_MODULUS}};

    MultiCharMap = {
        {"...", TOKEN_NL_SYMBOL},
        {"==", TOKEN_OPERATOR_EQUALS},
        {">>$", TOKEN_SINGLELINE_COMMENT},
        {"<<$", TOKEN_MULTILINE_COMMENT},
        {"+=", TOKEN_OPERATOR_ADD_ASSIGN},
        {"++", TOKEN_OPERATOR_INCREMENT},
        {"--", TOKEN_OPERATOR_DECREMENT},
        {"<=", TOKEN_OPERATOR_LESS_EQUAL},
        {">=", TOKEN_OPERATOR_GREATER_EQUAL},
        {"=>", TOKEN_SPACESHIP},
        {"|=", TOKEN_ARRAY_INITIALIZER},
        {"..", TOKEN_OPERATOR_ARRAYRANGE},
        {"+>", TOKEN_ARRAY_INSERT},
        {"-<", TOKEN_ARRAY_REMOVE},
        {"~>", TOKEN_ARRAY_SORT_ASC},
        {"<~", TOKEN_ARRAY_SORT_DESC},
        {"=/=", TOKEN_OPERATOR_DOESNT_EQUAL}};

    KeywordMap = {
        {"begin:", TOKEN_KEYWORD_BEGIN},
        {"proc", TOKEN_KEYWORD_FUNCTION},
        {"input", TOKEN_KEYWORD_INPUT},
        {"int", TOKEN_KEYWORD_INT},
        {"double", TOKEN_KEYWORD_DOUBLE},
        {"str", TOKEN_KEYWORD_STR},
        {"check", TOKEN_KEYWORD_CHECK},
        {"char", TOKEN_KEYWORD_CHAR},
        {"out_to_console", TOKEN_KEYWORD_PRINT},
        {"if", TOKEN_KEYWORD_IF},
        {"else", TOKEN_KEYWORD_ELSE},
        {"for", TOKEN_KEYWORD_FOR},
        {"end", TOKEN_EOF},
        {"bool", TOKEN_KEYWORD_BOOL},
        {"result", TOKEN_KEYWORD_RESULT},
        {"elements", TOKEN_KEYWORD_ELEMENT},
        {"repeat", TOKEN_KEYWORD_REPEAT},
        {"range", TOKEN_KEYWORD_RANGE}};
}

/**
 * @brief Advances the cursor position by one character
 * @return The new current character, or '\0' if end of file is reached
 *
 * Moves the cursor one position forward in the source code and updates
 * the current character. Returns '\0' if cursor moves past the end of the source.
 */
char Lexer::advanceCursor()
{
    cursor++;
    if (cursor < size)
    {
        current = source[cursor];
        return current;
    }
    else
    {
        current = '\0';
        return '\0';
    }
}

/**
 * @brief Checks if the upcoming characters match a given keyword
 * @param keyword The keyword to match against
 * @return true if the keyword matches, false otherwise
 *
 * Compares characters starting from the current cursor position
 * with the given keyword without advancing the cursor.
 */
bool Lexer::matchKeyword(const std::string &keyword)
{
    if (cursor + keyword.size() > static_cast<size_t>(size))
    {
        return false;
    }
    return source.substr(cursor, keyword.size()) == keyword;
}

/**
 * @brief Consumes a keyword if it matches the upcoming characters
 * @param keyword The keyword to consume
 * @throws std::runtime_error if the keyword doesn't match
 *
 * Verifies that the upcoming characters match the keyword and advances
 * the cursor past the keyword if it matches. Throws an error otherwise.
 */
void Lexer::consumeKeyword(const std::string &keyword)
{
    if (!matchKeyword(keyword))
    {
        throw std::runtime_error("Error: Keyword mismatch during consumeKeyword.");
    }
    cursor += keyword.length();
    current = (cursor < size) ? source[cursor] : '\0';
}

/**
 * @brief Looks ahead in the source code without advancing the cursor
 * @param offset Number of characters to look ahead
 * @return Character at cursor+offset position, or '\0' if out of bounds
 *
 * Safely peeks at a character ahead of the current cursor position.
 */
char Lexer::peakAhead(int offset)
{
    if (cursor + offset < size)
    {
        return source[cursor + offset];
    }
    else
    {
        return '\0';
    }
}

/**
 * @brief Skips whitespace characters (spaces, tabs, newlines)
 *
 * Advances the cursor until a non-whitespace character is found
 * or the end of file is reached.
 */
void Lexer::checkAndSkip()
{
    while (!eof() && (current == ' ' || current == '\n' || current == '\t' || current == '\r'))
    {
        advanceCursor();
    }
}

/**
 * @brief Checks if the cursor has reached the end of the source code
 * @return true if end of file reached, false otherwise
 */
bool Lexer::eof() const
{
    return cursor >= size;
}

Token *Lexer::processSingleLineComment()
{
    std::string commentText = ">>$";

    while (!eof() && current != '\n' && current != '\r')
    {
        commentText += current;
        advanceCursor();
    }
    return new Token{TOKEN_SINGLELINE_COMMENT, commentText};
}

Token *Lexer::processMultiLineComment()
{
    std::string commentText = "<<$";

    bool foundEndMarker = false;

    while (!eof())
    {
        if (current == '$' && peakAhead(1) == '>' && peakAhead(2) == '>')
        {
            commentText += "$>>";
            advanceCursor();
            advanceCursor();
            advanceCursor();
            foundEndMarker = true;
            break;
        }
        commentText += current;
        advanceCursor();
    }
    if (!foundEndMarker)
    {
        throw std::runtime_error("Error: Unterminated multiLine comment");
    }
    return new Token{TOKEN_MULTILINE_COMMENT, commentText};
}

/**
 * @brief Processes true or false statements
 * @return Token pointer representing the true or false value
 * @throws std::runtime error if true or false is invalid
 */
Token *Lexer::processBool()
{
    std::string boolStr = "";
    while (isalpha(current) && current != ';')
    {
        boolStr += current;
        advanceCursor();
    }

    if (boolStr == "true")
    {
        return new Token{TOKEN_BOOL_VALUE, boolStr};
    }
    else if (boolStr == "false")
    {
        return new Token{TOKEN_BOOL_VALUE, boolStr};
    }
    else
    {
        throw std::runtime_error("Invalid boolean literal: " + boolStr);
    }
}

/**
 * @brief Processes numeric literals (integers and floating-point numbers)
 * @return Token pointer representing the numeric value
 * @throws std::runtime_error if the number format is invalid
 *
 * Parses integer and floating-point literals from the source code.
 * Handles decimal points and validates the number format.
 */
Token *Lexer::processNumber()
{
    std::string number;
    bool isDouble = false;

    // Collect digits and at most one decimal point
    while (std::isdigit(current))
    {
        number += current;
        advanceCursor();
    }

    if (current == '.')
    {
        if (peakAhead(1) == '.')
        {
            return new Token{TOKEN_INTEGER_VAL, number};
        }
        number += current;
        advanceCursor();
        isDouble = true;

        while (std::isdigit(current))
        {
            number += current;
            advanceCursor();
        }
    }

    // Create appropriate token based on whether a decimal point was found
    return new Token{isDouble ? TOKEN_DOUBLE_VAL : TOKEN_INTEGER_VAL, number};
}

/**
 * @brief Processes print keyword
 * @return Token pointer for print keyword or nullptr if not a print statement
 *
 * Checks if the current identifier is the print keyword ("out_to_console").
 */
Token *Lexer::processPrint()
{
    std::string printStatement;
    while (std::isalpha(current) || current == '_')
    {
        printStatement += current;
        advanceCursor();
    }
    if (printStatement == "out_to_console")
    {
        return new Token{TOKEN_KEYWORD_PRINT, printStatement};
    }
    else
    {
        return nullptr;
    }
}

/**
 * @brief Processes operators and punctuation
 * @return Token pointer representing the operator
 * @throws std::runtime_error if the operator is unknown
 *
 * Handles both single-character operators (like +, -, *, /) and
 * multi-character operators (like ==, +=, ++, --).
//  */
Token *Lexer::processOperator()
{
    std::string op(1, current);

    // Check for multi-character operators first
    if (op == ">" && peakAhead(1) == '>' && peakAhead(2) == '$')
    {
        return processSingleLineComment();
    }
    else if (op == "<" && peakAhead(1) == '<' && peakAhead(2) == '$')
    {
        return processMultiLineComment();
    }

    // Check for other potential multi-character operators
    std::string potentialMultiChar = "";

    // Check for 2-character operators
    if (peakAhead(1) != '\0')
    {
        potentialMultiChar = op + peakAhead(1);

        // Check for 3-character operators
        if (peakAhead(2) != '\0')
        {
            std::string threeCharOp = potentialMultiChar + peakAhead(2);

            // Try to match 3-character operators
            auto it3 = MultiCharMap.find(threeCharOp);
            if (it3 != MultiCharMap.end())
            {
                advanceCursor(); // Skip first char
                advanceCursor(); // Skip second char
                advanceCursor(); // Skip third char
                return new Token{it3->second, threeCharOp};
            }
        }

        // Try to match 2-character operators
        auto it2 = MultiCharMap.find(potentialMultiChar);
        if (it2 != MultiCharMap.end())
        {
            advanceCursor(); // Skip first char
            advanceCursor(); // Skip second char
            return new Token{it2->second, potentialMultiChar};
        }
    }

    // It's a single character operator
    advanceCursor(); // Consume the single character

    // Try to match single character operators
    auto it1 = singleCharMap.find(op[0]);
    if (it1 != singleCharMap.end())
    {
        return new Token{it1->second, op};
    }

    throw std::runtime_error("Error: Unknown operator: " + op);
}

/**
 * @brief Processes string literals enclosed in double quotes
 * @return Token pointer representing the string value
 * @throws std::runtime_error if the string is unterminated
 *
 * Handles string literals by collecting characters between double quotes.
 */
Token *Lexer::processStringLiteral()
{
    if (current == '"')
    {
        advanceCursor(); // Skip opening quote
        std::string value;

        // Collect characters until closing quote or EOF
        while (current != '"' && !eof())
        {
            value += current;
            advanceCursor();
        }

        // Verify string was properly terminated
        if (current != '"')
        {
            throw std::runtime_error("Error: Unterminated String Literal.");
        }
        advanceCursor(); // Skip closing quote
        return new Token{TOKEN_STRING_VAL, value};
    }
    else
    {
        throw std::runtime_error("Error: Invalid string literal.");
    }
}

/**
 * @brief Processes character literals enclosed in single quotes
 * @return Token pointer representing the character value
 * @throws std::runtime_error if the character literal is invalid
 *
 * Handles both regular character literals ('a') and special
 * escape sequences like '\n' for newline.
 */
Token *Lexer::processCharLiteral()
{
    // Regular character literal: 'c'
    if (current == '\'' && peakAhead(1) != '\'' && peakAhead(2) == '\'')
    {
        advanceCursor(); // Skip first single quote
        char charValue = current;
        advanceCursor(); // Move to second quote

        if (current != '\'')
        {
            throw std::runtime_error("Error: Invalid character literal.");
        }

        advanceCursor(); // Move past closing quote
        return new Token{TOKEN_CHAR_VAL, std::string(1, charValue)};
    }
    // Special case for newline: '\n'
    else if (current == '\'' && peakAhead(1) == '\\' && peakAhead(2) == 'n' && peakAhead(3) == '\'')
    {
        advanceCursor();             // Skip first single quote
        char newLineValue = current; // Backslash character
        advanceCursor();             // Skip 'n'
        advanceCursor();             // Move to closing quote

        if (current != '\'')
        {
            throw std::runtime_error("Error: Invalid character literal.");
        }
        advanceCursor(); // Move past closing quote
        return new Token{TOKEN_OPERATOR_NEWLINE, std::string(1, newLineValue)};
    }

    throw std::runtime_error("Error: Invalid character literal format.");
}

/**
 * @brief Processes input type specification like <int>, <double>, etc.
 * @return Token pointer representing the input type
 * @throws std::runtime_error if the input type format is invalid
 *
 * Handles input type specifications enclosed in angle brackets.
 */
Token *Lexer::processInputType()
{
    if (current == '<')
    {
        advanceCursor();
        std::string typeName;

        while (current != '>' && !eof())
        {
            typeName += current;
            advanceCursor();
        }

        if (current != '>')
        {
            throw std::runtime_error("Error: Unterminated input type specification.");
        }
        advanceCursor();

        if (typeName == "int" ||
            typeName == "double" ||
            typeName == "char" ||
            typeName == "str" ||
            typeName == "bool")
        {
            return new Token{isArrayType ? TOKEN_ELEMENT_TYPE : TOKEN_INPUT_TYPE, typeName};
        }
        else
        {
            throw std::runtime_error("Error: Invalid input type: " + typeName);
        }
    }
    else
    {
        throw std::runtime_error("Error: Expected '<' for input type specification.");
    }
}

/**
 * @brief Processes keywords and identifiers
 * @param tokens Vector of tokens processed so far (unused)
 * @return Token pointer representing the keyword or identifier
 *
 * Identifies language keywords and user-defined identifiers.
 * Special handling for "begin:" with the colon as part of the token.
 */
Token *Lexer::processKeyword()
{
    std::string keyword;

    // Collect identifier characters
    while (std::isalpha(current) || current == '_')
    {
        keyword += current;
        advanceCursor();
    }

    // Handle special case for "begin:"
    if (keyword == "begin" && current == ':')
    {
        advanceCursor(); // consume colon
        return new Token{TOKEN_KEYWORD_BEGIN, "begin"};
    }

    // Debug print for "range" and "repeat"
    if (keyword == "range" || keyword == "repeat")
    {
        std::cerr << "DEBUG: Found keyword '" << keyword << "'" << std::endl;
    }

    // Look up in keyword map
    auto it = KeywordMap.find(keyword);
    if (it != KeywordMap.end())
    {
        std::cerr << "DEBUG: Found in KeywordMap: " << keyword << " -> "
                  << getTokenTypeName(it->second) << std::endl;

        Token *token = new Token{it->second, keyword};

        // Special handling for input and element keywords
        if ((token->TYPE == TOKEN_KEYWORD_INPUT || token->TYPE == TOKEN_KEYWORD_ELEMENT) && (current == '<'))
        {
            if (token->TYPE == TOKEN_KEYWORD_ELEMENT)
            {
                isArrayType = true;
            }
        }

        return token;
    }

    // If "range" or "repeat" wasn't found in the map, log that too
    if (keyword == "range" || keyword == "repeat")
    {
        std::cerr << "DEBUG: '" << keyword << "' NOT found in KeywordMap" << std::endl;
    }

    // If not found in keyword map, it's an identifier
    return new Token{TOKEN_IDENTIFIER, keyword};
}
// Token *Lexer::processKeyword()
// {
//     std::string keyword;

//     // Collect identifier characters
//     while (std::isalpha(current) || current == '_')
//     {
//         keyword += current;
//         advanceCursor();
//     }

//     // Handle special case for "begin:"
//     if (keyword == "begin" && current == ':')
//     {
//         advanceCursor(); // consume colon
//         return new Token{TOKEN_KEYWORD_BEGIN, "begin"};
//     }

//     // Look up in keyword map
//     auto it = KeywordMap.find(keyword);
//     if (it != KeywordMap.end())
//     {
//         Token *token = new Token{it->second, keyword};

//         // Special handling for input and element keywords
//         if ((token->TYPE == TOKEN_KEYWORD_INPUT || token->TYPE == TOKEN_KEYWORD_ELEMENT) && (current == '<'))
//         {
//             if (token->TYPE == TOKEN_KEYWORD_ELEMENT)
//             {
//                 isArrayType = true;
//             }
//         }

//         return token;
//     }

//     // If not found in keyword map, it's an identifier
//     return new Token{TOKEN_IDENTIFIER, keyword};
// }

/**
 * @brief Main tokenization method that processes the entire source code
 * @return Vector of Token pointers representing the tokenized source
 * @throws std::runtime_error if unexpected characters are encountered
 *
 * Processes the entire source code and returns a vector of tokens
 * that can be used by a parser for syntax analysis.
 */
std::vector<Token *> Lexer::tokenize()
{
    std::vector<Token *> tokens;

    while (!eof())
    {
        // Skip whitespace before processing next token
        checkAndSkip();

        // Process different token types based on the current character
        if (std::isalpha(current))
        {
            if ((current == 't' && matchKeyword("true")) ||
                (current == 'f' && matchKeyword("false")))
            {
                Token *token = processBool();
                tokens.push_back(token);
            }
            else
            {
                // Process keywords and identifiers
                Token *token = processKeyword();
                tokens.push_back(token);

                if (token->TYPE == TOKEN_KEYWORD_ELEMENT)
                {
                    isArrayType = true;
                }

                if ((token->TYPE == TOKEN_KEYWORD_INPUT || token->TYPE == TOKEN_KEYWORD_ELEMENT) && (current == '<'))
                {
                    Token *typeToken = processInputType();
                    if (isArrayType)
                    {
                        typeToken->TYPE = TOKEN_ELEMENT_TYPE;
                        isArrayType = false; // reset flag
                    }
                    tokens.push_back(typeToken);
                }
            }
        }
        else if (std::isdigit(current))
        {
            // Process numeric literals
            Token *token = processNumber();
            tokens.push_back(token);
        }
        else if (std::ispunct(current))
        {
            // Process punctuation and operators
            if (current == '"')
            {
                Token *token = processStringLiteral();
                tokens.push_back(token);
            }
            else if (current == '\'')
            {
                Token *token = processCharLiteral();
                tokens.push_back(token);
            }
            else
            {
                Token *token = processOperator();
                tokens.push_back(token);
            }
        }
        else if (!eof())
        {
            // Handle unexpected characters
            throw std::runtime_error("Error: Unexpected character: " + std::string(1, current));
        }
    }

    return tokens;
}

/**
 * @brief Helper function to get string representation of token types
 * @param type The token type to convert to string
 * @return String representation of the token type
 *
 * Converts the enum token type to a human-readable string for debugging
 * and error reporting purposes.
 */
std::string getTokenTypeName(tokenType type)
{
    // Since TokenMap is in Lexer class, we need to manually map token types to names
    static const std::unordered_map<tokenType, std::string> tokenTypeNames = {
        {TOKEN_INTEGER_VAL, "TOKEN_INTEGER_VAL"},
        {TOKEN_DOUBLE_VAL, "TOKEN_DOUBLE_VAL"},
        {TOKEN_CHAR_VAL, "TOKEN_CHAR_VAL"},
        {TOKEN_STRING_VAL, "TOKEN_STRING_VAL"},
        {TOKEN_EQUALS, "TOKEN_EQUALS"},
        {TOKEN_OPERATOR_ADD, "TOKEN_OPERATOR_ADD"},
        {TOKEN_OPERATOR_SUBT, "TOKEN_OPERATOR_SUBT"},
        {TOKEN_OPERATOR_MULT, "TOKEN_OPERATOR_MULT"},
        {TOKEN_OPERATOR_DIV, "TOKEN_OPERATOR_DIV"},
        {TOKEN_SEMICOLON, "TOKEN_SEMICOLON"},
        {TOKEN_LEFT_PAREN, "TOKEN_LEFT_PAREN"},
        {TOKEN_RIGHT_PAREN, "TOKEN_RIGHT_PAREN"},
        {TOKEN_KEYWORD_INT, "TOKEN_KEYWORD_INT"},
        {TOKEN_KEYWORD_DOUBLE, "TOKEN_KEYWORD_DOUBLE"},
        {TOKEN_KEYWORD_CHAR, "TOKEN_KEYWORD_CHAR"},
        {TOKEN_KEYWORD_STR, "TOKEN_KEYWORD_STR"},
        {TOKEN_KEYWORD_PRINT, "TOKEN_KEYWORD_PRINT"},
        {TOKEN_IDENTIFIER, "TOKEN_IDENTIFIER"},
        {TOKEN_EOF, "TOKEN_EOF"},
        {TOKEN_OPERATOR_LESSTHAN, "TOKEN_OPERATOR_LESSTHAN"},
        {TOKEN_OPERATOR_GREATERTHAN, "TOKEN_OPERATOR_GREATERTHAN"},
        {TOKEN_RIGHT_CURL, "TOKEN_RIGHT_CURL"},
        {TOKEN_LEFT_CURL, "TOKEN_LEFT_CURL"},
        {TOKEN_KEYWORD_IF, "TOKEN_KEYWORD_IF"},
        {TOKEN_KEYWORD_ELSE, "TOKEN_KEYWORD_ELSE"},
        {TOKEN_OPERATOR_LESS_EQUAL, "TOKEN_OPERATOR_LESS_EQUAL"},
        {TOKEN_OPERATOR_GREATER_EQUAL, "TOKEN_OPERATOR_GREATER_EQUAL"},
        {TOKEN_OPERATOR_EQUALS, "TOKEN_OPERATOR_EQUALS"},
        {TOKEN_OPERATOR_ADD_ASSIGN, "TOKEN_OPERATOR_ADD_ASSIGN"},
        {TOKEN_KEYWORD_FOR, "TOKEN_KEYWORD_FOR"},
        {TOKEN_OPERATOR_INCREMENT, "TOKEN_OPERATOR_INCREMENT"},
        {TOKEN_OPERATOR_DECREMENT, "TOKEN_OPERATOR_DECREMENT"},
        {TOKEN_OPERATOR_NEWLINE, "TOKEN_OPERATOR_NEWLINE"},
        {TOKEN_KEYWORD_BEGIN, "TOKEN_KEYWORD_BEGIN"},
        {TOKEN_SPACESHIP, "TOKEN_SPACESHIP"},
        {TOKEN_KEYWORD_FUNCTION, "TOKEN_KEYWORD_PROC"},
        {TOKEN_COMMA, "TOKEN_COMMA"},
        {TOKEN_OPERATOR_DOESNT_EQUAL, "TOKEN_OPERATOR_DOESNT_EQUAL"},
        {TOKEN_OPERATOR_MODULUS, "TOKEN_OPERATOR_MODULUS"},
        {TOKEN_NL_SYMBOL, "TOKEN_NL_SYMBOL"},
        {TOKEN_KEYWORD_BOOL, "TOKEN_KEYWORD_BOOL"},
        {TOKEN_BOOL_VALUE, "TOKEN_BOOL_VALUE"},
        {TOKEN_KEYWORD_RESULT, "TOKEN_KEYWORD_RESULT"},
        {TOKEN_SINGLELINE_COMMENT, "TOKEN_SINGLELINE_COMMENT"},
        {TOKEN_MULTILINE_COMMENT, "TOKEN_MULTILINE_COMMENT"},
        {TOKEN_KEYWORD_INPUT, "TOKEN_KEYWORD_INPUT"},
        {TOKEN_INPUT_TYPE, "TOKEN_INPUT_TYPE"},
        {TOKEN_KEYWORD_CHECK, "TOKEN_KEYWORD_CHECK"},
        {TOKEN_KEYWORD_ELEMENT, "TOKEN_KEYWORD_ELEMENT"},
        {TOKEN_ARRAY_INITIALIZER, "TOKEN_ARRAY_INITIALIZER"},
        {TOKEN_OPERATOR_ARRAYRANGE, "TOKEN_OPERATOR_ARRAYRANGE"},
        {TOKEN_ARRAY_LENGTH, "TOKEN_ARRAY_LENGTH"},
        {TOKEN_ARRAY_ACCESS, "TOKEN_ARRAY_ACCESS"},
        {TOKEN_ARRAY_LAST_INDEX, "TOKEN_ARRAY_LAST_INDEX"},
        {TOKEN_ARRAY_INSERT, "TOKEN_ARRAY_INSERT"},
        {TOKEN_ARRAY_REMOVE, "TOKEN_ARRAY_REMOVE"},
        {TOKEN_ARRAY_SORT_ASC, "TOKEN_ARRAY_SORT_ASC"},
        {TOKEN_ARRAY_SORT_DESC, "TOKEN_ARRAY_SORT_DESC"},
        {TOKEN_ELEMENT_TYPE, "TOKEN_ELEMENT_TYPE"},
        {TOKEN_KEYWORD_RANGE, "TOKEN_KEYWORD_RANGE"},
        {TOKEN_DOT, "TOKEN_DOT"},
        {TOKEN_KEYWORD_REPEAT, "TOKEN_KEYWORD_REPEAT"}};

    auto it = tokenTypeNames.find(type);
    if (it != tokenTypeNames.end())
    {
        return it->second;
    }
    return "UNKNOWN_TOKEN_TYPE";
}