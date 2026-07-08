#ifndef TOY_LEXER_H
#define TOY_LEXER_H

#include <cctype>
#include <string>

namespace toy {

enum class TokenKind {
    Eof,

    Identifier,
    Integer,

    Fun,
    Return,

    LParen,
    RParen,

    LBrace,
    RBrace,

    Semicolon,

    Unknown
};

struct Token {
    TokenKind kind;
    std::string text;
};

class Lexer {
public:
    explicit Lexer(std::string source);

    Token nextToken();

private:
    std::string source;
    size_t position = 0;

    char current() const;
    void advance();
    void skipWhitespace();

    Token identifier();
    Token number();
};

} // namespace toy

#endif