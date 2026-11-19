#ifndef AST_WHILE_HPP
#define AST_WHILE_HPP

#include "ast/expression.hpp"
#include "ast/body.hpp"


namespace  ast {


class While {

    ast::Expression cond;
    ast::Body body;

public:

    While(Expression cond, Body body) :
        cond { std::move(cond) },
        body { std::move(body) } { }

    While(const While& other) :
        cond { other.cond },
        body { other.body } { }

    While(While&& other) noexcept = default;

    While& operator=(const While& other) = default;
    While& operator=(While&& other) noexcept = default;

    Expression condition() const { return cond; }
    Body loop_body() const { return body; }
};


}  // namespace ast


#endif  // AST_WHILE_HPP
