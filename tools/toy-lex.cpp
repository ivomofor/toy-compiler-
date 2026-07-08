#include <fstream>
#include <iostream>
#include <sstream>

#include "toy/Lexer.h"

using namespace toy;

static const char *tokenKindToString(TokenKind kind) {
    switch (kind) {
        case TokenKind::Eof:
            return "Eof";

        case TokenKind::Identifier:
            return "Identifier";

        case TokenKind::Integer:
            return "Integer";

        case TokenKind::Fun:
            return "Fun";

        case TokenKind::Return:
            return "Return";

        case TokenKind::LParen:
            return "LParen";

        case TokenKind::RParen:
            return "RParen";

        case TokenKind::LBrace:
            return "LBrace";

        case TokenKind::RBrace:
            return "RBrace";

        case TokenKind::Semicolon:
            return "Semicolon";

        case TokenKind::Unknown:
            return "Unknown";
    }

    return "Invalid";
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: toy-lex <source-file>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Error: Could not open file: " << argv[1] << '\n';
        return 1;
    }

    std::stringstream buffer;
    buffer << input.rdbuf();

    Lexer lexer(buffer.str());

    while (true) {
        Token token = lexer.nextToken();

        std::cout << tokenKindToString(token.kind);

        if (!token.text.empty())
            std::cout << " : \"" << token.text << "\"";

        std::cout << '\n';

        if (token.kind == TokenKind::Eof)
            break;
    }

    return 0;
}