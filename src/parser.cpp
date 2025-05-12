#include "parser.hpp"
#include "lexer.hpp"
#include "dynamic_array.hpp"
#include <iostream>

/**
 * @brief Initialize the maps used by the parser
 */
void Parser::initializeParserMaps()
{
    // Initialize statement dispatch table
    statementDispatch = {
        {TOKEN_IDENTIFIER, &Parser::parseID},
        {TOKEN_KEYWORD_INT, &Parser::parseKeywordINT},
        {TOKEN_KEYWORD_PRINT, &Parser::parseKeywordPrint},
        {TOKEN_OPERATOR_INCREMENT, &Parser::parseIncrementOperator},
        {TOKEN_OPERATOR_DECREMENT, &Parser::parseDecrementOperator},
        {TOKEN_EOF, &Parser::parseKeywordEOF},
        {TOKEN_EQUALS, &Parser::parseEquals},
        {TOKEN_INTEGER_VAL, &Parser::parseIntegerValue},
        {TOKEN_SEMICOLON, &Parser::parseSemicolon},
        {TOKEN_OPERATOR_ADD, &Parser::parseAdd},
        {TOKEN_LEFT_PAREN, &Parser::parseLeftParen},
        {TOKEN_KEYWORD_FOR, &Parser::parseKeywordFor},
        {TOKEN_RIGHT_PAREN, &Parser::parseRightParen},
        {TOKEN_CHAR_VAL, &Parser::parseCharValue},
        {TOKEN_KEYWORD_CHAR, &Parser::parseKeywordChar},
        {TOKEN_DOUBLE_VAL, &Parser::parseDoubleValue},
        {TOKEN_KEYWORD_DOUBLE, &Parser::parseKeywordDouble},
        {TOKEN_KEYWORD_STR, &Parser::parseStringValue},
        {TOKEN_STRING_VAL, &Parser::parseStringValue},
        {TOKEN_LEFT_CURL, &Parser::parseLeftCurl},
        {TOKEN_RIGHT_CURL, &Parser::parseRightCurl},
        {TOKEN_KEYWORD_IF, &Parser::parseKeywordIf},
        {TOKEN_KEYWORD_CHECK, &Parser::parseKeywordCheck},
        {TOKEN_OPERATOR_DIV, &Parser::parseDivi},
        {TOKEN_OPERATOR_MODULUS, &Parser::parseModulus},
        {TOKEN_OPERATOR_MULT, &Parser::parseMult},
        {TOKEN_NL_SYMBOL, &Parser::parseNewLineCharacter},
        {TOKEN_KEYWORD_BOOL, &Parser::parseKeywordBool},
        {TOKEN_BOOL_VALUE, &Parser::parseBoolValue},
        {TOKEN_KEYWORD_RESULT, &Parser::parseResultStatement},
        {TOKEN_KEYWORD_BEGIN, &Parser::parseBeginBlock},
        {TOKEN_KEYWORD_FUNCTION, &Parser::parseFunctionDecleration},
        {TOKEN_KEYWORD_INPUT, &Parser::parseKeywordInput},
        {TOKEN_KEYWORD_ELEMENT, &Parser::parseKeywordElement},
        {TOKEN_KEYWORD_RANGE, &Parser::parseKeywordRange},
        {TOKEN_KEYWORD_REPEAT, &Parser::parseKeywordRepeat},
        {TOKEN_KEYWORD_ELEMENT, &Parser::parseArrayDeclaration},
        {TOKEN_ARRAY_INITIALIZER, &Parser::parseArrayInit},
        {TOKEN_ARRAY_LENGTH, &Parser::parseArrayLength},
        {TOKEN_ARRAY_INSERT, &Parser::parseArrayInsert},
        {TOKEN_ARRAY_REMOVE, &Parser::parseArrayRemove},
        {TOKEN_ARRAY_SORT_ASC, &Parser::parseArraySortAsc},
        {TOKEN_ARRAY_SORT_DESC, &Parser::parseArraySortDesc},
        {TOKEN_DOT, &Parser::parseDot},
        {TOKEN_SINGLELINE_COMMENT, &Parser::parseSingleLineComment},
        {TOKEN_MULTILINE_COMMENT, &Parser::parseMultiLineComment}};

    // Initialize expression dispatch table
    expressionDispatch = {
        {TOKEN_INTEGER_VAL, &Parser::parseIntegerValue},
        {TOKEN_IDENTIFIER, &Parser::parseID},
        {TOKEN_LEFT_PAREN, &Parser::parseLeftParen},
        {TOKEN_CHAR_VAL, &Parser::parseCharValue},
        {TOKEN_STRING_VAL, &Parser::parseStringValue},
        {TOKEN_DOUBLE_VAL, &Parser::parseDoubleValue},
        {TOKEN_BOOL_VALUE, &Parser::parseBoolValue},
        {TOKEN_OPERATOR_DECREMENT, &Parser::parseDecrementOperator},
        {TOKEN_OPERATOR_INCREMENT, &Parser::parseIncrementOperator},
        {TOKEN_OPERATOR_NEWLINE, &Parser::parseNewLine},
        {TOKEN_NL_SYMBOL, &Parser::parseNewLineCharacter},
        {TOKEN_OPERATOR_SUBT, &Parser::parseSubt},
        {TOKEN_KEYWORD_RANGE, &Parser::parseKeywordRange},
        {TOKEN_KEYWORD_REPEAT, &Parser::parseKeywordRepeat},
        {TOKEN_ARRAY_LENGTH, &Parser::parseArrayLength},
        {TOKEN_ARRAY_LAST_INDEX, &Parser::parseArrayLastIndex},
    };

    // Initialize operator precedence map (higher number = higher precedence)
    operatorPrecedence = {
        {TOKEN_OPERATOR_MULT, 7},
        {TOKEN_OPERATOR_DIV, 7},
        {TOKEN_OPERATOR_MODULUS, 7},
        {TOKEN_OPERATOR_ADD, 6},
        {TOKEN_OPERATOR_SUBT, 6},
        {TOKEN_OPERATOR_LESSTHAN, 5},
        {TOKEN_OPERATOR_GREATERTHAN, 5},
        {TOKEN_OPERATOR_LESS_EQUAL, 5},
        {TOKEN_OPERATOR_GREATER_EQUAL, 5},
        {TOKEN_OPERATOR_EQUALS, 4},
        {TOKEN_OPERATOR_DOESNT_EQUAL, 4},
        {TOKEN_EQUALS, 3},             // Assignment has lower precedence
        {TOKEN_OPERATOR_INCREMENT, 8}, // Unary operators have highest precedence
        {TOKEN_OPERATOR_DECREMENT, 8},
        {TOKEN_DOT, 9},                // Dot operator (for method calls) has even higher precedence
        {TOKEN_ARRAY_LENGTH, 9},       // Array length operator has high precedence
        {TOKEN_ARRAY_INITIALIZER, 2},  // Array initialization has low precedence
        {TOKEN_OPERATOR_ARRAYRANGE, 2} // Array range operator has low precedence
    };

    // Initialize token to node type mapping
    tokenToNodeType = {
        // Operators
        {TOKEN_OPERATOR_ADD, NODE_ADD},
        {TOKEN_OPERATOR_SUBT, NODE_SUBT},
        {TOKEN_OPERATOR_MULT, NODE_MULT},
        {TOKEN_OPERATOR_DIV, NODE_DIVISION},
        {TOKEN_OPERATOR_MODULUS, NODE_MODULUS},
        {TOKEN_OPERATOR_LESSTHAN, NODE_LESS_THAN},
        {TOKEN_OPERATOR_GREATERTHAN, NODE_GREATER_THAN},
        {TOKEN_OPERATOR_LESS_EQUAL, NODE_LESS_EQUAL},
        {TOKEN_OPERATOR_DOESNT_EQUAL, NODE_NOT_EQUAL},
        {TOKEN_EQUALS, NODE_EQUALS},
        {TOKEN_OPERATOR_INCREMENT, NODE_OPERATOR_INCREMENT},
        {TOKEN_OPERATOR_DECREMENT, NODE_OPERATOR_DECREMENT},
        {TOKEN_OPERATOR_NEWLINE, NODE_NEWLINE},
        {TOKEN_NL_SYMBOL, NODE_NEWLINE_SYMBOL},

        // Literals
        {TOKEN_INTEGER_VAL, NODE_INT_LITERAL},
        {TOKEN_DOUBLE_VAL, NODE_DOUBLE_LITERAL},
        {TOKEN_CHAR_VAL, NODE_CHAR_LITERAL},
        {TOKEN_STRING_VAL, NODE_STRING_LITERAL},
        {TOKEN_BOOL_VALUE, NODE_BOOL_LITERAL},

        // Type declarations
        {TOKEN_KEYWORD_INT, NODE_INT},
        {TOKEN_KEYWORD_DOUBLE, NODE_DOUBLE},
        {TOKEN_KEYWORD_CHAR, NODE_CHAR},
        {TOKEN_KEYWORD_STR, NODE_STRING},
        {TOKEN_KEYWORD_BOOL, NODE_BOOL},

        // Keywords and structures
        {TOKEN_KEYWORD_PRINT, NODE_PRINT},
        {TOKEN_LEFT_PAREN, NODE_LEFT_PAREN},
        {TOKEN_RIGHT_PAREN, NODE_RIGHT_PAREN},
        {TOKEN_LEFT_CURL, NODE_LEFT_CURL},
        {TOKEN_RIGHT_CURL, NODE_RIGHT_CURL},
        {TOKEN_SEMICOLON, NODE_SEMICOLON},
        {TOKEN_KEYWORD_IF, NODE_IF},
        {TOKEN_KEYWORD_FOR, NODE_FOR},
        {TOKEN_KEYWORD_CHECK, NODE_CHECK},
        {TOKEN_EOF, NODE_EOF},
        {TOKEN_KEYWORD_BEGIN, NODE_BEGIN_BLOCK},
        {TOKEN_KEYWORD_FUNCTION, NODE_FUNCTION_DECLERATION},
        {TOKEN_KEYWORD_RESULT, NODE_RESULTSTATEMENT},
        {TOKEN_KEYWORD_INPUT, NODE_KEYWORD_INPUT},

    };
}

/**
 * @brief Generic Parsing method that uses a dispatch table
 * @param dispatchTable map of token types to parsing functions
 * @return AST node resulting from the appropriate parsing function
 */
AST_NODE *
Parser::parseByTokenType(const std::unordered_map<tokenType, ParseFunction> &dispatchTable)
{
    if (current == nullptr)
    {
        std::cerr << "Unexpected end of file";
        exit(1);
    }

    auto it = dispatchTable.find(current->TYPE);
    if (it != dispatchTable.end())
    {
        return (this->*(it->second))();
    }
    else
    {
        std::cerr << "Unexpected Token: " + getTokenTypeName(current->TYPE);
        exit(1);
        return nullptr;
    }
}

AST_NODE *Parser::parseSingleLineComment()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_COMMENT;
    node->VALUE = current->value;
    advanceCursor();
    return node;
}

AST_NODE *Parser::parseMultiLineComment()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_COMMENT;
    node->VALUE = current->value;
    advanceCursor();
    return node;
}

/**
 * @brief Advances to the next token if the current token matches the expected type
 * @param type Expected token type
 * @return Pointer to the next token or nullptr if at the end
 * @throws Exits with error if token type doesn't match or unexpected EOF
 *
 * Performs validation that the current token is of the expected type before
 * advancing the cursor. Provides detailed error information on mismatch.
 */
Token *Parser::proceed(enum tokenType type)
{
    // std::cout << "About to proceed: Expected " << getTokenTypeName(type)
    //           << ", Got " << getTokenTypeName(tokens[cursor]->TYPE)
    //           << ", Value: " << tokens[cursor]->value << std::endl;

    // Check for end of file
    if (cursor >= size)
    {
        std::cerr << "< Syntax Error > Unexpected end of file" << std::endl;
        exit(1);
    }

    // Validate token type
    if (tokens[cursor]->TYPE != type)
    {
        std::cerr << "< Syntax Error > Expected " << getTokenTypeName(type)
                  << " but got " << getTokenTypeName(tokens[cursor]->TYPE) << std::endl;
        exit(1);
    }

    // Advance cursor
    cursor++;

    // Update current token pointer
    if (cursor < size)
    {
        current = tokens[cursor];
    }
    else
    {
        current = nullptr;
    }

    return current;
}

/**
 * @brief Returns the current token being processed
 * @return Pointer to the current token
 */
Token *Parser::getCurrentToken()
{
    return current;
}

/**
 * @brief Advances the cursor to the next token
 *
 * Updates the current token pointer after advancing the cursor.
 */
void Parser::advanceCursor()
{
    if (cursor < size)
    {
        cursor++;
        current = tokens[cursor];

        while (current && (current->TYPE == TOKEN_SINGLELINE_COMMENT ||
                           current->TYPE == TOKEN_MULTILINE_COMMENT))
        {
            if (cursor < size)
            {
                cursor++;
                current = tokens[cursor];
            }
            else
            {
                current = nullptr;
                break;
            }
        }
    }
    else
    {
        current = nullptr;
    }
}

/**
 * @brief Looks ahead at the next token without advancing the cursor
 * @return Pointer to the next token, or nullptr if at the end
 *
 * Provides a way to examine the next token without consuming it.
 */
Token *Parser::peakAhead()
{
    // Check if we can look ahead
    if (cursor + 1 < size)
    {
        return tokens[cursor + 1];
    }
    else
    {
        return nullptr;
    }
}

//-------------------------------------------------------------------
// Value parsing methods
//-------------------------------------------------------------------

/**
 * @brief Parses a floating-point (double) literal value
 * @return AST node representing the double literal
 *
 * Creates a NODE_DOUBLE_LITERAL node with the value from the token.
 */
AST_NODE *Parser::parseDoubleValue()
{
    std::string doubleValue = current->value;
    proceed(TOKEN_DOUBLE_VAL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_DOUBLE_LITERAL;
    node->VALUE = doubleValue;

    return node;
}

/**
 * @brief Parses a double variable declaration and optional initialization
 * @return AST node representing the double variable
 *
 * Handles both simple declarations (double x;) and
 * declarations with initialization (double x = expression;).
 */
AST_NODE *Parser::parseKeywordDouble()
{
    proceed(TOKEN_KEYWORD_DOUBLE);

    std::string identifierName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_DOUBLE;
    node->VALUE = identifierName;

    // Check for initialization with equals sign
    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);
        node->CHILD = parseExpression();
    }
    else
    {
        node->CHILD = nullptr;
    }

    return node;
}

/**
 * @brief Parses a string literal or string variable declaration
 * @return AST node representing the string
 * @throws Exits with error on unexpected token
 *
 * Handles both string literals and string variable declarations.
 */
AST_NODE *Parser::parseStringValue()
{
    AST_NODE *node = new AST_NODE();

    if (current->TYPE == TOKEN_STRING_VAL)
    {
        // Handle string literal
        node->TYPE = NODE_STRING_LITERAL;
        node->VALUE = current->value;
        proceed(TOKEN_STRING_VAL);
    }
    else if (current->TYPE == TOKEN_KEYWORD_STR)
    {
        // Handle string variable declaration
        proceed(TOKEN_KEYWORD_STR);

        std::string variableName = current->value;
        proceed(TOKEN_IDENTIFIER);
        node->TYPE = NODE_STRING;
        node->VALUE = variableName;

        // Check for initialization
        if (current != nullptr && current->TYPE == TOKEN_EQUALS)
        {
            proceed(TOKEN_EQUALS);
            node->CHILD = parseExpression();
        }
    }
    else
    {
        std::cerr << "Unexpected token in string expression" << std::endl;
        exit(1);
    }
    return node;
}

/**
 * @brief Parses a character variable declaration
 * @return AST node representing the character variable
 *
 * Handles both simple declarations (char c;) and
 * declarations with initialization (char c = 'x';).
 */
AST_NODE *Parser::parseKeywordChar()
{
    proceed(TOKEN_KEYWORD_CHAR);
    std::string identifierName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_CHAR;
    node->VALUE = identifierName;

    // Check for initialization
    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);
        node->CHILD = parseExpression();
    }
    else
    {
        node->CHILD = nullptr;
    }

    return node;
}

/**
 * @brief Parses a character literal value
 * @return AST node representing the character literal
 *
 * Creates a NODE_CHAR_LITERAL node with the value from the token.
 */
AST_NODE *Parser::parseCharValue()
{
    std::string charValue = current->value;
    proceed(TOKEN_CHAR_VAL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_CHAR_LITERAL;
    node->VALUE = charValue;

    return node;
}

/**
 * @brief Parses a bool literal value
 * @return AST Node representing the integer value
 */
AST_NODE *Parser::parseBoolValue()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_BOOL_LITERAL;
    node->VALUE = current->value;

    proceed(TOKEN_BOOL_VALUE);
    return node;
}

/**
 * @brief Parses an integer literal value
 * @return AST node representing the integer literal
 *
 * Creates a NODE_INT_LITERAL node with the value from the token.
 */
AST_NODE *Parser::parseIntegerValue()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INT_LITERAL;
    node->VALUE = current->value;

    proceed(TOKEN_INTEGER_VAL);

    return node;
}

/**
 * @brief Parses an integer variable declaration
 * @return AST node representing the integer variable
 *
 * Handles both simple declarations (int x;) and
 * declarations with initialization (int x = expression;).
 */
AST_NODE *Parser::parseKeywordINT()
{
    proceed(TOKEN_KEYWORD_INT);

    std::string variableName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INT;
    node->VALUE = variableName;

    // Check for initialization
    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);
        node->CHILD = parseExpression();
    }
    else
    {
        node->CHILD = nullptr;
    }
    return node;
}

/**
 * @brief Parses a newline character
 * @return AST node representing the newline
 */
AST_NODE *Parser::parseNewLine()
{
    proceed(TOKEN_OPERATOR_NEWLINE);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_NEWLINE;

    return node;
}
//-------------------------------------------------------------------
// Array Parsing Methods
//-------------------------------------------------------------------

AST_NODE *Parser::parseArrayDeclaration()
{
    proceed(TOKEN_KEYWORD_ELEMENT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_DECLARATION;

    if (current->TYPE != TOKEN_ELEMENT_TYPE)
    {
        std::cerr << "< Syntax Error > Expected element type after 'elements'" << std::endl;
        exit(1);
    }

    std::string elementType = current->value;
    proceed(TOKEN_ELEMENT_TYPE);

    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array name after type." << std::endl;
        exit(1);
    }

    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);
    node->VALUE = arrayName;

    AST_NODE *typeNode = new AST_NODE();
    typeNode->TYPE = NODE_ELEMENT_TYPE;
    typeNode->VALUE = elementType;
    node->CHILD = typeNode;

    return node;
}

AST_NODE *Parser::parseArrayInit()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_INIT;
    node->VALUE = current->value;

    // skip name of array
    proceed(TOKEN_IDENTIFIER);

    if (current->TYPE != TOKEN_ARRAY_INITIALIZER)
    {
        std::cerr << "< Syntax Error > Expected '|=' following array identifier" << std::endl;
        exit(1);
    }

    proceed(TOKEN_ARRAY_INITIALIZER);

    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' to being initializing array" << std::endl;
        exit(1);
    }

    proceed(TOKEN_LEFT_PAREN);

    node->SUB_STATEMENTS.push_back(parseExpression());

    while (current->TYPE == TOKEN_COMMA)
    {
        proceed(TOKEN_COMMA);
        node->SUB_STATEMENTS.push_back(parseExpression());
    }

    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected closing parenthesis after array initialization" << std::endl;
        exit(1);
    }

    proceed(TOKEN_RIGHT_PAREN);
    return node;
}
AST_NODE *Parser::parseArrayRange()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_RANGE;
    node->VALUE = current->value;

    proceed(TOKEN_IDENTIFIER);

    if (current->TYPE != TOKEN_EQUALS)
    {
        std::cerr << "< Syntax Error > Expected '=' to initialize the array with a range" << std::endl;
        exit(1);
    }

    proceed(TOKEN_EQUALS);

    if (current->TYPE != TOKEN_KEYWORD_RANGE)
    {
        std::cerr << "< Syntax Error > Expected keyword range." << std::endl;
        exit(1);
    }

    // Directly parse the range expression (including parentheses)
    node->CHILD = parseKeywordRange();

    return node;
}
AST_NODE *Parser::parseArrayRepeat()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_REPEAT;
    node->VALUE = current->value;

    proceed(TOKEN_IDENTIFIER);

    if (current->TYPE != TOKEN_EQUALS)
    {
        std::cerr << "< Syntax Error > Expected '=' to initialize the array with a range" << std::endl;
        exit(1);
    }

    proceed(TOKEN_ARRAY_INITIALIZER);

    if (current->TYPE != TOKEN_KEYWORD_REPEAT)
    {
        std::cerr << "< Syntax Error > Expected keyword repeat.";
        exit(1);
    }

    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *numToRepeat = parseExpression();
    node->CHILD = numToRepeat;

    if (current->TYPE != TOKEN_COMMA)
    {
        std::cerr << "< Syntax Error > Expected ',' seperating repeat value and amount." << std::endl;
        exit(1);
    }

    proceed(TOKEN_COMMA);

    AST_NODE *counter = parseExpression();
    node->SUB_STATEMENTS.push_back(counter);

    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected closing parenthesis ')'" << std::endl;
        exit(1);
    }

    proceed(TOKEN_RIGHT_PAREN);

    return node;
}
AST_NODE *Parser::parseArrayLength()
{
    // Expect array length operator (#)
    if (current->TYPE != TOKEN_ARRAY_LENGTH)
    {
        std::cerr << "< Syntax Error > Expected '#' for array length" << std::endl;
        exit(1);
    }
    proceed(TOKEN_ARRAY_LENGTH);

    // Parse the array name
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array identifier after '#'" << std::endl;
        exit(1);
    }
    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Create and populate the node
    AST_NODE *arrayLength = new AST_NODE();
    arrayLength->TYPE = NODE_ARRAY_LENGTH;
    arrayLength->VALUE = arrayName; // The array being measured

    return arrayLength;
}
AST_NODE *Parser::parseArrayInsert()
{
    // Parse the insertion operator (+>)
    proceed(TOKEN_ARRAY_INSERT);

    // Check for array identifier
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array identifier after '+>'" << std::endl;
        exit(1);
    }
    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Check for opening parenthesis
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' after array identifier" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    // Create the node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_INSERT;
    node->VALUE = arrayName;

    // Parse the index expression
    AST_NODE *indexNode = parseExpression();
    node->CHILD = indexNode;

    // Check for comma
    if (current->TYPE != TOKEN_COMMA)
    {
        std::cerr << "< Syntax Error > Expected ',' after index in array insert" << std::endl;
        exit(1);
    }
    proceed(TOKEN_COMMA);

    // Parse the value expression
    AST_NODE *valueNode = parseExpression();
    node->SUB_STATEMENTS.push_back(valueNode);

    // Check for closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected ')' after value in array insert" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    return node;
}
AST_NODE *Parser::parseArrayRemove()
{
    // Parse the removal operator (-<)
    proceed(TOKEN_ARRAY_REMOVE);

    // Check for array identifier
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array identifier after '-<'" << std::endl;
        exit(1);
    }
    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Check for opening parenthesis
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' after array identifier" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    // Create the node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_REMOVE;
    node->VALUE = arrayName;

    // Parse the index expression
    AST_NODE *indexNode = parseExpression();
    node->CHILD = indexNode;

    // Check for closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected ')' after index in array remove" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    return node;
}
AST_NODE *Parser::parseArraySortAsc()
{ // Parse the ascending sort operator (~>)
    proceed(TOKEN_ARRAY_SORT_ASC);

    // Check for array identifier
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array identifier after '~>'" << std::endl;
        exit(1);
    }
    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Create the node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_SORT_ASC;
    node->VALUE = arrayName;

    return node;
}
AST_NODE *Parser::parseArraySortDesc()
{ // Parse the descending sort operator (<~)
    proceed(TOKEN_ARRAY_SORT_DESC);

    // Check for array identifier
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array identifier after '<~'" << std::endl;
        exit(1);
    }
    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Create the node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_SORT_DESC;
    node->VALUE = arrayName;

    return node;
}

AST_NODE *Parser::parseArrayLastIndex()
{
    // Consume the token
    proceed(TOKEN_ARRAY_LAST_INDEX);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_LAST_INDEX;
    node->VALUE = current->value; // or set as needed

    return node;
}
AST_NODE *Parser::parseKeywordElement()
{
    // Parse the 'elements' keyword
    proceed(TOKEN_KEYWORD_ELEMENT);

    // Check for element type
    if (current->TYPE != TOKEN_ELEMENT_TYPE)
    {
        std::cerr << "< Syntax Error > Expected element type after 'elements'" << std::endl;
        exit(1);
    }

    std::string elementType = current->value;
    proceed(TOKEN_ELEMENT_TYPE);

    // Check for array identifier
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected array identifier after element type" << std::endl;
        exit(1);
    }

    std::string arrayName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Create the array declaration node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_DECLARATION;
    node->VALUE = arrayName;

    // Create and attach the element type node
    AST_NODE *typeNode = new AST_NODE();
    typeNode->TYPE = NODE_ELEMENT_TYPE;
    typeNode->VALUE = elementType;
    node->CHILD = typeNode;

    return node;
}

AST_NODE *Parser::parseKeywordRange()
{
    // Parse the 'range' keyword
    proceed(TOKEN_KEYWORD_RANGE);

    // Check for opening parenthesis
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' after 'range'" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    // Create the range node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_RANGE;

    // Parse the start integer
    if (current->TYPE != TOKEN_INTEGER_VAL)
    {
        std::cerr << "< Syntax Error > Expected integer at start of range" << std::endl;
        exit(1);
    }
    AST_NODE *startNode = new AST_NODE();
    startNode->TYPE = NODE_INT_LITERAL;
    startNode->VALUE = current->value;
    proceed(TOKEN_INTEGER_VAL);
    node->CHILD = startNode;

    // Check for range operator
    if (current->TYPE != TOKEN_OPERATOR_ARRAYRANGE)
    {
        std::cerr << "< Syntax Error > Expected '..' in range expression" << std::endl;
        exit(1);
    }
    proceed(TOKEN_OPERATOR_ARRAYRANGE);

    // Parse the end integer
    if (current->TYPE != TOKEN_INTEGER_VAL)
    {
        std::cerr << "< Syntax Error > Expected integer at end of range" << std::endl;
        exit(1);
    }
    AST_NODE *endNode = new AST_NODE();
    endNode->TYPE = NODE_INT_LITERAL;
    endNode->VALUE = current->value;
    proceed(TOKEN_INTEGER_VAL);
    node->SUB_STATEMENTS.push_back(endNode);

    // Check for closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected ')' after range expression" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    return node;
}

AST_NODE *Parser::parseKeywordRepeat()
{
    // Parse the 'repeat' keyword
    proceed(TOKEN_KEYWORD_REPEAT);

    // Check for opening parenthesis
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' after 'repeat'" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    // Create the repeat node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ARRAY_REPEAT;

    // Parse the value to repeat
    AST_NODE *valueNode = parseExpression();
    node->CHILD = valueNode;

    // Check for comma
    if (current->TYPE != TOKEN_COMMA)
    {
        std::cerr << "< Syntax Error > Expected ',' after value in repeat expression" << std::endl;
        exit(1);
    }
    proceed(TOKEN_COMMA);

    // Parse the count expression
    AST_NODE *countNode = parseExpression();
    node->SUB_STATEMENTS.push_back(countNode);

    // Check for closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected ')' after count in repeat expression" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    return node;
}

AST_NODE *Parser::parseDot()
{
}

//-------------------------------------------------------------------
// Keyword and special token parsing methods
//-------------------------------------------------------------------
/**
 * @brief Parses bool decleration
 * @return AST Node representing bool
 * @throws exits with error if ...
 */

AST_NODE *Parser::parseKeywordBool()
{
    proceed(TOKEN_KEYWORD_BOOL);

    std::string variableName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_BOOL;
    node->VALUE = variableName;

    // Check for initialization
    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);
        node->CHILD = parseExpression();
    }
    else
    {
        node->CHILD = nullptr;
    }
    return node;
}

/**
 * @brief Parses the end-of-file marker
 * @return AST node representing the EOF
 * @throws Exits with error if tokens exist after EOF
 *
 * Validates that no tokens exist after the EOF token.
 */
AST_NODE *Parser::parseKeywordEOF()
{
    std::string eofValue = current->value;
    proceed(TOKEN_EOF);

    // Check if there are tokens after EOF
    if (cursor < size)
    {
        std::cerr << "Unexpected token after EOF: "
                  << getTokenTypeName(tokens[cursor]->TYPE) << std::endl;
        exit(1);
    }

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_EOF;
    node->VALUE = eofValue;

    return node;
}

/**
 * @brief Parses a print statement
 * @return AST node representing the print operation
 * @throws Exits with error on syntax issues
 *
 * Handles print statements in the form: out_to_console(expression)
 */
AST_NODE *Parser::parseKeywordPrint()
{
    proceed(TOKEN_KEYWORD_PRINT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_PRINT;

    // Check for opening parenthesis
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "Expected '(' after print keyword" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    // Parse the expression to print
    node->CHILD = parseExpression();

    // Check for closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "Expected ')' after print argument" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    return node;
}
//======================================================
//================  PRINT STATEMENTS  =================
//======================================================
/**
 * @brief Parses the input keyword
 * @return AST node representing the input statement
 */
AST_NODE *Parser::parseKeywordInput()
{
    // Create a node for input keyword
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_KEYWORD_INPUT;
    node->VALUE = "input";

    proceed(TOKEN_KEYWORD_INPUT);

    // Your lexer gives us TOKEN_INPUT_TYPE directly
    if (current->TYPE != TOKEN_INPUT_TYPE)
    {
        std::cerr << "< Syntax Error > Expected input type after 'input' keyword" << std::endl;
        exit(1);
    }

    // Parse the input type (already parsed by lexer as TOKEN_INPUT_TYPE)
    AST_NODE *inputType = new AST_NODE();
    inputType->TYPE = NODE_INPUT_TYPE;
    inputType->VALUE = current->value;
    node->CHILD = inputType;

    proceed(TOKEN_INPUT_TYPE);

    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' following input." << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *promptNode = new AST_NODE();
    promptNode->TYPE = NODE_INPUT_PROMPT;
    promptNode->CHILD = parseExpression();

    node->SUB_STATEMENTS.push_back(promptNode);

    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected ')' follwing the prompt" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);
    // Expect spaceship operator (=>)
    if (current->TYPE != TOKEN_SPACESHIP)
    {
        std::cerr << "< Syntax Error > Expected '=>' after input prompt" << std::endl;
        exit(1);
    }
    proceed(TOKEN_SPACESHIP);

    // Expect variable name
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected variable name after '=>'" << std::endl;
        exit(1);
    }

    // Create a variable node and add it to sub-statements
    AST_NODE *varNode = new AST_NODE();
    varNode->TYPE = NODE_IDENTIFIER;
    varNode->VALUE = current->value;

    promptNode->SUB_STATEMENTS.push_back(varNode);

    proceed(TOKEN_IDENTIFIER);

    return node;
}

/**
 * @brief parses the input type specification
 * @return AST node representing input type
 * @throws exit with error on invalid input type
 */

AST_NODE *Parser::parseInputType()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INPUT_TYPE;

    // Extract type from token value (which would be like "<int>")
    std::string typeWithBrackets = current->value;

    // Remove brackets to get just the type name
    std::string typeName = typeWithBrackets.substr(1, typeWithBrackets.length() - 2);

    // Validate the extracted type
    if (typeName == "int" ||
        typeName == "double" ||
        typeName == "string" || typeName == "str" ||
        typeName == "bool" ||
        typeName == "char")
    {

        node->VALUE = typeName;
    }
    else
    {
        std::cerr << "< Syntax Error > Invalid input type: " << typeName << std::endl;
        exit(1);
    }

    proceed(TOKEN_INPUT_TYPE);

    return node;
}
//======================================================
//================  RETURN STATEMENTS  =================
//======================================================

/**
 * @brief Parses a result/return statement
 * @return AST node representing the return statement
 * @throws Exits with error on syntax issues
 *
 * result => {statement};
 *
 * Handles return statements in the form: result => {expression}
 * Creates a node that will later be used during execution to
 * return a value from the function call.
 */
AST_NODE *Parser::parseResultStatement()
{
    // Debug output to track parsing
    std::cout << "Parsing result statement" << std::endl;

    AST_NODE *resultStatement = new AST_NODE();
    resultStatement->TYPE = NODE_RESULTSTATEMENT;
    proceed(TOKEN_KEYWORD_RESULT);

    if (current->TYPE != TOKEN_SPACESHIP)
    {
        std::cerr << "< Syntax Error > Expected '=>' after result keyword." << std::endl;
        exit(1);
    }
    proceed(TOKEN_SPACESHIP);

    if (current->TYPE != TOKEN_LEFT_CURL)
    {
        std::cerr << "< Syntax Error > Expected '{' following the '=>' in result statement." << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_CURL);

    // Use parseExpression to handle any valid expression inside the braces
    // This works for simple values like "area" and complex expressions like "x + 3" or "-1"
    resultStatement->CHILD = parseExpression();

    // Debug output to see what token we're at after parsing the expression
    std::cout << "After parsing result expression, current token: "
              << getTokenTypeName(current->TYPE) << std::endl;

    if (current->TYPE != TOKEN_RIGHT_CURL)
    {
        std::cerr << "< Syntax Error > Expected '}' to close result statement." << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_CURL);

    return resultStatement;
}

/**
 * @brief Parses the 'result' keyword
 * @return AST node representing the result keyword
 *
 * Creates a NODE_RESULT node that indicates the start of a
 * return statement in the language syntax.
 */
AST_NODE *Parser::parseKeywordResult()
{
    AST_NODE *keywordResult = new AST_NODE();
    keywordResult->TYPE = NODE_RESULT;
    keywordResult->VALUE = current->value;

    return keywordResult;
}

/**
 * @brief Parses the expression inside a result statement's curly braces
 * @return AST node representing the expression to be returned
 * @throws Exits with error on syntax issues
 *
 * Processes the expression between the curly braces in a
 * 'result => {expression}' syntax, which will be the value
 * returned by the function.
 */
AST_NODE *Parser::parseResultExpression()
{
    proceed(TOKEN_LEFT_CURL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_RESULT_EXPRESSION;

    node->CHILD = parseExpression();

    // Expect and Consume the closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_CURL)
    {
        std::cerr << "< Syntax Error > Expected closing curly brace" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_CURL);

    return node;
}
/**
 * @brief Parses an equals sign (assignment operator)
 * @return AST node representing the equals sign
 */
AST_NODE *Parser::parseEquals()
{
    proceed(TOKEN_EQUALS);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_EQUALS;
    node->VALUE = current->value;

    return node;
}

/**
 * @brief Parses a semicolon
 * @return AST node representing the semicolon
 */
AST_NODE *Parser::parseSemicolon()
{
    proceed(TOKEN_SEMICOLON);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_SEMICOLON;

    return node;
}

/**
 * @brief Parses an identifier, which could be a variable or function
 * @return AST node representing the identifier
 * @throws Exits with error on syntax issues
 *
 * Handles variables, function calls, and special identifiers.
 */
AST_NODE *Parser::parseID()
{
    std::string identifierName = current->value;
    proceed(TOKEN_IDENTIFIER);

    if (current && current->TYPE == TOKEN_DOT)
    {
        return parseDot();
    }

    if (current && current->TYPE == TOKEN_ARRAY_INITIALIZER)
    {
        cursor--; // step back so parseArrayInit sees the identifier
        current = tokens[cursor];
        return parseArrayInit(); // consumes IDENT, '|=', and the list
    }

    if (current && current->TYPE == TOKEN_ARRAY_ACCESS)
    {
        proceed(TOKEN_ARRAY_ACCESS);
        if (current->TYPE != TOKEN_LEFT_PAREN)
        {
            std::cerr << "< Syntax Error > Expected '(' after '@'" << std::endl;
            exit(1);
        }
        proceed(TOKEN_LEFT_PAREN);
        AST_NODE *node = new AST_NODE();
        node->TYPE = NODE_ARRAY_ACCESS;
        node->VALUE = identifierName;

        // index or last-element
        if (current->TYPE == TOKEN_ARRAY_LAST_INDEX)
        {
            proceed(TOKEN_ARRAY_LAST_INDEX);
            AST_NODE *lastNode = new AST_NODE();
            lastNode->TYPE = NODE_ARRAY_LAST_INDEX;
            node->CHILD = lastNode;
        }
        else
        {
            node->CHILD = parseExpression();
        }

        if (current->TYPE != TOKEN_RIGHT_PAREN)
        {
            std::cerr << "< Syntax Error > Expected ')' after array access" << std::endl;
            exit(1);
        }
        proceed(TOKEN_RIGHT_PAREN);
        return node;
    }

    if (current && current->TYPE == TOKEN_LEFT_PAREN)
    {
        AST_NODE *call = new AST_NODE();
        call->TYPE = NODE_FUNCTION_CALL;
        call->VALUE = identifierName;
        proceed(TOKEN_LEFT_PAREN);

        while (current && current->TYPE != TOKEN_RIGHT_PAREN)
        {
            call->SUB_STATEMENTS.push_back(parseExpression());
            if (current->TYPE == TOKEN_COMMA)
                proceed(TOKEN_COMMA);
            else if (current->TYPE != TOKEN_RIGHT_PAREN)
            {
                std::cerr << "< Syntax Error > Expected ',' or ')'" << std::endl;
                exit(1);
            }
        }
        proceed(TOKEN_RIGHT_PAREN);
        return call;
    }

    if (identifierName == "end")
    {
        return parseKeywordEOF();
    }

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_IDENTIFIER;
    node->VALUE = identifierName;

    if (current && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);
        node->CHILD = parseExpression();
    }

    return node;
}
/**
 * @brief parses a newline character '...'
 * @return AST_NODE creating a newline
 * @throws Exits with error on syntax issues
 *
 * Handles the call of a newline character
 */

AST_NODE *Parser::parseNewLineCharacter()
{
    proceed(TOKEN_NL_SYMBOL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_NEWLINE_SYMBOL;

    return node;
}

//-------------------------------------------------------------------
// Expression and operator parsing methods
//-------------------------------------------------------------------

/**
 * @brief Parses a left parenthesis and its contents
 * @return AST node representing the parenthesized expression
 * @throws Exits with error on syntax issues
 *
 * Handles expressions enclosed in parentheses.
 */
AST_NODE *Parser::parseLeftParen()
{
    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_PAREN_EXPR;

    node->CHILD = parseExpression();

    // Expect and Consume the closing parenthesis
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected closing parenthesis" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    return node;
}

/**
 * @brief Handles unexpected right parenthesis
 * @return Always exits with error
 * @throws Exits with error
 *
 * Called when a right parenthesis is found without a matching left parenthesis.
 */
AST_NODE *Parser::parseRightParen()
{
    // This is handled in left paren
    // if we make it here something is wrong
    std::cerr << "< Syntax Error > Unexpected right parenthesis." << std::endl;
    exit(1);

    return nullptr;
}

/**
 * @brief Parses an addition operator
 * @return AST node representing the addition
 */
AST_NODE *Parser::parseAdd()
{
    proceed(TOKEN_OPERATOR_ADD);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ADD;

    return node;
}

/**
 * @brief Parses a multiplication operator
 * @return AST node representing the multiplication
 */
AST_NODE *Parser::parseMult()
{
    proceed(TOKEN_OPERATOR_MULT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_MULT;

    return node;
}

/**
 * @brief Parses a subtraction operator
 * @return AST node representing the subtraction
 */
AST_NODE *Parser::parseSubt()
{
    proceed(TOKEN_OPERATOR_SUBT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_SUBT;

    return node;
}
/**
 * @brief Parses a decrement operator
 * @return AST node representing the decrement operation
 * @throws Exits with error if not followed by an identifier
 *
 * Handles the decrement operator (--).
 */
AST_NODE *Parser::parseDecrementOperator()
{
    proceed(TOKEN_OPERATOR_DECREMENT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_OPERATOR_DECREMENT;

    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "ERROR: Expected identifier after decrement operator." << std::endl;
        exit(1);
    }

    AST_NODE *identNode = new AST_NODE();
    identNode->TYPE = NODE_IDENTIFIER;
    identNode->VALUE = current->value;
    node->SUB_STATEMENTS.push_back(identNode);
    proceed(TOKEN_IDENTIFIER);
    return node;
}
/**
 * @brief Parses an increment operator
 * @return AST node representing the increment operation
 * @throws Exits with error if not followed by an identifier
 *
 * Handles the increment operator (++).
 */
AST_NODE *Parser::parseIncrementOperator()
{
    proceed(TOKEN_OPERATOR_INCREMENT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_OPERATOR_INCREMENT;

    // Check for identifier after increment
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "ERROR: Expected identifier after increment operator." << std::endl;
        exit(1);
    }

    // Create identifier node as a child of the increment
    AST_NODE *identNode = new AST_NODE();
    identNode->TYPE = NODE_IDENTIFIER;
    identNode->VALUE = current->value;

    node->SUB_STATEMENTS.push_back(identNode);

    proceed(TOKEN_IDENTIFIER);

    return node;
}

/**
 * @brief Parses a greater than operator
 * @return AST node representing the greater than comparison
 */
AST_NODE *Parser::greaterThan()
{
    proceed(TOKEN_OPERATOR_GREATERTHAN);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_GREATER_THAN;

    return node;
}

/**
 * @brief Parses a less than operator
 * @return AST node representing the less than comparison
 */
AST_NODE *Parser::lessThan()
{
    proceed(TOKEN_OPERATOR_LESSTHAN);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_LESS_THAN;

    return node;
}
/**
 * @brief parses division operators
 * @return AST node representing the division operator
 */
AST_NODE *Parser::parseDivi()
{
    proceed(TOKEN_OPERATOR_DIV);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_DIVISION;
    return node;
}
/**
 * @brief parses modulus operators
 * @return AST node representing the modulus operator
 */
AST_NODE *Parser::parseModulus()
{
    proceed(TOKEN_OPERATOR_MODULUS);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_MODULUS;
    return node;
}
/**
 * @brief parses != operators
 * @return AST node representing the != operator
 */
AST_NODE *Parser::parseDoesntEqual()
{
    proceed(TOKEN_OPERATOR_DOESNT_EQUAL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_NOT_EQUAL;

    return node;
}
//-------------------------------------------------------------------
// Function Handling methods
//-------------------------------------------------------------------
/**
 * @brief Parses a function declaration
 * @return AST node representing the function declaration
 * @throws Exits with error on syntax issues
 */
AST_NODE *Parser::parseFunctionDecleration()
{
    proceed(TOKEN_KEYWORD_FUNCTION);
    // get function name
    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Functions must have a name." << std::endl;
        exit(1);
        return nullptr;
    }
    std::string functionName = current->value;
    proceed(TOKEN_IDENTIFIER);

    // Create function node
    AST_NODE *function = new AST_NODE();
    function->TYPE = NODE_FUNCTION_DECLERATION;
    function->VALUE = functionName;

    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Function must have params within parenthesis" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *params = parseFunctionParams();
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected closing parenthesis after functions params." << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    if (current->TYPE != TOKEN_SPACESHIP)
    {
        std::cerr << "< Syntax Error > Expected spaceship ( => ) following function decleration" << std::endl;
        exit(1);
    }
    proceed(TOKEN_SPACESHIP); // Consume arrow token

    AST_NODE *functionBody = nullptr;

    if (current->TYPE == TOKEN_LEFT_CURL)
    {
        functionBody = parseFunctionBody();
    }
    else
    {
        functionBody = new AST_NODE();
        functionBody->TYPE = NODE_FUNCTION_BODY;
        functionBody->SUB_STATEMENTS.push_back(parseStatement());
    }

    function->SUB_STATEMENTS.push_back(params);
    function->CHILD = functionBody;

    return function;
}

/**
 * @brief Parses function parameters
 * @return AST node representing the parameter list
 */
AST_NODE *Parser::parseFunctionParams()
{
    AST_NODE *paramsNode = new AST_NODE();
    paramsNode->TYPE = NODE_FUNCTION_PARAMS;

    if (current->TYPE == TOKEN_RIGHT_PAREN)
    {
        return paramsNode;
    }
    while (true)
    {
        AST_NODE *param = parseParameter();
        if (param)
        {
            paramsNode->SUB_STATEMENTS.push_back(param);
        }

        if (current->TYPE == TOKEN_COMMA)
        {
            proceed(TOKEN_COMMA);
        }
        else
        {
            break;
        }
    }
    return paramsNode;
}

/**
 * @brief Parses an individual function parameter
 * @return AST node representing a single parameter
 * @throws Exits with error on syntax issues
 */
AST_NODE *Parser::parseParameter()
{
    NODE_TYPE paramType;

    if (current->TYPE == TOKEN_KEYWORD_INT)
    {
        paramType = NODE_INT;
        proceed(TOKEN_KEYWORD_INT);
    }
    else if (current->TYPE == TOKEN_KEYWORD_DOUBLE)
    {
        paramType = NODE_DOUBLE;
        proceed(TOKEN_KEYWORD_DOUBLE);
    }
    else if (current->TYPE == TOKEN_KEYWORD_CHAR)
    {
        paramType = NODE_CHAR;
        proceed(TOKEN_KEYWORD_CHAR);
    }
    else if (current->TYPE == TOKEN_KEYWORD_STR)
    {
        paramType = NODE_STRING;
        proceed(TOKEN_KEYWORD_STR);
    }
    else
    {
        std::cerr << "< Syntax Error > Expected parameter type" << std::endl;
        exit(1);
    }

    if (current->TYPE != TOKEN_IDENTIFIER)
    {
        std::cerr << "< Syntax Error > Expected parameter name" << std::endl;
        exit(1);
    }

    std::string paramName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *paramNode = new AST_NODE();
    paramNode->TYPE = NODE_PARAM;
    paramNode->VALUE = paramName;

    AST_NODE *typeNode = new AST_NODE();
    typeNode->TYPE = paramType;
    paramNode->CHILD = typeNode;

    return paramNode;
}

/**
 * @brief Parses a function body
 * @return AST node representing the function body
 */
AST_NODE *Parser::parseFunctionBody()
{
    AST_NODE *bodyNode = parseLeftCurl();

    bodyNode->TYPE = NODE_FUNCTION_BODY;
    return bodyNode;
}

//-------------------------------------------------------------------
// Block and control structure parsing methods
//-------------------------------------------------------------------

/**
 * @brief Parses a block of code enclosed in curly braces
 * @return AST node representing the block
 * @throws Exits with error on syntax issues
 *
 * Handles blocks of code enclosed in curly braces { }.
 */

AST_NODE *Parser::parseLeftCurl()
{
    proceed(TOKEN_LEFT_CURL);

    AST_NODE *blockNode = new AST_NODE();
    blockNode->TYPE = NODE_BLOCK;

    // Parse statements until closing brace
    while (current != nullptr && current->TYPE != TOKEN_RIGHT_CURL)
    {
        // Skip comments
        if (current->TYPE == TOKEN_SINGLELINE_COMMENT || current->TYPE == TOKEN_MULTILINE_COMMENT)
        {
            advanceCursor();
            continue;
        }

        // Use statement dispatch table
        AST_NODE *statement = parseByTokenType(statementDispatch);
        if (statement != nullptr)
        {
            blockNode->SUB_STATEMENTS.push_back(statement);
        }

        // Handle semicolons within the block
        if (current != nullptr && current->TYPE == TOKEN_SEMICOLON)
        {
            proceed(TOKEN_SEMICOLON);
        }
    }

    // Check for the closing brace
    if (current == nullptr || current->TYPE != TOKEN_RIGHT_CURL)
    {
        std::cerr << "< Syntax Error > Expected '}' to close block" << std::endl;
        exit(1);
    }

    // Consume the closing brace
    proceed(TOKEN_RIGHT_CURL);

    return blockNode;
}

/**
 * @brief Handles unexpected right curly brace
 * @return Always exits with error
 * @throws Exits with error
 *
 * Called when a right curly brace is found without a matching left curly brace.
 */
AST_NODE *Parser::parseRightCurl()
{
    // This should not be called directly
    // If we make it here, it means we found a closing brace without a matching open brace
    std::cerr << "< Syntax Error > Unexpected '}' without matching '{'" << std::endl;
    exit(1);

    return nullptr;
}

/**
 * @brief Parses the beginning block of a program
 * @return AST node representing the begin block
 *
 * Handles the 'begin:' block that starts a program.
 */
AST_NODE *Parser::parseBeginBlock()
{
    proceed(TOKEN_KEYWORD_BEGIN);

    // Create node for begin block
    AST_NODE *beginNode = new AST_NODE();
    beginNode->TYPE = NODE_BEGIN_BLOCK;

    // Parse statements until EOF
    while (current != nullptr && current->TYPE != TOKEN_EOF)
    {
        AST_NODE *statement = parseStatement();
        if (statement != nullptr)
        {
            beginNode->SUB_STATEMENTS.push_back(statement);
        }

        // Handle semicolons between statements
        if (current != nullptr && current->TYPE == TOKEN_SEMICOLON)
        {
            proceed(TOKEN_SEMICOLON);
        }
    }
    return beginNode;
}

/**
 * @brief Parses a check statement
 * @returns AST node representing the check statement
 * @throws exits with error on syntax issues
 */

AST_NODE *Parser::parseKeywordCheck()
{
    proceed(TOKEN_KEYWORD_CHECK);

    // Parse the condition
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "Expected '(' after if keyword" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *conditionNode = parseExpression();

    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "Expected ')' after if condition" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    // Parse the loop block
    AST_NODE *loop = nullptr;

    if (current->TYPE == TOKEN_LEFT_CURL)
    {
        loop = parseLeftCurl();
    }
    else
    {
        loop = new AST_NODE();
        loop->TYPE = NODE_BLOCK;
        loop->SUB_STATEMENTS.push_back(parseStatement());
    }

    // Create if statement node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_CHECK;
    node->CHILD = conditionNode;
    node->SUB_STATEMENTS.push_back(loop);

    return node;
}

/**
 * @brief Parses an if statement
 * @return AST node representing the if statement
 * @throws Exits with error on syntax issues
 *
 * Handles if-else statements with their conditions and blocks.
 */
AST_NODE *Parser::parseKeywordIf()
{
    proceed(TOKEN_KEYWORD_IF);

    // Parse the condition
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "Expected '(' after if keyword" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    std::cout << "After left paren: " << getTokenTypeName(current->TYPE);
    AST_NODE *condition = parseExpression();

    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "Expected ')' after if condition" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    // Parse 'then' block
    AST_NODE *thenBlock = nullptr;

    if (current->TYPE == TOKEN_LEFT_CURL)
    {
        thenBlock = parseLeftCurl();
    }
    else
    {
        thenBlock = new AST_NODE();
        thenBlock->TYPE = NODE_BLOCK;
        thenBlock->SUB_STATEMENTS.push_back(parseStatement());
    }

    // Parse optional 'else' block
    AST_NODE *elseBlock = nullptr;
    if (current != nullptr && current->TYPE == TOKEN_KEYWORD_ELSE)
    {
        proceed(TOKEN_KEYWORD_ELSE);

        if (current->TYPE == TOKEN_LEFT_CURL)
        {
            elseBlock = parseLeftCurl();
        }
        else
        {
            elseBlock = new AST_NODE();
            elseBlock->TYPE = NODE_BLOCK;
            elseBlock->SUB_STATEMENTS.push_back(parseStatement());
        }
    }

    // Create if statement node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_IF;
    node->CHILD = condition;
    node->SUB_STATEMENTS.push_back(thenBlock);
    if (elseBlock != nullptr)
    {
        node->SUB_STATEMENTS.push_back(elseBlock);
    }
    return node;
}

/**
 * @brief Handles unexpected else keyword
 * @return Always exits with error
 * @throws Exits with error
 *
 * Called when an 'else' is found without a matching 'if'.
 */
AST_NODE *Parser::parseKeywordElse()
{
    std::cerr << "< Syntax Error > Unexpected 'else' without matching if" << std::endl;
    exit(1);

    return nullptr;
}

/**
 * @brief Parses the arguments for a for loop
 * @return AST node representing the for loop arguments
 * @throws Exits with error on syntax issues
 *
 * Handles the three parts of for loop arguments: initialization,
 * condition, and increment.
 */
AST_NODE *Parser::parseArgs()
{
    AST_NODE *args = new AST_NODE();
    args->TYPE = NODE_FOR_ARGS;

    // Parse initialization (optional)
    if (current->TYPE != TOKEN_SEMICOLON)
    {
        if (current->TYPE == TOKEN_KEYWORD_INT || current->TYPE == TOKEN_KEYWORD_DOUBLE)
        {
            AST_NODE *init = parseStatement();
            args->SUB_STATEMENTS.push_back(init);
        }
        else
        {
            AST_NODE *init = parseExpression();
            args->SUB_STATEMENTS.push_back(init);

            if (current->TYPE != TOKEN_SEMICOLON)
            {
                std::cerr << "< Syntax Error > Expected ';' after initialization in for loop" << std::endl;
                exit(1);
            }
            proceed(TOKEN_SEMICOLON);
        }
    }
    else
    {
        args->SUB_STATEMENTS.push_back(nullptr);
        proceed(TOKEN_SEMICOLON);
    }

    // Parse condition (optional)
    if (current->TYPE != TOKEN_SEMICOLON)
    {
        AST_NODE *condition = parseExpression();
        args->SUB_STATEMENTS.push_back(condition);
    }
    else
    {
        args->SUB_STATEMENTS.push_back(nullptr);
    }

    if (current->TYPE != TOKEN_SEMICOLON)
    {
        std::cerr << "< Syntax Error > Expected ';' after condition in for loop" << std::endl;
        exit(1);
    }

    proceed(TOKEN_SEMICOLON);

    // Parse increment (optional)
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        AST_NODE *increment = parseExpression();
        args->SUB_STATEMENTS.push_back(increment);
    }
    else
    {
        args->SUB_STATEMENTS.push_back(nullptr);
    }
    return args;
}

/**
 * @brief Parses a for loop
 * @return AST node representing the for loop
 * @throws Exits with error on syntax issues
 *
 * Handles for loops with their initialization, condition, increment,
 * and body block.
 */
AST_NODE *Parser::parseKeywordFor()
{
    proceed(TOKEN_KEYWORD_FOR);

    // Parse for loop parameters
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "< Syntax Error > Expected '(' after for keyword" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *args = parseArgs();
    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "Expected ')'  after for args" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

    // Parse for loop body
    AST_NODE *forBlock = nullptr;
    if (current->TYPE == TOKEN_LEFT_CURL)
    {
        forBlock = parseLeftCurl();
    }
    else
    {
        forBlock = new AST_NODE();
        forBlock->TYPE = NODE_BLOCK;
        forBlock->SUB_STATEMENTS.push_back(parseStatement());
    }

    // Create for loop node
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_FOR;
    node->CHILD = args;
    node->SUB_STATEMENTS.push_back(forBlock);

    return node;
}

/**
 * @brief Parses a term (atomic element of an expression)
 * @return AST node representing the term
 * @throws Exits with error on syntax issues
 *
 * Handles the atomic elements of expressions, like literals,
 * identifiers, and parenthesized expressions.
 */

AST_NODE *Parser::parseTerm()
{
    if (current->TYPE == TOKEN_OPERATOR_SUBT)
    {
        proceed(TOKEN_OPERATOR_SUBT);

        AST_NODE *unaryMinus = new AST_NODE();
        unaryMinus->TYPE = NODE_SUBT;

        AST_NODE *operand = parseTerm();
        unaryMinus->SUB_STATEMENTS.push_back(operand);
        return unaryMinus;
    }

    return parseByTokenType(expressionDispatch);
}

/**
 * @brief Parses an expression
 * @return AST node representing the expression
 *
 * Handles expressions with operators, applying operator precedence
 * through a left-to-right evaluation. Builds a tree representing
 * the expression structure.
 */
AST_NODE *Parser::parseExpression()
{
    AST_NODE *left = parseTerm();

    while (current != nullptr && operatorPrecedence.find(current->TYPE) != operatorPrecedence.end())
    {
        tokenType opType = current->TYPE;
        NODE_TYPE nodeType = tokenToNodeType[opType];

        proceed(opType);

        // Handle special case for post-increment/decrement
        if (opType == TOKEN_OPERATOR_INCREMENT || opType == TOKEN_OPERATOR_DECREMENT)
        {
            AST_NODE *opNode = new AST_NODE();
            opNode->TYPE = nodeType;
            opNode->SUB_STATEMENTS.push_back(left);
            return opNode;
        }

        // For binary operators, parse the right operand
        AST_NODE *opNode = new AST_NODE();
        opNode->TYPE = nodeType;

        AST_NODE *right = parseTerm();

        // Build the operator node with left and right children
        opNode->SUB_STATEMENTS.push_back(left);
        opNode->SUB_STATEMENTS.push_back(right);

        // The entire expression becomes the new left operand
        left = opNode;
    }

    return left;
}

/**
 * @brief Main parsing method that processes the entire program
 * @return AST node representing the root of the program
 * @throws Exits with error on syntax issues
 *
 * Entry point for parsing the token stream into an AST.
 * Handles the overall program structure and validates that
 * the program ends properly.
 */
AST_NODE *Parser::parse()
{
    initializeParserMaps();

    AST_NODE *root = new AST_NODE();
    root->TYPE = NODE_ROOT;

    bool foundBegin = false;

    while (cursor < size && tokens[cursor]->TYPE != TOKEN_EOF)
    {
        current = tokens[cursor];
        if (current == nullptr)
        {
            break;
        }

        if (current->TYPE == TOKEN_SINGLELINE_COMMENT || current->TYPE == TOKEN_MULTILINE_COMMENT)
        {
            advanceCursor();
            continue;
        }

        if (current->TYPE == TOKEN_KEYWORD_BEGIN)
        {
            if (foundBegin)
            {
                std::cerr << "< Syntax Error > Multiple 'begin' blocks found" << std::endl;
                exit(1);
            }
            AST_NODE *statement = parseBeginBlock();
            root->SUB_STATEMENTS.push_back(statement);
            foundBegin = true;
        }
        else
        {
            AST_NODE *statement = parseByTokenType(statementDispatch);
            if (statement != nullptr)
            {
                root->SUB_STATEMENTS.push_back(statement);
            }
        }

        if (cursor < size && tokens[cursor]->TYPE == TOKEN_SEMICOLON)
        {
            proceed(TOKEN_SEMICOLON);
        }
    }

    if (cursor < size && tokens[cursor]->TYPE == TOKEN_EOF)
    {
        if (tokens[cursor]->value == "end")
        {
            current = tokens[cursor];
            root->SUB_STATEMENTS.push_back(parseKeywordEOF());
        }
        else
        {
            std::cerr << "< Syntax Error > Program must end with keyword 'end' 1" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "< Syntax Error > Program must end with keyword 'end' 2" << std::endl;
        exit(1);
    }

    return root;
}

/**
 * @brief Helper function to get string representation of node types
 * @param type The node type to convert to string
 * @return String representation of the node type
 *
 * Converts the enum node type to a human-readable string for debugging
 * and error reporting purposes.
 */
std::string getNodeTypeName(NODE_TYPE type)
{
    switch (type)
    {
    case NODE_ROOT:
        return "NODE_ROOT";
    case NODE_VARIABLE:
        return "NODE_VARIABLE";
    case NODE_PRINT:
        return "NODE_PRINT";
    case NODE_RETURN:
        return "NODE_RETURN";
    case NODE_INT:
        return "NODE_INT";
    case NODE_INT_LITERAL:
        return "NODE_INT_LITERAL";
    case NODE_EQUALS:
        return "NODE_EQUALS";
    case NODE_SEMICOLON:
        return "NODE_SEMICOLON";
    case NODE_IDENTIFIER:
        return "NODE_IDENTIFIER";
    case NODE_ADD:
        return "NODE_ADD";
    case NODE_DOUBLE_LITERAL:
        return "NODE_DOUBLE_LITERAL";
    case NODE_DOUBLE:
        return "NODE_DOUBLE";
    case NODE_CHAR_LITERAL:
        return "NODE_CHAR_LITERAL";
    case NODE_CHAR:
        return "NODE_CHAR";
    case NODE_STRING_LITERAL:
        return "NODE_STRING_LITERAL";
    case NODE_STRING:
        return "NODE_STRING";
    case NODE_LEFT_PAREN:
        return "NODE_LEFT_PAREN";
    case NODE_RIGHT_PAREN:
        return "NODE_RIGHT_PAREN";
    case NODE_PAREN_EXPR:
        return "NODE_PAREN_EXPR";
    case NODE_LEFT_CURL:
        return "NODE_LEFT_CURL";
    case NODE_RIGHT_CURL:
        return "NODE_RIGHT_CURL";
    case NODE_LESS_THAN:
        return "NODE_LESS_THAN";
    case NODE_GREATER_THAN:
        return "NODE_GREATER_THAN";
    case NODE_BLOCK:
        return "NODE_BLOCK";
    case NODE_IF:
        return "NODE_IF";
    case NODE_EOF:
        return "NODE_EOF";
    case NODE_MULT:
        return "NODE_MULT";
    case NODE_SUBT:
        return "NODE_SUBT";
    case NODE_FOR:
        return "NODE_FOR";
    case NODE_FOR_ARGS:
        return "NODE_FOR_ARGS";
    case NODE_OPERATOR_INCREMENT:
        return "NODE_OPERATOR_INCREMENT";
    case NODE_NEWLINE:
        return "NODE_NEWLINE";
    case NODE_FUNCTION_DECLERATION:
        return "NODE_FUNCTION_DECLERATION";
    case NODE_FUNCTION_PARAMS:
        return "NODE_FUNCTION_PARAMS";
    case NODE_FUNCTION_BODY:
        return "NODE_FUNCTION_BODY";
    case NODE_PARAM:
        return "NODE_PARAM";
    case NODE_FUNCTION_SPACESHIP:
        return "NODE_FUNCTION_SPACESHIP";
    case NODE_BEGIN_BLOCK:
        return "NODE_BEGIN_BLOCK";
    case NODE_FUNCTION:
        return "NODE_FUNCTION";
    case NODE_FUNCTION_CALL:
        return "NODE_FUNCTION_CALL";
    case NODE_DIVISION:
        return "NODE_DIVISION";
    case NODE_MODULUS:
        return "NODE_MODULUS";
    case NODE_NEWLINE_SYMBOL:
        return "NODE_NEWLINE_SYMBOL";
    case NODE_NOT_EQUAL:
        return "NODE_NOT_EQUAL";
    case NODE_BOOL:
        return "NODE_BOOL";
    case NODE_BOOL_LITERAL:
        return "NODE_BOOL_LITERAL";
    case NODE_RESULT_EXPRESSION:
        return "NODE_RESULT_EXPRESSION";
    case NODE_RESULTSTATEMENT:
        return "NODE_RESULTSTATEMENT";
    case NODE_LESS_EQUAL:
        return "NODE_LESS_EQUAL";
    case NODE_OPERATOR_DECREMENT:
        return "NODE_OPERATOR_DECREMENT";
    case NODE_KEYWORD_INPUT:
        return "NODE_KEYWORD_INPUT";
    case NODE_INPUT_TYPE:
        return "NODE_INPUT_TYPE";
    case NODE_INPUT_PROMPT:
        return "NODE_INPUT_PROMPT";
    case NODE_CHECK:
        return "NODE_CHECK";
    case NODE_ARRAY_DECLARATION:
        return "NODE_ARRAY_DECLARATION";
    case NODE_ARRAY_ACCESS:
        return "NODE_ARRAY_ACCESS";
    case NODE_ARRAY_ASSIGN:
        return "NODE_ARRAY_ASSIGN";
    case NODE_ARRAY_RANGE:
        return "NODE_ARRAY_RANGE";
    case NODE_ARRAY_INIT:
        return "NODE_ARRAY_INIT";
    case NODE_ARRAY_REPEAT:
        return "NODE_ARRAY_REPEAT";
    case NODE_ARRAY_LENGTH:
        return "NODE_ARRAY_LENGTH";
    case NODE_ARRAY_INSERT:
        return "NODE_ARRAY_INSERT";
    case NODE_ARRAY_REMOVE:
        return "NODE_ARRAY_REMOVE";
    case NODE_ARRAY_SORT_ASC:
        return "NODE_ARRAY_SORT_ASC";
    case NODE_ARRAY_SORT_DESC:
        return "NODE_ARRAY_SORT_DESC";
    case NODE_ARRAY_LAST_INDEX:
        return "NODE_ARRAY_LAST_INDEX";
    case NODE_COMMENT:
        return "NODE_COMMENT";
    case NODE_RANGE_OPERATOR:
        return "NODE_RANGE_OPERATOR";
    case NODE_ELEMENT_TYPE:
        return "NODE_ELEMENT_TYPE";
    default:
        return "Unknown node";
    }
}

/**
 * @brief Parses a single statement
 * @return AST node representing the statement
 * @throws Exits with error on syntax issues
 *
 * General-purpose method for parsing individual statements
 * based on their starting token. Handles semicolon placement
 * based on statement type.
 */
AST_NODE *Parser::parseStatement()
{
    if (current == nullptr)
    {
        std::cerr << "Unexpected end of file while parsing statement" << std::endl;
        exit(1);
    }

    AST_NODE *statement = parseByTokenType(statementDispatch);

    if (statement != nullptr && current != nullptr &&
        current->TYPE == TOKEN_SEMICOLON &&
        statement->TYPE != NODE_BLOCK &&
        statement->TYPE != NODE_IF)
    {
        proceed(TOKEN_SEMICOLON);
    }
    return statement;
}