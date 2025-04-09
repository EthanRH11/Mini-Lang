#include "parser.hpp"
#include <iostream>

Token *Parser::proceed(enum tokenType type)
{
    std::cout << "About to proceed: Expected " << getTokenTypeName(type)
              << ", Got " << getTokenTypeName(tokens[cursor]->TYPE)
              << ", Value: " << tokens[cursor]->value << std::endl;
    if (cursor >= size)
    {
        std::cerr << "< Syntax Error > Unexpected end of file" << std::endl;
        exit(1);
    }

    if (tokens[cursor]->TYPE != type)
    {
        std::cerr << "< Syntax Error > Expected " << getTokenTypeName(type)
                  << " but got " << getTokenTypeName(tokens[cursor]->TYPE) << std::endl;
        exit(1);
    }

    cursor++;

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
        return nullptr;
    }
}

AST_NODE *Parser::parseDoubleValue()
{
    std::string doubleValue = current->value;
    proceed(TOKEN_DOUBLE_VAL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_DOUBLE_LITERAL;
    node->VALUE = doubleValue;

    return node;
}

AST_NODE *Parser::parseKeywordDouble()
{
    proceed(TOKEN_KEYWORD_DOUBLE);

    std::string identifierName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_DOUBLE;
    node->VALUE = identifierName;

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

AST_NODE *Parser::parseStringValue()
{
    AST_NODE *node = new AST_NODE();

    if (current->TYPE == TOKEN_STRING_VAL)
    {
        node->TYPE = NODE_STRING_LITERAL;
        node->VALUE = current->value;

        proceed(TOKEN_STRING_VAL);
    }
    else if (current->TYPE == TOKEN_KEYWORD_STR)
    {
        proceed(TOKEN_KEYWORD_STR);

        std::string variableName = current->value;
        proceed(TOKEN_IDENTIFIER);
        node->TYPE = NODE_STRING;
        node->VALUE = variableName;

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
AST_NODE *Parser::parseKeywordString()
{
    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_STRING;
    node->VALUE = current->value;

    return node;
}
AST_NODE *Parser::parseKeywordChar()
{
    proceed(TOKEN_KEYWORD_CHAR);
    std::string identifierName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_CHAR;
    node->VALUE = identifierName;

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
AST_NODE *Parser::parseCharValue()
{
    std::string charValue = current->value;
    proceed(TOKEN_CHAR_VAL);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_CHAR_LITERAL;
    node->VALUE = charValue;

    return node;
}

// EOF Keyword
AST_NODE *Parser::parseKeywordEOF()
{
    std::string eofValue = current->value;
    proceed(TOKEN_EOF);

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

// PRINT Keyword
AST_NODE *Parser::parseKeywordPrint()
{
    proceed(TOKEN_KEYWORD_PRINT);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_PRINT;
    if (current->TYPE != TOKEN_LEFT_PAREN)
    {
        std::cerr << "Expected '(' after print keyword" << std::endl;
        exit(1);
    }
    proceed(TOKEN_LEFT_PAREN);
    node->CHILD = parseExpression();

    if (current->TYPE != TOKEN_RIGHT_PAREN)
    {
        std::cerr << "Expected ')' after print argument" << std::endl;
        exit(1);
    }
    proceed(TOKEN_RIGHT_PAREN);

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

    if (identifierName == "out_to_console")
    {
        AST_NODE *node = new AST_NODE();
        node->TYPE = NODE_PRINT;

        if (current->TYPE != TOKEN_LEFT_PAREN)
        {
            std::cerr << "Expected '(' after out_to_console" << std::endl;
            exit(1);
        }
        proceed(TOKEN_LEFT_PAREN);
        node->CHILD = parseExpression();

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

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_IDENTIFIER;
    node->VALUE = identifierName;

    if (current != nullptr && current->TYPE == TOKEN_EQUALS)
    {
        proceed(TOKEN_EQUALS);

        node->CHILD = parseExpression();
    }
    return node;
}

AST_NODE *Parser::parseKeywordINT()
{
    proceed(TOKEN_KEYWORD_INT);

    std::string variableName = current->value;
    proceed(TOKEN_IDENTIFIER);

    AST_NODE *node = new AST_NODE();
    node->TYPE = NODE_INT;
    node->VALUE = variableName;

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
    std::cerr << "Unexpected token in expression" << std::endl;
    exit(1);
    return nullptr;
}
AST_NODE *Parser::parseExpression()
{
    AST_NODE *left = parseTerm();
    while (current != nullptr &&
           (current->TYPE == TOKEN_OPERATOR_ADD ||
            current->TYPE == TOKEN_OPERATOR_SUBT))
    {

        AST_NODE *opNode = nullptr;
        if (current->TYPE == TOKEN_OPERATOR_ADD)
        {
            proceed(TOKEN_OPERATOR_ADD);
            opNode = new AST_NODE();
            opNode->TYPE = NODE_ADD;
        }

        // ADD FUTURE OPERATORS HERE
        AST_NODE *right = parseTerm();

        opNode->SUB_STATEMENTS.push_back(left);
        opNode->SUB_STATEMENTS.push_back(right);

        left = opNode;
    }
    return left;
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

        case TOKEN_STRING_VAL:
            statement = parseStringValue();
            break;

        case TOKEN_KEYWORD_STR:
            statement = parseKeywordString();
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

        // Expect a semicolon after most statements
        if (cursor < size && tokens[cursor]->TYPE == TOKEN_SEMICOLON)
        {
            proceed(TOKEN_SEMICOLON); // Just consume it, don't add to tree
        }
    }

    // Handle EOF
    if (cursor < size && tokens[cursor]->TYPE == TOKEN_EOF)
    {
        current = tokens[cursor];
        root->SUB_STATEMENTS.push_back(parseKeywordEOF());
    }
    return root;
}

std::string getNodeTypeName(NODE_TYPE type)
{
    switch (type)
    {
    case NODE_ROOT:
        return "NODE_ROOT";
    case NODE_VARIABLE:
        return "NODE_VARIABLE";
    case NODE_PRINT:
        return "NODE_PRINT"; // 2
    case NODE_RETURN:
        return "NODE_RETURN"; // 3
    case NODE_INT:
        return "NODE_INT"; // 4
    case NODE_INT_LITERAL:
        return "NODE_INT_LITERAL"; // 5
    case NODE_EQUALS:
        return "NODE_EQUALS"; // 6
    case NODE_SEMICOLON:
        return "NODE_SEMICOLON"; // 7
    case NODE_IDENTIFIER:
        return "NODE_IDENTIFIER"; // 8
    case NODE_ADD:
        return "NODE_ADD"; // 9
    case NODE_DOUBLE_LITERAL:
        return "NODE_DOUBLE_LITERAL"; // 10
    case NODE_DOUBLE:
        return "NODE_DOUBLE"; // 11
    case NODE_CHAR_LITERAL:
        return "NODE_CHAR_LITERAL"; // 12
    case NODE_CHAR:
        return "NODE_CHAR"; // 13
    case NODE_STRING_LITERAL:
        return "NODE_STRING_LITERAL"; // 14
    case NODE_STRING:
        return "NODE_STRING"; // 15
    case NODE_LEFT_PAREN:
        return "NODE_LEFT_PAREN"; // 16
    case NODE_RIGHT_PAREN:
        return "NODE_RIGHT_PAREN"; // 17
    case NODE_PAREN_EXPR:
        return "NODE_PAREN_EXPR"; // 18
    case NODE_EOF:
        return "NODE_EOF"; // 19
    default:
        return "Unknown node";
    }
}
