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
        if (text == "var")
            return {TokenKind::Var, text};
        if (text == "if")
            return {TokenKind::If, text};
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

            case '=':
            advance();
            return Token{TokenKind::Equal, "="};

            case ';':
            advance();
            return {TokenKind::Semicolon, ";"};

            case '+':
            advance();
            return {TokenKind::Plus, "+"};

            case '-':
            advance();
            return {TokenKind::Minus, "-"};

            case '*':
            advance();
            return {TokenKind::Star, "*"};

            case '/':
            advance();
            return {TokenKind::Slash, "/"};

            case ',':
                advance();
                return {TokenKind::Comma, ","};

            default:
            advance();
                return {TokenKind::Unknown, std::string(1, ch)};
        }
    }

}