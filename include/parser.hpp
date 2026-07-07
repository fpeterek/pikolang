#ifndef PARSER_HPP
#define PARSER_HPP

#include <array>
#include <optional>
#include <string_view>

#include "tokenizer.hpp"
#include "errors.hpp"
#include "ast.hpp"


namespace parser {


using iterator = Tokenized::const_iterator;


struct Context {
    std::string_view filename;
    iterator current;
    iterator end;

    const Token& token() const { return *current; }
};


struct Result {
    std::optional<Statement> statement;
    std::vector<Error> errors;
    iterator next;

    bool success() const {
        return statement.has_value();
    }

    operator bool() const {
        return success();
    }
};


class Parser {

    std::vector<Error> errors;
    iterator current;
    const Tokenized& tokenized;

    Context create_context();
    void process_result(Result result);

    std::string_view filename() { return tokenized.file(); }
    auto& tokens() { return tokenized.tokens(); }

    iterator end() { return tokens().end(); };
    
    Parser(const Tokenized& tokenized) :
        current { tokenized.tokens().begin() },
        tokenized { tokenized }
        { }


    Result parse_import();

    static constexpr std::array parsers {
        &Parser::parse_import,
    };

    AST parse();

public:

    static AST parse(const Tokenized& tokenized);

};

}

#endif // PARSER_HPP
