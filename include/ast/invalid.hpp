#ifndef AST_INVALID_HPP
#define AST_INVALID_HPP

#include <string_view>


namespace ast {


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


}

#endif  // AST_INVALID_HPP
