#ifndef AST_IMPORT_HPP
#define AST_IMPORT_HPP


#include <string_view>

#include "ast/scoped_identifier.hpp"


namespace ast {


class Import {

    std::string_view lang;
    ScopedIdentifier tgt;
    std::string_view tgt_name;
    
public:
    Import(std::string_view lang, ScopedIdentifier target, std::string_view as = "") :
        lang { lang },
        tgt { std::move(target) },
        tgt_name { as } { }


    std::string_view language() const { return lang; }
    const ScopedIdentifier& target() const { return tgt; }
    std::string_view as() const { return tgt_name; }

    bool has_alias() const { return not tgt_name.empty(); }
};


}  // namespace ast

#endif  // AST_IMPORT_HPP
