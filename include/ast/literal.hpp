#ifndef AST_LITERAL_HPP
#define AST_LITERAL_HPP


#include <string_view>


namespace ast {


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


}  // namespace ast


#endif  // AST_LITERAL_HPP
