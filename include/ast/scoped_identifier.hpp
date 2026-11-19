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


    const Array<Identifier>& identifiers() const { return idents; }
};


}  // namespace ast


#endif  // AST_SCOPED_IDENTIFIER_HPP
