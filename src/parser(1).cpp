#include "parser(1).hpp"
#include "lexer.hpp"

Parser::Parser(const std::vector<Token *> &tokens)
    : tokens(tokens), cursor(0), size(tokens.size()), current(tokens.empty() ? nullptr : tokens[0])
{
    initializeDispatchTables();
}

void Parser::initializeDispatchTables()
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
        {TOKEN_KEYWORD_INPUT, &Parser::parseKeywordInput}
        // Add all other token types and their parsing functions
    };

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
        {TOKEN_OPERATOR_SUBT, &Parser::parseSubt}
        // Add other expression-related token types
    };
}

std::unique_ptr<AST_NODE> Parser::parseByTokenType(const std::unordered_map<tokenType, ParseFunction> &dispatchTable)
{
    if (current == nullptr)
    {
        std::cerr << "Unexpected end of file";
    }

    auto it = dispatchTable.find(current->TYPE);
    if (it != dispatchTable.end())
    {
        return (this->*(it->second))();
    }
    else
    {
        std::cerr << "Unexpected token: " + getTokenTypeName(current->TYPE);
        return nullptr;
    }
}

std::unique_ptr<AST_NODE> Parser::parse()
{
    auto root = std::make_unique<AST_NODE>();
    root->TYPE = NODE_ROOT;

    bool foundBegin = false;

    while (cursor < size && tokens[cursor]->TYPE != TOKEN_EOF)
    {
        current = tokens[cursor];
        if (current == nullptr)
        {
            break;
        }

        std::unique_ptr<AST_NODE> statement = nullptr;

        if (current->TYPE == TOKEN_SINGLELINE_COMMENT || current->TYPE == TOKEN_MULTILINE_COMMENT)
        {
            advanceCursor();
            continue;
        }

        auto it = statementDispatch.find(current->TYPE);
        if (it != statementDispatch.end())
        {
            statement = (this->*(it->second))();

            if (current->TYPE == TOKEN_KEYWORD_BEGIN)
            {
                if (foundBegin)
                {
                    std::cerr << "< Syntax Error > Multiple 'begin:' blocks found" << std::endl;
                    exit(1);
                }
                foundBegin = true;
            }
        }
        else
        {
            std::cerr << "< Syntax Error > Unexpected token: " << getTokenTypeName(current->TYPE) << std::endl;
            exit(1);
        }

        if (statement != nullptr)
        {
            root->SUB_STATEMENTS.push_back(std::move(statement));
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