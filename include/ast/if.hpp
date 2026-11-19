#ifndef AST_IF_HPP
#define AST_IF_HPP

#include <variant>

#include "ast/expression.hpp"


namespace  ast {


class If {

    Expression cond;
    Expression if_true;
    Expression if_false;

public:

    If(Expression cond,
       Expression if_true,
       Expression if_false = std::monostate { }) :
        cond { std::move(cond) },
        if_true { std::move(if_true) },
        if_false { std::move(if_false) } { }


    If(If&& other) noexcept = default;

    If& operator=(If&& other) noexcept = default;

    Expression condition() const { return cond; }
    Expression true_branch() const { return if_true; }
    Expression false_branch() const { return if_false; }

    bool has_else() const {
        return not std::holds_alternative<std::monostate>(if_false);
    }
};


}  // namespace ast


#endif  // AST_IF_HPP
