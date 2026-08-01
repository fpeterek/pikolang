#ifndef AST_VARIABLE_HPP
#define AST_VARIABLE_HPP

#include <optional>

#include "ast/identifier.hpp"
#include "ast/expression.hpp"
#include "ast/data_type.hpp"


namespace ast {


class Variable {

    std::optional<Expression> initial_val;
    DataType var_type;
    Identifier var_name;

public:

    Variable(Identifier arg_name, DataType arg_type) :
        initial_val { std::nullopt },
        var_type { arg_type },
        var_name { std::move(arg_name) } { }
        
    Variable(Identifier arg_name, DataType arg_type, Expression default_val) :
        initial_val { std::move(default_val) },
        var_type { arg_type },
        var_name { std::move(arg_name) } { }

    Variable(Variable&& other) noexcept = default;

    Variable& operator=(Variable&& other) noexcept = default;

    bool is_mutable() const { return var_type.is_mutable(); }
    bool is_const() const { return var_type.is_mutable(); }

    bool is_pointer() const { return var_type.is_pointer(); }
    bool is_ref() const { return var_type.is_ref(); }
    bool is_referential() const { return var_type.is_referential(); }
    bool is_value() const { return var_type.is_value(); }

    const Identifier& identifier() const { return var_name; }
    std::string_view name() const { return var_name.identifier(); }

    bool has_initial() const { return initial_val.has_value(); }
    
    const Expression& initial_value() const { return *initial_val; }
};


}  // namespace ast


#endif  // AST_VARIABLE_HPP
