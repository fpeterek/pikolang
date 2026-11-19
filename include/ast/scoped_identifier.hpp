#ifndef AST_SCOPED_IDENTIFIER_HPP
#define AST_SCOPED_IDENTIFIER_HPP

#include "ast/identifier.hpp"
#include "collections/array.hpp"


namespace ast {


class ScopedIdentifier {
    Array<Identifier> idents;

public:
    ScopedIdentifier(Array<Identifier> identifiers) :
        idents { std::move(identifiers) }
        { }

    ScopedIdentifier(const ScopedIdentifier& other) = default;
    ScopedIdentifier(ScopedIdentifier&& other) noexcept = default;

    ScopedIdentifier& operator=(const ScopedIdentifier& other) = default;
    ScopedIdentifier& operator=(ScopedIdentifier&& other) noexcept = default;

    const Array<Identifier>& identifiers() const { return idents; }
};


}  // namespace ast


#endif  // AST_SCOPED_IDENTIFIER_HPP
