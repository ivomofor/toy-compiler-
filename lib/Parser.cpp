#include "toy/Parser.h"
#include "toy/AST.h"
#include <stdexcept>

namespace toy {

    Parser::Parser(Lexer &lexer)
        : lexer(lexer) {
            currentToken = this->lexer.nextToken();
            lookaheadToken = this->lexer.nextToken();
        }

    void Parser::advance() {
        currentToken = lookaheadToken;
        lookaheadToken = lexer.nextToken();
    }

    void Parser::match(TokenKind expected) {
        if (currentToken.kind != expected) 
            throw std::runtime_error("Unexpected token");
        advance();  
    }

    std::unique_ptr<Expression> Parser::parseExpression() {
        auto left = parseTerm();
        if (!left)
            return nullptr;
        while (currentToken.kind == TokenKind::Plus || currentToken.kind == TokenKind::Minus) {
            char op;
            if (currentToken.kind == TokenKind::Plus)
                op = '+';
            else
                op = '-';
            advance();
            auto right = parseTerm();
            if (!right) {
                throw std::runtime_error("Expected expression after operator");
            }
            left = std::make_unique<BinaryExpression>(op,std::move(left),std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expression> Parser::parseCallExpression() {

        std::string callee = currentToken.text;
        match(TokenKind::Identifier);
        match(TokenKind::LParen);
        std::vector<std::unique_ptr<Expression>> arguments;

        if (currentToken.kind != TokenKind::RParen) {
            while (true) {
                arguments.push_back(parseExpression());
                if (currentToken.kind == TokenKind::RParen) {
                    break;
                }
                match(TokenKind::Comma);
            }
        }

        match(TokenKind::RParen);
        return std::make_unique<CallExpression>(std::move(callee), std::move(arguments));
    }

    std::unique_ptr<Expression> Parser::parsePrimary() {

        if (currentToken.kind == TokenKind::Integer) {
            int value = std::stoi(currentToken.text);
            advance();
            return std::make_unique<IntegerLiteral>(value);
        }

        if (currentToken.kind == TokenKind::Identifier) {

            if (lookaheadToken.kind == TokenKind::LParen) {
                return parseCallExpression();
            }

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

    std::unique_ptr<Expression> Parser::parseTerm() {

        auto left = parsePrimary();

        if (!left)
            return nullptr;

        while (currentToken.kind == TokenKind::Star ||currentToken.kind == TokenKind::Slash) {
            char op;
            if (currentToken.kind == TokenKind::Star)
                op = '*';
            else
                op = '/';

            advance();
            auto right = parsePrimary();

            if (!right) {
                throw std::runtime_error("Expected expression after operator");
            }

            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }

        return left;
    }

    std::unique_ptr<VariableDecl> Parser::parseVariableDecl() {

        match(TokenKind::Var);
        std::string name = currentToken.text;
        match(TokenKind::Identifier);
        match(TokenKind::Equal);
        auto initializer = parseExpression();
        match(TokenKind::Semicolon);

        return std::make_unique<VariableDecl>(std::move(name),std::move(initializer));
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

        if (currentToken.kind == TokenKind::If) {
            return parseIfStatement();
        }

        if (currentToken.kind == TokenKind::While) {
            return parseWhileStatement();
        }

        return nullptr;
    }

    std::unique_ptr<IfStmt> Parser::parseIfStatement() {

        match(TokenKind::If);
        match(TokenKind::LParen);

        auto condition = parseExpression();

        match(TokenKind::RParen);
        match(TokenKind::LBrace);

        std::vector<std::unique_ptr<Statement>> thenBody;

        while (currentToken.kind != TokenKind::RBrace) {

            auto stmt = parseStatement();

            if (!stmt)
                throw std::runtime_error("Invalid statement inside if");

            thenBody.push_back(std::move(stmt));
        }

        match(TokenKind::RBrace);

        return std::make_unique<IfStmt>(std::move(condition),std::move(thenBody));
    }

    std::unique_ptr<WhileStmt> Parser::parseWhileStatement() {

        match(TokenKind::While);
        match(TokenKind::LParen);

        auto condition = parseExpression();

        match(TokenKind::RParen);
        match(TokenKind::LBrace);

        std::vector<std::unique_ptr<Statement>> body;

        while (currentToken.kind != TokenKind::RBrace) {

            auto stmt = parseStatement();

            if (!stmt)
                throw std::runtime_error("Invalid statement inside while");

            body.push_back(std::move(stmt));
        }

        match(TokenKind::RBrace);

        return std::make_unique<WhileStmt>(std::move(condition),std::move(body));
    }

    std::unique_ptr<FunctionDecl> Parser::parseFunction() {

        match(TokenKind::Fun);
        std::string name = currentToken.text;
        match(TokenKind::Identifier);
        auto function = std::make_unique<FunctionDecl>(name);
        match(TokenKind::LParen);

        if (currentToken.kind != TokenKind::RParen) {

            while (true) {

                if (currentToken.kind != TokenKind::Identifier) {
                    throw std::runtime_error("Expected parameter name");
                }
                function->parameters.push_back(currentToken.text);
                advance();
                if (currentToken.kind == TokenKind::RParen)
                    break;
                match(TokenKind::Comma);
            }
        }
        match(TokenKind::RParen);
        match(TokenKind::LBrace);

        //auto function = std::make_unique<FunctionDecl>(name);
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