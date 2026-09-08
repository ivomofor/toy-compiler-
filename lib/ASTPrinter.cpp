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

        if (!function.parameters.empty()) {
            printIndent(indent + 2);
            std::cout << "Parameters\n";
            for (const auto &parameter : function.parameters) {
                printIndent(indent + 4);
                std::cout << parameter << '\n';
            }
        }

        for (const auto &stmt : function.body) {
            printStatement(*stmt,indent + 2);
        }
    }

    void ASTPrinter::printStatement(const Statement &statement, int indent) {

        if (statement.getKind() == ASTNodeKind::VariableDecl) {
            auto *variableDecl =
                static_cast<const VariableDecl *>(&statement);

            printIndent(indent);
            std::cout << "Variable: " << variableDecl->name << '\n';
            printExpression(*variableDecl->initializer, indent + 2);
            return;
        }

        if (statement.getKind() == ASTNodeKind::IfStmt) {
            auto *ifStmt =
                static_cast<const IfStmt *>(&statement);

            printIndent(indent);
            std::cout << "If\n";

            printIndent(indent + 2);
            std::cout << "Condition\n";

            printExpression(*ifStmt->condition, indent + 4);

            printIndent(indent + 2);
            std::cout << "Then\n";

            for (const auto &stmt : ifStmt->thenBody) {
                printStatement(*stmt, indent + 4);
            }

            return;
        }

        if (statement.getKind() == ASTNodeKind::WhileStmt) {
            auto *whileStmt =
                static_cast<const WhileStmt *>(&statement);

            printIndent(indent);
            std::cout << "While\n";

            printIndent(indent + 2);
            std::cout << "Condition\n";

            printExpression(*whileStmt->condition, indent + 4);

            printIndent(indent + 2);
            std::cout << "Body\n";

            for (const auto &stmt : whileStmt->body) {
                printStatement(*stmt, indent + 4);
            }

            return;
        }

        if (statement.getKind() == ASTNodeKind::ReturnStmt) {
            auto *returnStmt =
                static_cast<const ReturnStmt *>(&statement);

            printIndent(indent);
            std::cout << "Return\n";

            printExpression(*returnStmt->value, indent + 2);
            return;
        }
    }

   

    void ASTPrinter::printExpression(
    const Expression &expression,
    int indent) {

        if (expression.getKind() == ASTNodeKind::IntegerLiteral) {
            auto *integer =
                static_cast<const IntegerLiteral *>(&expression);

            printIndent(indent);
            std::cout << "Integer: " << integer->value << '\n';
            return;
        }

        if (expression.getKind() == ASTNodeKind::VariableReference) {
            auto *variable =
                static_cast<const VariableReference *>(&expression);

            printIndent(indent);
            std::cout << "VariableReference: "
                      << variable->name << '\n';
            return;
        }

        if (expression.getKind() == ASTNodeKind::BinaryExpression) {
            auto *binary =
                static_cast<const BinaryExpression *>(&expression);

            printIndent(indent);
            std::cout << "BinaryExpression: "
                      << binary->op << '\n';

            printExpression(*binary->left, indent + 2);
            printExpression(*binary->right, indent + 2);
            return;
        }

        if (expression.getKind() == ASTNodeKind::CallExpression) {
            auto *call =
                static_cast<const CallExpression *>(&expression);

            printIndent(indent);
            std::cout << "Call: "
                      << call->callee << '\n';

            for (const auto &argument : call->arguments) {
                printExpression(*argument, indent + 2);
            }

            return;
        }
    }

}