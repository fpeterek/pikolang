#ifndef AST_AST_HPP
#define AST_AST_HPP

#include "ast/statement.hpp"
#include "collections/array.hpp"
#include "errors.hpp"


namespace ast {


class AST {
    Array<Statement> stmts;
    Array<Error> errs;

public:

    AST(Array<Statement> statements, Array<Error> errors) :
        stmts { std::move(statements) },
        errs { std::move(errors) } { }

    const Array<Statement>& statements() const { return stmts; }
    const Array<Error>& errors() const { return errs; }
};


}  // namespace ast

#endif  // AST_AST_HPP
