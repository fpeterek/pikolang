#include "print_ast.hpp"

#include <variant>
#include <print>

#include "ast.hpp"
#include "colors.hpp"


namespace {

void print_identifier(std::string_view identifier) {
    std::print("{}{}", colors::identifier, identifier);
}

void print_identifier(const ast::Identifier& identifier) {
    print_identifier(identifier.identifier());
}

void print_scoped(const ast::ScopedIdentifier& identifier) {
    for (size_t i = 0; i < identifier.identifiers().size(); ++i) {
        print_identifier(identifier.identifiers()[i]);

        if (i < identifier.identifiers().size() - 1) {
            std::print("::", colors::member_access);
        }
    }
}

void print_import(const ast::Import& import) {

    std::print("{}import ", colors::keyword);

    print_scoped(import.target());

    if (import.has_alias()) {
        std::print("{}as ", colors::keyword);
        print_identifier(import.as());
    }

    std::println("{}", colors::standard);
}

void print_invalid(const ast::Invalid& invalid) {
    std::print("{}{}{}", colors::invalid, invalid.token(), colors::standard);
}

void print_statement(const ast::Statement& stmt) {

    if (std::holds_alternative<ast::Import>(stmt)) {
        print_import(std::get<ast::Import>(stmt));
    }

    if (std::holds_alternative<ast::Invalid>(stmt)) {
        print_invalid(std::get<ast::Invalid>(stmt));
    }

}

} // namespace


void print_ast(const ast::AST& ast) {

    for (const auto& stmt : ast.statements()) {
        print_statement(stmt);
    }
    
}
