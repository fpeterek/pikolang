#ifndef AST_FN_CALL_HPP
#define AST_FN_CALL_HPP

#include <vector>

#include "ast/scoped_identifier.hpp"
#include "ast/expression.hpp"


namespace ast {


class FnCall {

    ScopedIdentifier fn;
    std::vector<Expression> args;

public:

    FnCall(ScopedIdentifier id, std::vector<Expression> args) :
        fn { std::move(id) },
        args { std::move(args) } { }

    FnCall(FnCall&& other) noexcept = default;
    FnCall(const FnCall& other) noexcept = default;

    FnCall& operator=(FnCall&& other) noexcept = default;
    FnCall& operator=(const FnCall& other) noexcept = default;

    const ScopedIdentifier& function() const { return fn; }
    const std::vector<Expression>& arguments() const { return args; }
};


}  // namespace ast


#endif  // AST_FN_CALL_HPP
