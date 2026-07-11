#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <variant>
#include <vector>

#include "errors.hpp"

class Identifier {
    std::string ident;

public:
    Identifier(std::string identifier) :
        ident { std::move(identifier) }
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

    std::string lang;
    ScopedIdentifier tgt;
    std::string tgt_name;
    
public:
    Import(std::string lang, ScopedIdentifier target, std::string as = "") :
        lang { std::move(lang) },
        tgt { std::move(target) },
        tgt_name { std::move(as) } { }

    Import(Import&& other) = default;

    Import& operator=(const Import& other) = default;
    Import& operator=(Import&& other) = default;

    std::string_view language() const { return lang; }
    const ScopedIdentifier& target() const { return tgt; }
    std::string_view as() const { return tgt_name; }
};


using Statement = std::variant<Import>;


class AST {
    std::vector<Statement> stmts;
    std::vector<Error> errs;

public:

    AST(std::vector<Statement> statements, std::vector<Error> errors) :
        stmts { std::move(statements) },
        errs { std::move(errors) } { }

    const std::vector<Statement>& statements() { return stmts; }
    const std::vector<Error>& errors() { return errs; }
};

#endif // AST_HPP
