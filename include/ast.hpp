#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <variant>
#include <vector>

#include "errors.hpp"


class Import {

    std::string lang;
    std::string tgt;
    
public:
    Import(std::string lang, std::string target) :
        lang { std::move(lang) },
        tgt { std::move(target) } { }

    std::string_view language() { return lang; }
    std::string_view target() { return tgt; }
};


using Statement = std::variant<Import>;


class AST {
    std::vector<Statement> stmts;
    std::vector<Error> errs;

public:
    const std::vector<Statement>& statements() { return stmts; }
    const std::vector<Error>& errors() { return errs; }
};

#endif // AST_HPP
