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

    const ScopedIdentifier& function() const { return fn; }
    const Array<Expression>& arguments() const { return args; }
};


}  // namespace ast


#endif  // AST_FN_CALL_HPP
