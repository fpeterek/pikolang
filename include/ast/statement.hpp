#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include <variant>

#include "ast/import.hpp"
#include "ast/invalid.hpp"
#include "ast/empty.hpp"
#include "ast/variable.hpp"
#include "ast/function_def.hpp"
#include "ast/expression.hpp"


namespace ast {


using Statement =
    std::variant<
        Import,
        Invalid,
        Empty,
        Variable,
        FunctionDef,
        Expression
    >;


}  // namespace ast


#endif  // AST_STATEMENT_HPP
