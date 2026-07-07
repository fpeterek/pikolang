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

    std::string_view identifier() const { return ident; }
};

class ScopedIdentifier {
    std::vector<Identifier> idents;

public:
    ScopedIdentifier(std::vector<Identifier> identifiers) :
        idents { std::move(identifiers) }
        { }

    ScopedIdentifier(ScopedIdentifier&& other) = default;

    const std::vector<Identifier>& identifiers() const { return idents; }
};

class Import {

    std::string lang;
    ScopedIdentifier tgt;
    
public:
    Import(std::string lang, ScopedIdentifier target) :
        lang { std::move(lang) },
        tgt { std::move(target) } { }

    std::string_view language() const { return lang; }
    const ScopedIdentifier& target() const { return tgt; }
};


using Statement = std::variant<Import>;


class AST {
    std::vector<Statement> stmts;
    std::vector<Error> errs;

public:
    const std::vector<Statement>& statements() { return stmts; }
    const std::vector<Error>& errors() { return errs; }
};

#endif // AST_HPP
