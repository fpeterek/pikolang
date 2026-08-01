#ifndef AST_SCOPED_IDENTIFIER_HPP
#define AST_SCOPED_IDENTIFIER_HPP

#include <vector>

#include "ast/identifier.hpp"


namespace ast {


class ScopedIdentifier {
    std::vector<Identifier> idents;

public:
    ScopedIdentifier(std::vector<Identifier> identifiers) :
        idents { std::move(identifiers) }
        { }

    ScopedIdentifier(const ScopedIdentifier& other) = default;
    ScopedIdentifier(ScopedIdentifier&& other) noexcept = default;

    ScopedIdentifier& operator=(const ScopedIdentifier& other) = default;
    ScopedIdentifier& operator=(ScopedIdentifier&& other) noexcept = default;

    const std::vector<Identifier>& identifiers() const { return idents; }
};


}  // namespace ast


#endif  // AST_SCOPED_IDENTIFIER_HPP
