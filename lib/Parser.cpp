#include "toy/Parser.h"
#include "toy/AST.h"
#include <stdexcept>

namespace toy {

    Parser::Parser(Lexer &lexer)
        : lexer(lexer) {
            currentToken = this->lexer.nextToken();
        }

    void Parser::advance() {
        currentToken = lexer.nextToken();
    }

    void Parser::match(TokenKind expected) {
        if (currentToken.kind != expected) 
            throw std::runtime_error("Unexpected token");
        advance();  
    }

    void Parser::parseProgram() {
        while (currentToken.kind != TokenKind::Eof) 
            parseFunction();
    }

    void Parser::parseStatement() {
        if (currentToken.kind == TokenKind::Return) {
            advance();

            int value = std::stoi(currentToken.text);

            match(TokenKind::Integer);
            match(TokenKind::Semicolon);

            ReturnStmt stmt(value);

            return;
        }

        throw std::runtime_error("Unknown statement.");
    }

    void Parser::parseFunction() {
        match(TokenKind::Fun);
        match(TokenKind::Identifier);
        match(TokenKind::LParen);
        match(TokenKind::RParen);
        match(TokenKind::LBrace);

        while (currentToken.kind != TokenKind::RBrace) 
            parseStatement();

        match(TokenKind::RBrace);
    }

} 