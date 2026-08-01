#ifndef AST_FUNTION_DEF_HPP
#define AST_FUNTION_DEF_HPP

#include <vector>

#include "ast/identifier.hpp"
#include "ast/data_type.hpp"
#include "ast/expression.hpp"
#include "ast/variable.hpp"


namespace ast {


class FunctionDef {

    Identifier fn_name;
    DataType ret_type;
    std::vector<Variable> args;
    Expression fn_body;

public:

    FunctionDef(Identifier fn_name, DataType ret_type,
                std::vector<Variable> args, Expression body) :
        fn_name { std::move(fn_name) },
        ret_type { std::move(ret_type) },
        args { std::move(args) },
        fn_body { std::move(body) } { }

    FunctionDef(FunctionDef&& other) noexcept = default;

    FunctionDef& operator=(FunctionDef&& other) noexcept = default;

    const Identifier& identifier() const { return fn_name; }
    std::string_view name() const { return fn_name.identifier(); }

    const DataType& return_type() const { return ret_type; }

    const std::vector<Variable>& arguments() const { return args; }

    const Expression& body() const { return fn_body; }
};


}  // namespace ast


#endif  // AST_FUNTION_DEF_HPP
