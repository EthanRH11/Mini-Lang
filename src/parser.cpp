#include "parser.hpp"
#include <iostream>

Token *Parser::proceed(enum tokenType type)
{
    // Check if we're at the end of the token stream
    if (cursor >= size)
    {
        std::cerr << "< Syntax Error > Unexpected end of file" << std::endl;
        exit(1);
    }

    // Check if the current token matches the expected type
    if (tokens[cursor]->TYPE != type)
    {
        std::cerr << "< Syntax Error > Expected " << getTokenTypeName(type)
                  << " but got " << getTokenTypeName(tokens[cursor]->TYPE) << std::endl;
        exit(1);
    }

    // Advance cursor
    cursor++;

    // Update current pointer with bounds check
    if (cursor < size)
    {
        current = tokens[cursor];
    }
    else
    {
        // We've reached the end of the token stream
        current = nullptr;
    }

    return current;
}

// Helper to get current token
Token *Parser::getCurrentToken()
{
    return current;
}

// Advance the cursor
void Parser::advanceCursor()
{
    if (cursor < size)
    {
        cursor++;
        current = tokens[cursor];
    }
}

Token *Parser::peakAhead()
{
    // Check if we can look ahead
    if (cursor + 1 < size)
    {
        return tokens[cursor + 1];
    }
    else
    {
        // if we are at the end of the tokenstream return null
        return nullptr;
    }
}

// EOF Keyword
AST_NODE *Parser::parseKeywordEOF()
{
    std::string eofValue = current->value;
    proceed(TOKEN_EOF);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_EOF;
    node->VALUE = eofValue;

    return node;
}

// PRINT Keyword
AST_NODE *Parser::parseKeywordPrint()
{
    proceed(TOKEN_KEYWORD_PRINT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_PRINT;

    return node;
}

// Parse Keyword INT
AST_NODE *Parser::parseIntegerValue()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INT_LITERAL;
    node->VALUE = current->value;

    proceed(TOKEN_INTEGER_VAL);

    return node;
}

AST_NODE *Parser::parseEquals()
{
    proceed(TOKEN_EQUALS);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_EQUALS;
    node->VALUE = current->value;

    return node;
}

AST_NODE *Parser::parserIntegerValue()
{

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INT_LITERAL;
    node->VALUE = current->value;

    proceed(TOKEN_INTEGER_VAL);

    return node;
}

AST_NODE *Parser::parseSemicolon()
{
    proceed(TOKEN_SEMICOLON);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_SEMICOLON;

    return node;
}

AST_NODE *Parser::parseID()
{
    std::string identifierName = current->value;

    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_IDENTIFIER;
    node->VALUE = identifierName;

    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);

        if (current == nullptr || current->TYPE != TOKEN_INTEGER_VAL)
        {
            std::cerr << "< Syntax Error > Expected value after '=' " << std::endl;
            exit(1);
        }

        AST_NODE *childNode = new AST_NODE();
        childNode->TYPE = NODE_INT_LITERAL;
        childNode->VALUE = current->value;

        node->CHILD = childNode;
        proceed(TOKEN_INTEGER_VAL);
    }

    return node;
}

AST_NODE *Parser::parseKeywordINT()
{
    proceed(TOKEN_KEYWORD_INT);
    std::cout << "We made it here (1)" << std::endl;

    // if (current == nullptr || current->TYPE != TOKEN_IDENTIFIER)
    // {
    //     std::cerr << "< Syntax Error > Expected Identifier after 'int' " << std::endl;
    //     exit(1);
    // }

    std::string variableName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INT;
    node->VALUE = variableName;

    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);

        if (current == nullptr || current->TYPE != TOKEN_INTEGER_VAL)
        {
            std::cerr << "< Syntax Error > Expected integer value after '=' " << std::endl;
            exit(1);
        }

        AST_NODE *childNode = new AST_NODE();
        childNode->TYPE = NODE_INT_LITERAL;
        childNode->VALUE = current->value;

        node->CHILD = childNode;
        proceed(TOKEN_INTEGER_VAL);
    }
    else
    {
        node->CHILD = nullptr;
    }
    return node;
}

AST_NODE *Parser::parseLeftParen()
{
    proceed(TOKEN_LEFT_PAREN);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_PAREN_EXPR;

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

AST_NODE *Parser::parseRightParen()
{
    // This is handled in left paren
    // if we make it here something is wrong
    std::cerr << "< Syntax Error > Unexpected right parenthesis." << std::endl;
    exit(1);

    return nullptr;
}

AST_NODE *Parser::parseAdd()
{
    proceed(TOKEN_OPERATOR_ADD);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_ADD;

    return node;
}

AST_NODE *Parser::parse()
{
    AST_NODE *root = new AST_NODE();
    root->TYPE = NODE_ROOT;

    while (cursor < size && tokens[cursor]->TYPE != TOKEN_EOF)
    {
        current = tokens[cursor];
        if (current == nullptr)
        {
            break;
        }

        AST_NODE *statement = nullptr;

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

        default:
            std::cerr << "< Syntax Error > Unexpected token: "
                      << getTokenTypeName(current->TYPE) << std::endl;
            exit(1);
        }

        if (statement != nullptr)
        {
            root->SUB_STATEMENTS.push_back(statement);
        }

        if (cursor < size && tokens[cursor]->TYPE == TOKEN_SEMICOLON)
        {
            root->SUB_STATEMENTS.push_back(parseSemicolon());
        }
    }

    if (cursor < size && tokens[cursor]->TYPE == TOKEN_EOF)
    {
        current = tokens[cursor];
        root->SUB_STATEMENTS.push_back(parseKeywordEOF());
    }
    return root;
}