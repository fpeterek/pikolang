#include "parser.hpp"
#include "ast.hpp"
#include "errors.hpp"
#include "token.hpp"
#include <format>
#include <optional>
#include <vector>


namespace parser {

namespace {

bool is_space(const Token& token) {
    return token.type() == TokenType::Space;
}

bool is_import(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == "import";
}

bool is_identifier(const Token& token) {
    return token.type() == TokenType::Id;
}

void advance(parser::Context& ctx) {
    ctx.current += 1;
}

void skip_spaces(parser::Context& ctx) {
    while (is_space(ctx.token())) {
        advance(ctx);
    }
}

void consume_import(parser::Context& ctx) {
    advance(ctx);
}

std::optional<ScopedIdentifier> parse_scoped_identifier(parser::Context ctx, Result& result) {
    if (not is_identifier(ctx.token())) {

        result.errors.emplace_back(Error {
            std::format("Invalid token '{}'", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        });

        return std::nullopt;
    }

    return std::nullopt;
}

Result parse_import(parser::Context ctx) {
    if (not is_import(ctx.token())) {
        return Result {};
    }

    Result result { };

    consume_import(ctx);
    skip_spaces(ctx);

    auto ident = parse_scoped_identifier(ctx, result);

    if (not result) {
        return result;
    }

    return Result {};
}

}

Result Parser::parse_import() {
    return parser::parse_import(create_context());
}


Context Parser::create_context() {
    return Context {
        tokenized.file(),
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
