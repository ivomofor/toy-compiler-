#include <toy/ASTPrinter.h>

#include <iostream>

namespace toy {

    void ASTPrinter::print(const Program &program) {
        std::cout << "Program\n";

        for (const auto &decl : program.declarations) {
            printFunction( static_cast<const FunctionDecl &>(*decl), 2);
        }
    }

    void ASTPrinter::printIndent(int indent) {
        for (int i = 0; i < indent; ++i)
            std::cout << ' ';
    }

    void ASTPrinter::printFunction(const FunctionDecl &function,int indent) {

        printIndent(indent);

        std::cout << "Function: " << function.name << '\n';

        for (const auto &stmt : function.body)
            printStatement(*stmt, indent + 2);
    }

    void ASTPrinter::printStatement(const Statement &statement,int indent) {

        auto returnStmt = dynamic_cast<const ReturnStmt*>(&statement);

        if (!returnStmt)
            return;

        printIndent(indent);
        std::cout << "Return\n";

        printExpression( *returnStmt->value,indent + 2);
    }

    void ASTPrinter::printExpression( const Expression &expression,int indent) {

        auto integer = dynamic_cast<const IntegerLiteral*>(&expression);

        if (!integer)
            return;

        printIndent(indent);

        std::cout << "Integer: " << integer->value << '\n';
    }

}