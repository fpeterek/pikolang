#include "parser.hpp"
#include "token.hpp"
#include <vector>


namespace parser {

namespace {

bool is_import(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == "import";
}

Result parse_import(parser::Context ctx) {
    if (not is_import(*ctx.current)) {
        return Result {};
    }
    return Result {};
}

}

Result Parser::parse_import() {
    return parser::parse_import(create_context());
}


Context Parser::create_context() {
    return Context {
        current,
        end(),
    };
}

void Parser::process_result(Result result) {
    if (not result) {
        return;
    }

    for (auto& err : result.errors) {
        errors.emplace_back(std::move(err));
    }

    current = result.next;
}

AST Parser::parse() {
}


AST Parser::parse(const Tokenized& tokenized) {
    Parser parser { tokenized };   

    return parser.parse();
}

}
