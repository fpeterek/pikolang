#include "parser.hpp"
#include <vector>

void Parser::import_language() {
    import_keyword();
    ignore_space();
    namespace_operator();
    ignore_space();
    import_language_specifier();
}

void Parser::import_no_language() {
    import_keyword();
}

void Parser::import_specifier() {
    // TODO: Express this
    //
    // either
    import_language();
    // or
    import_no_language();
}

std::optional<Import> Parser::parse_import() {

    import_specifier();
    expect_space();
    import_target();
    expect_newline();

}

AST Parser::parse() {

    std::vector<Statement> statements;
    std::vector<Error> errors;

    for (const auto parser : parsers) {
        auto res = (this->*parser)();

        if (res) {
        }
    }
    
    return { };
}


AST Parser::parse(const Tokenized& tokenized) {
    Parser parser { tokenized };   

    return parser.parse();
}
