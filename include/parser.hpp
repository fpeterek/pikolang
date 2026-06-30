#ifndef PARSER_HPP
#define PARSER_HPP

#include "tokenizer.hpp"

#include <array>
#include <optional>
#include <variant>

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

class Parser {

    const Tokenized& tokenized;

    using Iterator = decltype(tokenized.tokens().begin());

    Iterator expr_start;
    Iterator current;

    std::string_view filename() { return tokenized.file(); }
    auto& tokens() { return tokenized.tokens(); }

    
    Parser(const Tokenized& tokenized) :
        tokenized { tokenized },
        expr_start { tokenized.tokens().begin() },
        current { tokenized.tokens().begin() }
        { }

    std::optional<Import> parse_import();

    static constexpr std::array parsers {
        &Parser::parse_import,
    };

    AST parse();

public:

    static AST parse(const Tokenized& tokenized);

};


#endif // PARSER_HPP
