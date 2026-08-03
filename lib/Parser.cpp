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

    std::unique_ptr<Expression> Parser::parseExpression() {

        auto left = parsePrimary();

        if (!left)
            return nullptr;

        while (
            currentToken.kind == TokenKind::Plus ||
            currentToken.kind == TokenKind::Minus ||
            currentToken.kind == TokenKind::Star ||
            currentToken.kind == TokenKind::Slash
        ) {

            char op = '+';

        switch (currentToken.kind) {
                case TokenKind::Plus:
                    op = '+';
                    break;
                case TokenKind::Minus:
                    op = '-';
                    break;
                case TokenKind::Star:
                    op = '*';
                    break;
                case TokenKind::Slash:
                    op = '/';
                    break;
                default:
                    break;
            }

            advance();

            auto right = parsePrimary();

            if (!right)
                throw std::runtime_error("Expected expression after operator");

            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<Expression> Parser::parsePrimary() {

        if (currentToken.kind == TokenKind::Integer) {
            int value = std::stoi(currentToken.text);
            advance();
            return std::make_unique<IntegerLiteral>(value);
        }

        if (currentToken.kind == TokenKind::Identifier) {
            std::string name = currentToken.text;
            advance();
            return std::make_unique<VariableReference>(std::move(name));
        }

        if (currentToken.kind == TokenKind::LParen) {
            match(TokenKind::LParen);
            auto expression = parseExpression();
            match(TokenKind::RParen);
            return expression;
        }

        return nullptr;
    }

    std::unique_ptr<VariableDecl> Parser::parseVariableDecl() {

        match(TokenKind::Var);

        std::string name = currentToken.text;

        match(TokenKind::Identifier);
        match(TokenKind::Equal);

        auto initializer = parseExpression();

        match(TokenKind::Semicolon);

        return std::make_unique<VariableDecl>(
            std::move(name),
            std::move(initializer)
        );
    }
    
    std::unique_ptr<Statement> Parser::parseStatement() {

        if (currentToken.kind == TokenKind::Var) {
            return parseVariableDecl();
        }

        if (currentToken.kind == TokenKind::Return) {
            match(TokenKind::Return);
            auto value = parseExpression();
            match(TokenKind::Semicolon);
            return std::make_unique<ReturnStmt>(std::move(value));
        }

        return nullptr;
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
        auto statement = parseStatement();

        if (!statement) {
            throw std::runtime_error("Unexpected token in function body: " + currentToken.text);
        }

        function->body.push_back(std::move(statement));
    }

        match(TokenKind::RBrace);

        return function;
    }

    std::unique_ptr<Program> Parser::parseProgram() {

        auto program = std::make_unique<Program>();

        while (currentToken.kind != TokenKind::Eof) {
            program->declarations.push_back(parseFunction());
        }

        return program;
    }

} 