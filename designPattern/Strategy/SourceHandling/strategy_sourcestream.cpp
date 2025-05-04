#include "strategy_lexer.hpp"

CharacterStream::CharacterStream(const std::string &source) : source(source), cursor(0), size(source.length())
{
}

char CharacterStream::current() const
{
    if (cursor >= size)
    {
        return '\0'; // return null if eof
    }

    return source[cursor];
}

char CharacterStream::advanceCursor()
{
    if (cursor >= size)
    {
        return '\0';
    }

    char currentChar = source[cursor];
    cursor++;
    return currentChar;
}

char CharacterStream::peekAhead(int offset = 1) const
{
    size_t position = cursor + offset;
    if (position >= size)
    {
        return '\0';
    }
    return source[position];
}

void CharacterStream::skipWhiteSpace()
{
    while (cursor < size && isspace(source[cursor]))
    {
        cursor++;
    }
}

bool CharacterStream::eof() const
{
    return cursor >= size;
}

bool CharacterStream::matchKeyword(const std::string &keyword) const
{
    // Ensure characters still left
    if (cursor + keyword.length() > size)
    {
        return false;
    }

    for (size_t i = 0; i < keyword.length(); i++)
    {
        if (source[cursor + i] != keyword[i])
        {
            return false;
        }
    }

    if (cursor + keyword.length() < size)
    {
        char nextChar = source[cursor + keyword.length()];
        if (isalnum(nextChar) || nextChar == '_')
        {
            return false;
        }
    }
    return true;
}

void CharacterStream::consumeKeyword(const std::string &keyword)
{
    cursor += keyword.length();
}

size_t CharacterStream::getPosition() const
{
    return cursor;
}