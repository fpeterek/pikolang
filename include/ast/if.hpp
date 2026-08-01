#ifndef AST_IF_HPP
#define AST_IF_HPP

#include <memory>

#include "ast/expression.hpp"


namespace  ast {


class If {

    std::unique_ptr<Expression> cond;
    std::unique_ptr<Expression> if_true;
    std::unique_ptr<Expression> if_false;

public:

    If(std::unique_ptr<Expression> cond,
       std::unique_ptr<Expression> if_true,
       std::unique_ptr<Expression> if_false = nullptr) :
        cond { std::move(cond) },
        if_true { std::move(if_true) },
        if_false { std::move(if_false) } { }

    If(Expression cond, Expression if_true) :
        If {
            std::make_unique<Expression>(std::move(cond)),
            std::make_unique<Expression>(std::move(if_true))
        } { }

    If(Expression cond, Expression if_true, Expression if_false) :
        If {
            std::make_unique<Expression>(std::move(cond)),
            std::make_unique<Expression>(std::move(if_true)),
            std::make_unique<Expression>(std::move(if_false))
        } { }

    If(const If& other) :
        cond { std::make_unique<Expression>(*other.cond) },
        if_true { std::make_unique<Expression>(*other.if_true) },
        if_false { std::make_unique<Expression>(*other.if_false) } { }

    If(If&& other) noexcept = default;

    If& operator=(const If& other) {

        cond = std::make_unique<Expression>(*other.cond);
        if_true = std::make_unique<Expression>(*other.if_true);
        if_false = std::make_unique<Expression>(*other.if_false);

        return *this;
    }

    If& operator=(If&& other) noexcept = default;

    const Expression& condition() const { return *cond; }
    const Expression& true_branch() const { return *if_true; }
    const Expression& false_branch() const { return *if_false; }

    bool has_else() const { return if_false != nullptr; }
};


}  // namespace ast


#endif  // AST_IF_HPP
