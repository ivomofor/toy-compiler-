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

   void ASTPrinter::printStatement( const Statement &statement, int indent) { 
    
        auto variableDecl = dynamic_cast<const VariableDecl *>(&statement); 

        if (variableDecl) { 

            printIndent(indent); 

            std::cout << "Variable: " << variableDecl->name << '\n'; 

            printExpression( *variableDecl->initializer, indent + 2 ); 

            return;
     } 

     auto returnStmt = dynamic_cast<const ReturnStmt *>(&statement); 

     if (returnStmt) { 

            printIndent(indent); std::cout << "Return\n"; printExpression( *returnStmt->value, indent + 2 ); 

            return;
        } 
    }

   

    void ASTPrinter::printExpression( const Expression &expression,int indent) {

        auto integer = dynamic_cast<const IntegerLiteral*>(&expression);

        if (integer) {
            printIndent(indent);
            std::cout << "Integer: " << integer->value << '\n';
            return;
        }

        auto variable = dynamic_cast<const VariableReference *>(&expression);

        if (variable) {
            printIndent(indent);
            std::cout << "VariableReference: " << variable->name << '\n';
            return;
        }

        auto binary = dynamic_cast<const BinaryExpression *>(&expression);

        if (binary) {
            printIndent(indent);
            std::cout << "BinaryExpression: " << binary->op << '\n';
            printExpression(*binary->left,indent + 2);
            printExpression(*binary->right,indent + 2);
            return;
        }
    }

}