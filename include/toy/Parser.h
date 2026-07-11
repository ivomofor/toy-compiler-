#ifndef TOY_PARSER_H
#define TOY_PARSER_H

#include "toy/Lexer.h"

namespace toy {

    class Parser {
        public:
            explicit Parser(Lexer &lexer);
            void parseProgram();

        private:
            void advance();
            Lexer &lexer;
            Token currentToken;
            void match(TokenKind expected);
            void parseFunction();
            void parseStatement();
    };

} 

#endif