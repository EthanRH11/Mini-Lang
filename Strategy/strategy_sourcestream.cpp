#include "strategy_lexer.hpp"

CharacterStream::CharacterStream(const std::string &source)
{
}

char CharacterStream::current() const
{
}

char CharacterStream::advanceCursor()
{
}

char CharacterStream::peekAhead(int offset = 1) const
{
}

void CharacterStream::skipWhiteSpace()
{
}

bool CharacterStream::eof() const
{
}

bool CharacterStream::matchKeyword(const std::string &keyword) const
{
}

void CharacterStream::consumeKeyword(const std::string &keyword)
{
}

size_t CharacterStream::getPosition() const
{
}