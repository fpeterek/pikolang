#ifndef AST_FN_CALL_HPP
#define AST_FN_CALL_HPP

#include "ast/scoped_identifier.hpp"
#include "ast/expression.hpp"
#include "collections/array.hpp"


namespace ast {


class FnCall {

    ScopedIdentifier fn;
    Array<Expression> args;

public:

    FnCall(ScopedIdentifier id, Array<Expression> args) :
        fn { std::move(id) },
        args { std::move(args) } { }

    FnCall(FnCall&& other) noexcept = default;
    FnCall(const FnCall& other) noexcept = default;

    FnCall& operator=(FnCall&& other) noexcept = default;
    FnCall& operator=(const FnCall& other) noexcept = default;

    const ScopedIdentifier& function() const { return fn; }
    const Array<Expression>& arguments() const { return args; }
};


}  // namespace ast


#endif  // AST_FN_CALL_HPP
