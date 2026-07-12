#ifndef TOY_PARSER_H
#define TOY_PARSER_H

#include "toy/Lexer.h"
#include "toy/AST.h"

namespace toy {

class Parser {
    public:
        explicit Parser(Lexer &lexer);

        std::unique_ptr<Program> parseProgram();

    private:
        Lexer &lexer;
        Token currentToken;

        void advance();
        void match(TokenKind expected);

        std::unique_ptr<FunctionDecl> parseFunction();
        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<Expression> parseExpression();
    };

} 

#endif