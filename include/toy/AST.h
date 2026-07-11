#ifndef TOY_AST_H
#define TOY_AST_H

#include <memory>

namespace toy {

    class ASTNode {
        public:
            virtual ~ASTNode() = default;
    };

    class ReturnStmt : public ASTNode {
        public:
            explicit ReturnStmt(int value)
                : value(value) {}

            int value;
    };

}

#endif