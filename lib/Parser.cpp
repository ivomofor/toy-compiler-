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

    std::unique_ptr<Program> Parser::parseProgram() {

        auto program = std::make_unique<Program>();

        while (currentToken.kind != TokenKind::Eof) {
            program->declarations.push_back(parseFunction());
        }

        return program;
    }

    std::unique_ptr<Expression> Parser::parseExpression() {
        int value = std::stoi(currentToken.text);

        match(TokenKind::Integer);

        return std::make_unique<IntegerLiteral>(value);
    }

    std::unique_ptr<Statement> Parser::parseStatement() {

        match(TokenKind::Return);

        auto value = parseExpression();

        match(TokenKind::Semicolon);

        return std::make_unique<ReturnStmt>(std::move(value));
    }

    std::unique_ptr<FunctionDecl> Parser::parseFunction() {

        match(TokenKind::Fun);

        std::string name = currentToken.text;
        match(TokenKind::Identifier);

        match(TokenKind::LParen);
        match(TokenKind::RParen);

        match(TokenKind::LBrace);

        auto function = std::make_unique<FunctionDecl>(name);

        while (currentToken.kind != TokenKind::RBrace) {
            function->body.push_back(parseStatement());
        }

        match(TokenKind::RBrace);

        return function;
    }

} 