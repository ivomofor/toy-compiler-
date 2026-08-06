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

    class Program : public ASTNode {
        public:
            std::vector<std::unique_ptr<ASTNode>> declarations;
    };

    class Statement : public ASTNode {
        public:
            virtual ~Statement() = default;
    };

    class Expression : public ASTNode {
        public:
            virtual ~Expression() = default;
    };

    class CallExpression : public Expression {
        public:
            CallExpression(std::string callee, std::vector<std::unique_ptr<Expression>> arguments)
                : callee(std::move(callee)), arguments(std::move(arguments)) {}

            std::string callee;
            std::vector<std::unique_ptr<Expression>> arguments;
    };

    class FunctionDecl : public ASTNode {
        public:
            explicit FunctionDecl(std::string name)
            : name(std::move(name)) {}

        std::string name;
        std::vector<std::string> parameters;
        std::vector<std::unique_ptr<Statement>> body;
    };

    class ReturnStmt : public Statement {
        public:
            explicit ReturnStmt(std::unique_ptr<Expression> value)
            : value(std::move(value)) {}

        std::unique_ptr<Expression> value;
    };

    class VariableDecl : public Statement {
        public:
            VariableDecl(std::string name, std::unique_ptr<Expression> initializer)
                : name(std::move(name)), initializer(std::move(initializer)) {}

            std::string name;
            std::unique_ptr<Expression> initializer;
    };

    class IfStmt : public Statement {
        public:
            IfStmt(std::unique_ptr<Expression> condition, std::vector<std::unique_ptr<Statement>> thenBody)
                : condition(std::move(condition)),thenBody(std::move(thenBody)) {}

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> thenBody;
    };

    class WhileStmt : public Statement {
    public:
        WhileStmt(std::unique_ptr<Expression> condition,std::vector<std::unique_ptr<Statement>> body)
            : condition(std::move(condition)),body(std::move(body)) {}

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> body;
    };

    class IntegerLiteral : public Expression {
        public:
            explicit IntegerLiteral(int value)
            : value(value) {}

        int value;
    };

    class VariableReference : public Expression {
        public:
            explicit VariableReference(std::string name)
                : name(std::move(name)) {}

            std::string name;
    };

    class BinaryExpression : public Expression {
        public:
            BinaryExpression(char op, std::unique_ptr<Expression> left,std::unique_ptr<Expression> right)
                : op(op), left(std::move(left)), right(std::move(right)) {}

        char op;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
    };

}

#endif