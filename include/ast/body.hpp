#ifndef AST_BODY_HPP
#define AST_BODY_HPP

#include "ast/statement.hpp"


namespace  ast {

class Body {

    std::vector<ast::Statement> statements;

public:

    Body() noexcept = default;

    Body(std::vector<ast::Statement> statements) :
        statements { std::move(statements) } { }


    Body(const Body& other) = default;
    Body(Body&& other) noexcept = default;

    Body& operator=(const Body& other) = default;
    Body& operator=(Body&& other) noexcept = default;

    const std::vector<ast::Statement>& body() const { return statements; }
};


}  // namespace ast


#endif  // AST_BODY_HPP
