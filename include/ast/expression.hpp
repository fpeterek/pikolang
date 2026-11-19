#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include "ast/scoped_identifier.hpp"
#include <variant>


namespace ast {


class If;
class FnCall;
class Literal;
class Body;
class ScopedIdentifier;


using Expression = std::variant<std::monostate, If*, FnCall*, Literal*, ScopedIdentifier*, Body*>;


}  // namespace ast


#endif  // AST_EXPRESSION_HPP
