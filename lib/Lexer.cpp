#include "toy/Lexer.h"

namespace toy {
Lexer::Lexer(std::string source)
    : source(std::move(source)) {}

char Lexer::current() const {
    if (position >= source.size())
        return '\0';
    return source[position];

}

void Lexer::advance() {
    if(position < source.size())
        ++position;
}

void Lexer::skipWhitespace() {
    while (std::isspace(current()))
        advance();   
}

Token Lexer::identifier() {
    size_t start = position;
    while(std::isalnum(current()) || current() == '_')
        advance();
    std::string text = source.substr(start, position - start);

    if (text == "return")
        return {TokenKind::Return, text};
    if (text == "fun")
        return {TokenKind::Fun, text};
    return {TokenKind::Identifier, text};
}

Token Lexer::number() {
    size_t start = position;
    while (std::isdigit(current()))
        advance();
    return {TokenKind::Integer, source.substr(start, position -start)};
}

Token Lexer::nextToken() {

    skipWhitespace();

    char ch = current();

    if (ch == '\0')
        return {TokenKind::Eof, ""};

    if (std::isalpha(ch))
        return identifier();

    if (std::isdigit(ch))
        return number();

    switch (ch) {
        case '(':
        advance();
        return {TokenKind::LParen, "("};

        case ')':
        advance();
        return {TokenKind::RParen, ")"};

        case '{':
        advance();
        return {TokenKind::LBrace, "{"};

        case '}':
        advance();
        return {TokenKind::RBrace, "}"};

        case ';':
        advance();
        return {TokenKind::Semicolon, ";"};

        default:
        advance();
            return {TokenKind::Unknown, std::string(1, ch)};
    }    
}

}