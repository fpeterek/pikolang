#ifndef AST_BODY_HPP
#define AST_BODY_HPP

#include "ast/statement.hpp"
#include "collections/array.hpp"


namespace  ast {

class Body {

    Array<ast::Statement> statements;

public:

    Body(Array<ast::Statement> statements) :
        statements { std::move(statements) } { }

    const Array<ast::Statement>& body() const { return statements; }
};


}  // namespace ast


#endif  // AST_BODY_HPP
