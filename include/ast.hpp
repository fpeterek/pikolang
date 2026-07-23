#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string_view>
#include <variant>
#include <vector>
#include <optional>

#include "errors.hpp"


namespace ast {


class Identifier {
    std::string_view ident;

public:
    Identifier(std::string_view identifier) :
        ident { identifier }
        { }

    Identifier(const Identifier& other) = default;
    Identifier(Identifier&& other) noexcept = default;

    Identifier& operator=(const Identifier& other) = default;
    Identifier& operator=(Identifier&& other) noexcept = default;

    std::string_view identifier() const { return ident; }
};


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


class Import {

    std::string_view lang;
    ScopedIdentifier tgt;
    std::string_view tgt_name;
    
public:
    Import(std::string_view lang, ScopedIdentifier target, std::string_view as = "") :
        lang { lang },
        tgt { std::move(target) },
        tgt_name { as } { }

    Import(Import&& other) noexcept = default;

    Import& operator=(const Import& other) = default;
    Import& operator=(Import&& other) noexcept = default;

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

    Invalid(Invalid&& other) noexcept = default;

    Invalid& operator=(const Invalid& other) = default;
    Invalid& operator=(Invalid&& other) noexcept = default;

    std::string_view token() const { return tok; }
};


class Empty {
public:
    Empty() noexcept = default;
    Empty(Empty&& other) noexcept = default;
    Empty& operator=(const Empty& other) = default;
    Empty& operator=(Empty&& other) noexcept = default;
};


class If;
class FnCall;

class Literal {
public:

    enum Type {
        Char,
        Int,
        Float,
        String
    };

private:

    std::string_view lit_value;
    Type lit_type;

public:

    Literal(std::string_view value, Type type) :
        lit_value { value },
        lit_type { type } { }

    Literal() noexcept = default;

    Literal(Literal&& other) noexcept = default;
    Literal(const Literal& other) = default;

    Literal& operator=(Literal&& other) noexcept = default;
    Literal& operator=(const Literal& other) = default;

    Type type() const { return lit_type; };
    bool is_char() const { return lit_type == Char; }
    bool is_int() const { return lit_type == Int; }
    bool is_float() const { return lit_type == Float; }
    bool is_string() const { return lit_type == String; }

    std::string_view value() const { return lit_value; }

};

// using Expression = std::variant<If, FnCall, Literal, ScopedIdentifier>;
using Expression = std::variant<FnCall>;

class If {

    std::unique_ptr<Expression> cond;
    std::unique_ptr<Expression> if_true;
    std::unique_ptr<Expression> if_false;

public:

    If(std::unique_ptr<Expression> cond,
       std::unique_ptr<Expression> if_true,
       std::unique_ptr<Expression> if_false = nullptr) :
        cond { std::move(cond) },
        if_true { std::move(if_true) },
        if_false { std::move(if_false) } { }

    If(Expression cond, Expression if_true) :
        If {
            std::make_unique<Expression>(std::move(cond)),
            std::make_unique<Expression>(std::move(if_true))
        } { }

    If(Expression cond, Expression if_true, Expression if_false) :
        If {
            std::make_unique<Expression>(std::move(cond)),
            std::make_unique<Expression>(std::move(if_true)),
            std::make_unique<Expression>(std::move(if_false))
        } { }

    If(If&& other) noexcept = default;

    If(const If& other) :
        cond { std::make_unique<Expression>(other.condition()) },
        if_true { std::make_unique<Expression>(other.true_branch()) },
        if_false { nullptr } {

        if (other.has_else()) {
            if_false = std::make_unique<Expression>(other.false_branch());
        }
    }

    If& operator=(If&& other) noexcept = default;

    If& operator=(const If& other) {
        cond = std::make_unique<Expression>(other.condition());
        if_true = std::make_unique<Expression>(other.true_branch());

        if (other.has_else()) {
            if_false = std::make_unique<Expression>(other.false_branch());
        } else {
            if_false = nullptr;
        }

        return *this;
    }

    const Expression& condition() const { return *cond; }
    const Expression& true_branch() const { return *if_true; }
    const Expression& false_branch() const { return *if_false; }

    bool has_else() const { return if_false != nullptr; }
};

class FnCall {

    ScopedIdentifier fn;
    std::vector<Expression> args;

public:

    FnCall(ScopedIdentifier id, std::vector<Expression> args) :
        fn { std::move(id) },
        args { std::move(args) } { }

    FnCall(FnCall&& other) noexcept = default;
    FnCall(const FnCall& other) noexcept = default;

    FnCall& operator=(FnCall&& other) noexcept = default;
    FnCall& operator=(const FnCall& other) noexcept = default;

    const ScopedIdentifier& function() const { return fn; }
    const std::vector<Expression>& arguments() const { return args; }
};


class DataType {
public:

    enum Mut {
        Mutable,
        Const
    };

    enum Ref {
        Reference,
        Pointer,
        Value
    };

private:

    ScopedIdentifier type_name;
    Mut mut;
    Ref ref;

public:

    DataType(const DataType& other) = default;
    DataType(DataType&& other) noexcept = default;

    DataType& operator=(const DataType& other) = default;
    DataType& operator=(DataType&& other) noexcept = default;

    const ScopedIdentifier& type() const {
        return type_name;
    }

    Mut mutability() const { return mut; }
    bool is_mutable() const { return mut == Mut::Mutable; }
    bool is_const() const { return mut == Mut::Const; }

    Ref reference_status() const { return ref; }
    bool is_ref() const { return ref == Ref::Reference; }
    bool is_pointer() const { return ref == Ref::Pointer; }
    bool is_value() const { return ref == Ref::Value; }
    bool is_referential() const { return not is_value(); }
};


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

    Variable& operator=(const Variable& other) = default;
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
    FunctionDef(const FunctionDef& other) = default;

    FunctionDef& operator=(FunctionDef&& other) noexcept = default;
    FunctionDef& operator=(const FunctionDef& other) = default;

    const Identifier& identifier() const { return fn_name; }
    std::string_view name() const { return fn_name.identifier(); }

    const DataType& return_type() const { return ret_type; }

    const std::vector<Variable>& arguments() const { return args; }

    const Expression& body() const { return fn_body; }
};


using Statement = std::variant<Import, Invalid, Empty, Variable, FunctionDef, Expression>;


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
