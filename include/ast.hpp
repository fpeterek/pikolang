#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "errors.hpp"

namespace ast {


class Identifier {
    std::string_view ident;

public:
    Identifier(std::string_view identifier) :
        ident { identifier }
        { }

    Identifier(Identifier&& other) = default;

    Identifier& operator=(const Identifier& other) = default;
    Identifier& operator=(Identifier&& other) = default;

    std::string_view identifier() const { return ident; }
};

class ScopedIdentifier {
    std::vector<Identifier> idents;

public:
    ScopedIdentifier(std::vector<Identifier> identifiers) :
        idents { std::move(identifiers) }
        { }

    ScopedIdentifier(ScopedIdentifier&& other) = default;

    ScopedIdentifier& operator=(const ScopedIdentifier& other) = default;
    ScopedIdentifier& operator=(ScopedIdentifier&& other) = default;

    const std::vector<Identifier>& identifiers() const { return idents; }
};

class Import {

    std::string_view lang;
    ScopedIdentifier tgt;
    std::string_view tgt_name;
    
public:
    Import(std::string_view lang, ScopedIdentifier target, std::string_view as = "") :
        lang { lang },
        tgt { std::move(target) },
        tgt_name { as } { }

    Import(Import&& other) = default;

    Import& operator=(const Import& other) = default;
    Import& operator=(Import&& other) = default;

    std::string_view language() const { return lang; }
    const ScopedIdentifier& target() const { return tgt; }
    std::string_view as() const { return tgt_name; }

    bool has_alias() const { return not tgt_name.empty(); }
};

class Invalid {
    std::string_view tok;

public:

    Invalid(std::string_view token) :
        tok { token } { }

    Invalid(Invalid&& other) = default;

    Invalid& operator=(const Invalid& other) = default;
    Invalid& operator=(Invalid&& other) = default;

    std::string_view token() const { return tok; }
};

class Empty {
public:
    Empty() noexcept = default;
    Empty(Empty&& other) = default;
    Empty& operator=(const Empty& other) = default;
    Empty& operator=(Empty&& other) = default;
};

using Statement = std::variant<Import, Invalid, Empty>;


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


} // namespace ast

#endif // AST_HPP
