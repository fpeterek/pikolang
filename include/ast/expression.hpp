#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include <type_traits>
#include <cstdint>
#include <variant>

#include "ast/scoped_identifier.hpp"
#include "ast/scoped_identifier.hpp"


namespace ast {


class If;
class FnCall;
class Literal;


template<typename T>
concept ExpressionType =
    std::is_same_v<T, If> or
    std::is_same_v<T, FnCall> or
    std::is_same_v<T, Literal> or
    std::is_same_v<T, ScopedIdentifier>;


// using Expression = std::variant<If, FnCall, Literal, ScopedIdentifier>;


class Expression {

    constexpr static uint64_t empty_tag = 0b0000;
    constexpr static uint64_t if_tag    = 0b0001;
    constexpr static uint64_t call_tag  = 0b0010;
    constexpr static uint64_t lit_tag   = 0b0100;
    constexpr static uint64_t id_tag    = 0b1000;

    union Ptr {
        std::monostate empty;
        If* _if;
        FnCall* call;
        Literal* lit;
        ScopedIdentifier* id;
    };

    Ptr ptr;
    uint64_t tag = empty_tag;

    void assign_from(const Expression& other);
    void delete_data();

public:

    Expression() noexcept :
        ptr { .empty = std::monostate {} } { }

    Expression(const Expression& other);
    Expression& operator=(const Expression& other);

    Expression& operator=(Expression&& other) {
        delete_data();

        // Initialize union members explicitely to avoid any UB
        if (holds<If>()) {
            ptr._if = other.ptr._if;
            tag = if_tag;
        }
        else if (holds<FnCall>()) {
            ptr.call = other.ptr.call;
            tag = call_tag;
        }
        else if (holds<Literal>()) {
            ptr.lit = other.ptr.lit;
            tag = lit_tag;
        }
        else if (holds<ScopedIdentifier>()) {
            ptr.id = other.ptr.id;
            tag = id_tag;
        }

        other.tag = empty_tag;
        other.ptr.empty = std::monostate{};
        
        return *this;
    }

    Expression(Expression&& other) noexcept {
        // Initialize union members explicitely to avoid any UB
        if (holds<If>()) {
            ptr._if = other.ptr._if;
            tag = if_tag;
        }
        else if (holds<FnCall>()) {
            ptr.call = other.ptr.call;
            tag = call_tag;
        }
        else if (holds<Literal>()) {
            ptr.lit = other.ptr.lit;
            tag = lit_tag;
        }
        else if (holds<ScopedIdentifier>()) {
            ptr.id = other.ptr.id;
            tag = id_tag;
        }

        other.tag = empty_tag;
        other.ptr.empty = std::monostate{};
    }

    Expression(If&& _if);
    Expression(FnCall&& call);
    Expression(Literal&& lit);
    Expression(ScopedIdentifier&& id);

    ~Expression();

    template<ExpressionType T>
    bool holds() const {
        if constexpr (std::is_same_v<T, If>) {
            return tag == if_tag;
        }
        if constexpr (std::is_same_v<T, FnCall>) {
            return tag == call_tag;
        }
        if constexpr (std::is_same_v<T, Literal>) {
            return tag == lit_tag;
        }
        if constexpr (std::is_same_v<T, ScopedIdentifier>) {
            return tag == id_tag;
        }
    }

    template<ExpressionType T>
    const T& get() const {
        if (not holds<T>()) {
            throw std::bad_variant_access{};
        }
        if constexpr (std::is_same_v<T, If>) {
            return *ptr._if;
        }
        if constexpr (std::is_same_v<T, FnCall>) {
            return *ptr.call;
        }
        if constexpr (std::is_same_v<T, Literal>) {
            return *ptr.lit;
        }
        if constexpr (std::is_same_v<T, ScopedIdentifier>) {
            return *ptr.id;
        }
    }
};


}  // namespace ast


namespace std {
    template<ast::ExpressionType T>
    bool holds_alternative(const ast::Expression& expr) {
        return expr.holds<T>();
    }

    template<ast::ExpressionType T>
    const T& get(const ast::Expression& expr) {
        return expr.get<T>();
    }
} // namespace std


#endif  // AST_EXPRESSION_HPP
