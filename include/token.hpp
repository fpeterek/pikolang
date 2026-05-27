#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string_view>
#include <tuple>

class SourcePosition {

    size_t src_byte;
    size_t src_x;
    size_t src_y;

public:
    SourcePosition(size_t byte, size_t x, size_t y) :
        src_byte { byte },
        src_x { x },
        src_y { y } { }

    SourcePosition(const SourcePosition& other) = default;

    size_t x() const { return src_x; }
    size_t y() const { return src_y; };

    size_t line() const { return src_y; };
    size_t col() const { return src_x; };

    size_t byte() const { return src_byte; };
};

namespace std {
    template<>
    struct tuple_element<0, SourcePosition> {
        using type = size_t;
    };

    template<>
    struct tuple_element<1, SourcePosition> {
        using type = size_t;
    };

    template<>
    struct tuple_size<SourcePosition> {
        size_t value = 2;
    };

    template<size_t idx>
    size_t get(const SourcePosition& sp) {
        if constexpr (idx == 0) {
            return sp.x();
        } else if constexpr (idx == 1) {
            return sp.y();
        } else {
            static_assert(idx < 2, "Index must be <2");
        }
    }

}


enum struct TokenType {
    Id,
    QuotedId,
    Operator,
    Quote,
    Brace,
    Sep,
    Newline,
    Space,
    Integer,
    Float,
    MemberAccess,
    TypeDecl,
    Invalid,
};


class Token {

    std::string_view tok;
    SourcePosition src;
    TokenType tok_type;

public:

    Token(std::string_view token, SourcePosition source, TokenType type) :
        tok { token },
        src { source },
        tok_type { type } { }

    Token(const Token& other) = default;


    std::string_view token() const { return tok; }
    SourcePosition source() const { return src; }
    TokenType type() const { return tok_type; }

};

#endif  // TOKEN_HPP
