#ifndef TOY_AST_PRINTER_H
#define TOY_AST_PRINTER_H

#include "toy/AST.h"

namespace toy {

class ASTPrinter {
    public:
        void print(const Program &program);

    private:
        void printFunction(const FunctionDecl &function, int indent);
        void printStatement(const Statement &statement, int indent);
        void printExpression(const Expression &expression, int indent);

        void printIndent(int indent);
    };

}

#endif