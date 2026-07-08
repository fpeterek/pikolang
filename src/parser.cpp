#include "parser.hpp"

#include <format>
#include <optional>
#include <vector>

#include <experimental/scope>

#include "ast.hpp"
#include "errors.hpp"
#include "token.hpp"


namespace parser {

namespace {

bool is_space(const Token& token) {
    return token.type() == TokenType::Space;
}

bool is_newline(const Token& token) {
    return token.type() == TokenType::Newline;
}

bool is_import(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == "import";
}

bool is_identifier(const Token& token) {
    return token.type() == TokenType::Id;
}

bool is_scope_access(const Token& token) {
    return token.type() == TokenType::MemberAccess and
        token.token() == "::";
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

std::optional<Identifier> parse_identifier(parser::Context ctx, Result& result) {

    if (not is_identifier(ctx.token())) {
        result.errors.emplace_back(Error {
            std::format("Invalid token '{}', identifier expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source(),
        });

        return std::nullopt;
    }

    std::string id { ctx.token().token() };
    advance(ctx);

    return Identifier {
        std::move(id)
    };
}

bool parse_scope_operator(parser::Context ctx, Result& result) {
    skip_spaces(ctx);

    if (not is_scope_access(ctx.token())) {
        result.errors.emplace_back(Error {
            std::format("Invalid token '{}'", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        });

        return false;
    }
    advance(ctx);

    skip_spaces(ctx);

    return true;
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

    std::vector<Identifier> identifiers;

    auto ident = parse_identifier(ctx, result);

    if (not result or not ident) {
        return std::nullopt;
    }

    identifiers.emplace_back(std::move(*ident));

    while (parse_scoped_identifier(ctx, result)) {
        auto member = parse_identifier(ctx, result);

        if (not result or not member) {
            return std::nullopt;
        }

        identifiers.emplace_back(std::move(*member));
    }

    return ScopedIdentifier {
        std::move(identifiers)
    };
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
