#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>

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

    // Handle function calls
    if (current != nullptr && current->TYPE == TOKEN_LEFT_PAREN)
    {
        AST_NODE *functionCallNode = new AST_NODE();
        functionCallNode->TYPE = NODE_FUNCTION_CALL;
        functionCallNode->VALUE = identifierName;
        proceed(TOKEN_LEFT_PAREN);

        // Parse function arguments
        while (current != nullptr && current->TYPE != TOKEN_RIGHT_PAREN)
        {
            AST_NODE *argNode = parseExpression();
            functionCallNode->SUB_STATEMENTS.push_back(argNode);

            // Handle comma-separated arguments
            if (current->TYPE == TOKEN_COMMA)
            {
                proceed(TOKEN_COMMA);
            }
            else if (current->TYPE != TOKEN_RIGHT_PAREN)
            {
                std::cerr << "< Syntax Error > Expected ',' or ')'" << std::endl;
                exit(1);
            }
        }
        proceed(TOKEN_RIGHT_PAREN);
        return functionCallNode;
    }

    // Handle special identifiers
    if (identifierName == "out_to_console")
    {
        AST_NODE *node = new AST_NODE();
        node->TYPE = NODE_PRINT;

        // Check for opening parenthesis
        if (current->TYPE != TOKEN_LEFT_PAREN)
        {
            std::cerr << "Expected '(' after out_to_console" << std::endl;
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
    else if (identifierName == "end")
    {
        return parseKeywordEOF();
    }

    // Regular variable identifier
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_IDENTIFIER;
    node->VALUE = identifierName;

    // Check for assignment
    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
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

    // Parse the content inside parentheses
    if (current->TYPE == TOKEN_IDENTIFIER)
    {
        AST_NODE *childNode = new AST_NODE();
        childNode->TYPE = NODE_IDENTIFIER;
        childNode->VALUE = current->value;
        node->CHILD = childNode;
        proceed(TOKEN_IDENTIFIER);
    }
    else if (current->TYPE == TOKEN_INTEGER_VAL)
    {
        AST_NODE *childNode = new AST_NODE();
        childNode->TYPE = NODE_INT_LITERAL;
        childNode->VALUE = current->value;
        node->CHILD = childNode;
        proceed(TOKEN_INTEGER_VAL);
    }
    else
    {
        // Handle functions in the future.
        std::cerr << "< Syntax Error > Unexpeceted token inside parenthesis." << std::endl;
        exit(1);
    }

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
        AST_NODE *statement = nullptr;

        // Handle different statement types
        switch (current->TYPE)
        {
        case TOKEN_IDENTIFIER:
            statement = parseID();
            break;
        case TOKEN_KEYWORD_INT:
            statement = parseKeywordINT();
            break;
        case TOKEN_OPERATOR_INCREMENT:
            statement = parseIncrementOperator();
            break;
        case TOKEN_KEYWORD_PRINT:
            statement = parseKeywordPrint();
            break;
        case TOKEN_KEYWORD_CHAR:
            statement = parseKeywordChar();
            break;
        case TOKEN_KEYWORD_DOUBLE:
            statement = parseKeywordDouble();
            break;
        case TOKEN_KEYWORD_STR:
            statement = parseStringValue();
            break;
        case TOKEN_KEYWORD_FOR:
            statement = parseKeywordFor();
            break;
        case TOKEN_KEYWORD_IF:
            statement = parseKeywordIf();
            break;
        case TOKEN_OPERATOR_DOESNT_EQUAL:
            statement = parseDoesntEqual();
            break;
        case TOKEN_NL_SYMBOL:
            statement = parseNewLineCharacter();
            break;
        default:
            std::cerr << "< Syntax Error > Unexpected token in block: "
                      << getTokenTypeName(current->TYPE) << std::endl;
            exit(1);
        }

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
    if (current->TYPE == TOKEN_INTEGER_VAL)
    {
        return parseIntegerValue();
    }
    else if (current->TYPE == TOKEN_IDENTIFIER)
    {
        std::string identifierName = current->value;
        proceed(TOKEN_IDENTIFIER);

        AST_NODE *node = new AST_NODE();
        node->TYPE = NODE_IDENTIFIER;
        node->VALUE = identifierName;
        return node;
    }
    else if (current->TYPE == TOKEN_LEFT_PAREN)
    {
        proceed(TOKEN_LEFT_PAREN);
        AST_NODE *expr = parseExpression();

        if (current->TYPE != TOKEN_RIGHT_PAREN)
        {
            std::cerr << "Expected closing parenthesis" << std::endl;
            exit(1);
        }
        proceed(TOKEN_RIGHT_PAREN);
        return expr;
    }
    else if (current->TYPE == TOKEN_CHAR_VAL)
    {
        return parseCharValue();
    }
    else if (current->TYPE == TOKEN_STRING_VAL)
    {
        return parseStringValue();
    }
    else if (current->TYPE == TOKEN_DOUBLE_VAL)
    {
        return parseDoubleValue();
    }
    else if (current->TYPE == TOKEN_BOOL_VALUE)
    {
        return parseBoolValue();
    }
    else if (current->TYPE == TOKEN_OPERATOR_INCREMENT)
    {
        proceed(TOKEN_OPERATOR_INCREMENT);
        if (current->TYPE != TOKEN_IDENTIFIER)
        {
            std::cerr << "Expected identifier after increment operator" << std::endl;
            exit(1);
        }
        std::string identifierName = current->value;
        proceed(TOKEN_IDENTIFIER);

        // Create identifier node
        AST_NODE *idNode = new AST_NODE();
        idNode->TYPE = NODE_IDENTIFIER;
        idNode->VALUE = identifierName;

        // Create increment node with identifier as child
        AST_NODE *incNode = new AST_NODE();
        incNode->TYPE = NODE_OPERATOR_INCREMENT;
        incNode->SUB_STATEMENTS.push_back(idNode);

        return incNode;
    }
    else if (current->TYPE == TOKEN_OPERATOR_NEWLINE)
    {
        return parseNewLine();
    }
    else if (current->TYPE == TOKEN_NL_SYMBOL)
    {
        return parseNewLineCharacter();
    }

    // If we get here, the token is not valid in an expression
    std::cerr << "Unexpected token in expression" << std::endl;
    exit(1);
    return nullptr;
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
    // Start with a term as the left-hand side
    AST_NODE *left = parseTerm();

    // Continue processing operators as long as they're available
    while (current != nullptr &&
           (current->TYPE == TOKEN_OPERATOR_ADD ||
            current->TYPE == TOKEN_OPERATOR_SUBT ||
            current->TYPE == TOKEN_OPERATOR_MULT ||
            current->TYPE == TOKEN_OPERATOR_LESSTHAN ||
            current->TYPE == TOKEN_OPERATOR_GREATERTHAN ||
            current->TYPE == TOKEN_OPERATOR_INCREMENT ||
            current->TYPE == TOKEN_OPERATOR_DIV ||
            current->TYPE == TOKEN_OPERATOR_MODULUS ||
            current->TYPE == TOKEN_OPERATOR_DOESNT_EQUAL))
    {
        AST_NODE *opNode = nullptr;

        // Create appropriate operator node based on token type
        if (current->TYPE == TOKEN_OPERATOR_ADD)
        {
            proceed(TOKEN_OPERATOR_ADD);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_ADD;
        }
        else if (current->TYPE == TOKEN_OPERATOR_DIV)
        {
            proceed(TOKEN_OPERATOR_DIV);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_DIVISION;
        }
        else if (current->TYPE == TOKEN_OPERATOR_MODULUS)
        {
            proceed(TOKEN_OPERATOR_MODULUS);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_MODULUS;
        }
        else if (current->TYPE == TOKEN_NL_SYMBOL)
        {
            proceed(TOKEN_NL_SYMBOL);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_NEWLINE_SYMBOL;
        }
        else if (current->TYPE == TOKEN_OPERATOR_SUBT)
        {
            proceed(TOKEN_OPERATOR_SUBT);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_SUBT;
        }
        else if (current->TYPE == TOKEN_OPERATOR_MULT)
        {
            proceed(TOKEN_OPERATOR_MULT);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_MULT;
        }
        else if (current->TYPE == TOKEN_OPERATOR_LESSTHAN)
        {
            proceed(TOKEN_OPERATOR_LESSTHAN);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_LESS_THAN;
        }
        else if (current->TYPE == TOKEN_OPERATOR_GREATERTHAN)
        {
            proceed(TOKEN_OPERATOR_GREATERTHAN);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_GREATER_THAN;
        }
        else if (current->TYPE == TOKEN_OPERATOR_INCREMENT)
        {
            // Special case for post-increment
            proceed(TOKEN_OPERATOR_INCREMENT);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_OPERATOR_INCREMENT;

            // For post-increment, just add the expression as a child
            opNode->SUB_STATEMENTS.push_back(left);
            return opNode;
        }
        else if (current->TYPE == TOKEN_OPERATOR_DOESNT_EQUAL)
        {
            proceed(TOKEN_OPERATOR_DOESNT_EQUAL);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_NOT_EQUAL;
        }

        // For binary operators, parse the right operand
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
    // Create root node
    AST_NODE *root = new AST_NODE();
    root->TYPE = NODE_ROOT;

    bool foundBegin = false;

    std::cout << "Current Token: " << getCurrentToken() << std::endl;

    // Process tokens until EOF
    while (cursor < size && tokens[cursor]->TYPE != TOKEN_EOF)
    {
        current = tokens[cursor];
        if (current == nullptr)
        {
            break;
        }

        AST_NODE *statement = nullptr;

        // Dispatch based on token type
        switch (current->TYPE)
        {
        case TOKEN_IDENTIFIER:
            statement = parseID();
            break;

        case TOKEN_KEYWORD_INT:
            statement = parseKeywordINT();
            break;

        case TOKEN_KEYWORD_PRINT:
            statement = parseKeywordPrint();
            break;

        case TOKEN_OPERATOR_INCREMENT:
            statement = parseIncrementOperator();
            break;

        case TOKEN_EOF:
            statement = parseKeywordEOF();
            break;

        case TOKEN_EQUALS:
            statement = parseEquals();
            break;

        case TOKEN_INTEGER_VAL:
            statement = parseIntegerValue();
            break;

        case TOKEN_SEMICOLON:
            statement = parseSemicolon();
            break;

        case TOKEN_OPERATOR_ADD:
            statement = parseAdd();
            break;

        case TOKEN_LEFT_PAREN:
            statement = parseLeftParen();
            break;

        case TOKEN_KEYWORD_FOR:
            statement = parseKeywordFor();
            break;

        case TOKEN_RIGHT_PAREN:
            statement = parseRightParen();
            break;

        case TOKEN_CHAR_VAL:
            statement = parseCharValue();
            break;

        case TOKEN_KEYWORD_CHAR:
            statement = parseKeywordChar();
            break;

        case TOKEN_DOUBLE_VAL:
            statement = parseDoubleValue();
            break;

        case TOKEN_KEYWORD_DOUBLE:
            statement = parseKeywordDouble();
            break;

        case TOKEN_KEYWORD_STR:
            statement = parseStringValue();
            break;

        case TOKEN_KEYWORD_IF:
            statement = parseKeywordIf();
            break;

        case TOKEN_OPERATOR_GREATERTHAN:
            statement = greaterThan();
            break;

        case TOKEN_LEFT_CURL:
            statement = parseLeftCurl();
            break;

        case TOKEN_RIGHT_CURL:
            statement = parseRightCurl();
            break;

        case TOKEN_KEYWORD_ELSE:
            statement = parseKeywordElse();
            break;

        case TOKEN_OPERATOR_MULT:
            statement = parseMult();
            break;

        case TOKEN_OPERATOR_SUBT:
            statement = parseSubt();
            break;

        case TOKEN_OPERATOR_NEWLINE:
            statement = parseNewLine();
            break;

        case TOKEN_OPERATOR_DIV:
            statement = parseDivi();
            break;

        case TOKEN_OPERATOR_MODULUS:
            statement = parseModulus();
            break;

        case TOKEN_OPERATOR_DOESNT_EQUAL:
            statement = parseDoesntEqual();
            break;

        case TOKEN_NL_SYMBOL:
            statement = parseNewLineCharacter();
            break;

        case TOKEN_KEYWORD_BEGIN:
            // Ensure only one begin block in program
            if (foundBegin)
            {
                std::cerr << "< Syntax Error > Multiple 'begin:' blocks found" << std::endl;
                exit(1);
            }
            statement = parseBeginBlock();
            foundBegin = true;
            break;

        case TOKEN_KEYWORD_FUNCTION:
            statement = parseFunctionDecleration();
            break;

        default:
            std::cerr << "< Syntax Error > Unexpected token: "
                      << getTokenTypeName(current->TYPE) << std::endl;
            exit(1);
        }

        // Add the statement to the AST if valid
        if (statement != nullptr)
        {
            root->SUB_STATEMENTS.push_back(statement);
        }

        // Handle semicolons after statements
        if (cursor < size && tokens[cursor]->TYPE == TOKEN_SEMICOLON)
        {
            proceed(TOKEN_SEMICOLON); // Just consume it, don't add to tree
        }
    }

    // Validate proper program ending
    if (cursor < size && tokens[cursor]->TYPE == TOKEN_EOF)
    {
        if (tokens[cursor]->value == "end")
        {
            current = tokens[cursor];
            root->SUB_STATEMENTS.push_back(parseKeywordEOF());
        }
        else
        {
            std::cerr << "< Syntax Error > Program must end with keyword 'end'" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "< Syntax Error > Program must end with keyword 'end'" << std::endl;
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

    AST_NODE *statement = nullptr;

    // Dispatch based on token type
    switch (current->TYPE)
    {
    case TOKEN_IDENTIFIER:
        statement = parseID();
        break;

    case TOKEN_KEYWORD_INT:
        statement = parseKeywordINT();
        break;

    case TOKEN_KEYWORD_FOR:
        statement = parseKeywordFor();
        break;

    case TOKEN_KEYWORD_PRINT:
        statement = parseKeywordPrint();
        break;

    case TOKEN_EOF:
        statement = parseKeywordEOF();
        break;

    case TOKEN_EQUALS:
        statement = parseEquals();
        break;

    case TOKEN_INTEGER_VAL:
        statement = parseIntegerValue();
        break;

    case TOKEN_SEMICOLON:
        statement = parseSemicolon();
        break;

    case TOKEN_OPERATOR_ADD:
        statement = parseAdd();
        break;

    case TOKEN_LEFT_PAREN:
        statement = parseLeftParen();
        break;

    case TOKEN_RIGHT_PAREN:
        statement = parseRightParen();
        break;

    case TOKEN_CHAR_VAL:
        statement = parseCharValue();
        break;

    case TOKEN_KEYWORD_CHAR:
        statement = parseKeywordChar();
        break;

    case TOKEN_DOUBLE_VAL:
        statement = parseDoubleValue();
        break;

    case TOKEN_KEYWORD_DOUBLE:
        statement = parseKeywordDouble();
        break;

    case TOKEN_KEYWORD_STR:
        statement = parseStringValue();
        break;

    case TOKEN_STRING_VAL:
        statement = parseStringValue();
        break;

    case TOKEN_LEFT_CURL:
        statement = parseLeftCurl();
        break;

    case TOKEN_RIGHT_CURL:
        statement = parseRightCurl();
        break;

    case TOKEN_KEYWORD_IF:
        statement = parseKeywordIf();
        break;

    case TOKEN_OPERATOR_DIV:
        statement = parseDivi();
        break;

    case TOKEN_OPERATOR_MODULUS:
        statement = parseModulus();
        break;

    case TOKEN_OPERATOR_MULT:
        statement = parseMult();
        break;

    case TOKEN_NL_SYMBOL:
        statement = parseNewLineCharacter();
        break;
    case TOKEN_KEYWORD_BOOL:
        statement = parseKeywordBool();
        break;
    case TOKEN_BOOL_VALUE:
        statement = parseBoolValue();
        break;
    default:
        std::cerr << "< Syntax Error > Unexpected token in statement: "
                  << getTokenTypeName(current->TYPE) << std::endl;
        exit(1);
    }

    // Handle semicolons after statements, but only for statements that need them
    // Don't consume semicolons after blocks or control structures that don't need them
    if (statement != nullptr &&
        current != nullptr &&
        current->TYPE == TOKEN_SEMICOLON &&
        statement->TYPE != NODE_BLOCK &&
        statement->TYPE != NODE_IF)
    {
        proceed(TOKEN_SEMICOLON);
    }

    return statement;
}