#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <array>
#include <string_view>


namespace language {

constexpr std::array operator_chars {
    // Assignment
    '=',

    // Comparison
    '<', '>',

    // Arithmetic
    '+', '-', '*', '/', '%', 

    // Optional
    '?', '!',

    // Bitwise
    '&', '|', '^', '~',

};

constexpr std::array brace_chars {
    // Scope
    '{', '}',

    // Apply/Index
    '(', ')',

    // Template/Array
    '[', ']',
};

namespace operators {
    constexpr std::string_view member_access = "::";
}

namespace keywords {
    constexpr std::string_view import     = "import";
    constexpr std::string_view as         = "as";
    constexpr std::string_view _namespace = "namespace";
    constexpr std::string_view pure       = "pure";
    constexpr std::string_view fn         = "fn";
    constexpr std::string_view let        = "let";
    constexpr std::string_view var        = "var";
    constexpr std::string_view _if        = "if";
    constexpr std::string_view _else      = "else";
    constexpr std::string_view _for       = "for";
    constexpr std::string_view _while     = "while";
    constexpr std::string_view _break     = "break";
    constexpr std::string_view _continue  = "continue";
    constexpr std::string_view _return    = "return";
}

static constexpr std::array keywords_array = {
    keywords::import,
    keywords::as,
    keywords::_namespace,
    keywords::pure,
    keywords::fn,
    keywords::let,
    keywords::var,
    keywords::_if,
    keywords::_else,
    keywords::_for,
    keywords::_while,
    keywords::_break,
    keywords::_continue,
    keywords::_return,
};

}

#endif // LANGUAGE_HPP
