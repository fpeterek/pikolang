#ifndef AST_IDENTIFIER_HPP
#define AST_IDENTIFIER_HPP


#include <string_view>


namespace ast {


class Identifier {
    std::string_view ident;

public:
    Identifier(std::string_view identifier) :
        ident { identifier }
        { }

    std::string_view identifier() const { return ident; }
};


} // namespace ast


#endif  // AST_IDENTIFIER_HPP
