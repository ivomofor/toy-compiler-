#ifndef TOY_PARSER_H
#define TOY_PARSER_H

#include "toy/Lexer.h"

namespace toy {

class Parser {
public:
    explicit Parser(Lexer &lexer);

private:
    Lexer &lexer;
    Token currentToken;
};

} 

#endif