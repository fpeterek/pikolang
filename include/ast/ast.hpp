#ifndef AST_AST_HPP
#define AST_AST_HPP

#include <vector>

#include "ast/statement.hpp"
#include "errors.hpp"


namespace ast {


class AST {
    std::vector<Statement> stmts;
    std::vector<Error> errs;

public:

    AST(std::vector<Statement> statements, std::vector<Error> errors) :
        stmts { std::move(statements) },
        errs { std::move(errors) } { }

    const std::vector<Statement>& statements() const { return stmts; }
    const std::vector<Error>& errors() const { return errs; }
};


}  // namespace ast

#endif  // AST_AST_HPP
