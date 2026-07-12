#ifndef TOY_AST_H
#define TOY_AST_H

#include <memory>
#include <string>
#include <vector>

namespace toy {

    class ASTNode {
        public:
            virtual ~ASTNode() = default;
    };

    class Statement : public ASTNode {
        public:
            virtual ~Statement() = default;
    };

    class Expression : public ASTNode {
        public:
            virtual ~Expression() = default;
    };
    
    class Program : public ASTNode {
        public:
            std::vector<std::unique_ptr<ASTNode>> declarations;
    };
    
    class FunctionDecl : public ASTNode {
        public:
            explicit FunctionDecl(std::string name)
                : name(std::move(name)) {}

            std::string name;
            std::vector<std::unique_ptr<Statement>> body;
    };

    class ReturnStmt : public Statement {
        public:
            explicit ReturnStmt(std::unique_ptr<Expression> value)
                : value(std::move(value)) {}

            std::unique_ptr<Expression> value;
    };

    class IntegerLiteral : public Expression {
        public:
            explicit IntegerLiteral(int value)
                : value(value) {}

            int value;
    };
}

#endif