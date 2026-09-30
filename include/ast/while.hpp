#ifndef AST_WHILE_HPP
#define AST_WHILE_HPP

#include <memory>

#include "ast/expression.hpp"
#include "ast/body.hpp"


namespace  ast {


class While {

    std::unique_ptr<ast::Expression> cond;
    ast::Body body;

public:

    While(std::unique_ptr<Expression> cond, Body body) :
        cond { std::move(cond) },
        body { std::move(body) } { }

    While(Expression cond, Body body) :
        While {
            std::make_unique<Expression>(std::move(cond)),
            Body(std::move(body))
        } { }


    While(const While& other) :
        cond { std::make_unique<Expression>(*other.cond) },
        body { other.body } { }

    While(While&& other) noexcept = default;

    While& operator=(const While& other) {

        cond = std::make_unique<Expression>(*other.cond);
        body = other.body;

        return *this;
    }

    While& operator=(While&& other) noexcept = default;

    const Expression& condition() const { return *cond; }
    const Body& loop_body() const { return body; }
};


}  // namespace ast


#endif  // AST_WHILE_HPP
