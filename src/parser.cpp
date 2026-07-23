#include "parser.hpp"

#include <format>
#include <optional>
#include <vector>
#include <print>

#include "ast.hpp"
#include "colors.hpp"
#include "token.hpp"
#include "language.hpp"


namespace parser {

namespace {

namespace kwd = language::keywords;
namespace ops = language::operators;


bool is_space(const Token& token) {
    return token.type() == TokenType::Space;
}

bool is_newline(const Token& token) {
    return token.type() == TokenType::Newline;
}

bool is_open_paren(const Token& token) {
    return token.type() == TokenType::Brace and token.token() == "(";
}

bool is_closed_paren(const Token& token) {
    return token.type() == TokenType::Brace and token.token() == ")";
}

bool is_arrow(const Token& token) {
    return token.type() == TokenType::Operator and token.token() == "->";
}

bool is_assign_operator(const Token& token) {
    return token.type() == TokenType::Operator and token.token() == "=";
}

bool is_import(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == kwd::import;
}

bool is_as(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == kwd::as;
}

bool is_pure(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == kwd::pure;
}

bool is_fn(const Token& token) {
    return token.type() == TokenType::Keyword and
        token.token() == kwd::fn;
}

bool is_identifier(const Token& token) {
    return token.type() == TokenType::Id;
}

bool is_scope_access(const Token& token) {
    return token.type() == TokenType::MemberAccess and
        token.token() == ops::member_access;
}

void advance(parser::Context& ctx) {
    ctx.current += 1;
}

void skip_empty(parser::Context& ctx) {
    while (is_space(ctx.token()) or is_newline(ctx.token())) {
        advance(ctx);
    }
}

void skip_spaces(parser::Context& ctx) {
    while (is_space(ctx.token())) {
        advance(ctx);
    }
}

void expect_newline(parser::Context& ctx, Result& result) {
    if (not is_newline(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', newline expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return;
    }

    advance(ctx);
}

void expect_open_paren(parser::Context& ctx, Result& result) {
    if (not is_open_paren(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', '(' expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return;
    }

    advance(ctx);
}

void expect_closed_paren(parser::Context& ctx, Result& result) {
    if (not is_closed_paren(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', ')' expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return;
    }

    advance(ctx);
}

void expect_arrow(parser::Context& ctx, Result& result) {
    if (not is_arrow(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', '->' expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return;
    }

    advance(ctx);
}

void expect_assignment(parser::Context& ctx, Result& result) {
    if (not is_assign_operator(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', '=' expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return;
    }

    advance(ctx);
}

void skip_import_keyword(parser::Context& ctx) {
    advance(ctx);
}

void skip_fn_keyword(parser::Context& ctx) {
    advance(ctx);
}

std::optional<ast::Identifier> parse_identifier(parser::Context& ctx, Result& result) {

    skip_spaces(ctx);

    if (not is_identifier(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', identifier expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return std::nullopt;
    }

    std::string_view id { ctx.token().token() };
    advance(ctx);

    return ast::Identifier {
        id
    };
}

bool parse_scope_operator(parser::Context& ctx, Result& result) {
    skip_spaces(ctx);

    if (not is_scope_access(ctx.token())) {
        result.add_error(
            std::format("Invalid token '{}', '{}' expected", ctx.token().token(), ops::member_access),
            ctx.filename,
            ctx.token().source()
        );

        return false;
    }
    advance(ctx);

    skip_spaces(ctx);

    return true;
}

std::optional<ast::ScopedIdentifier> parse_scoped_identifier(parser::Context& ctx, Result& result) {
    skip_spaces(ctx);

    if (not is_identifier(ctx.token())) {

        result.add_error(
            std::format("Invalid token '{}', identifier expected", ctx.token().token()),
            ctx.filename,
            ctx.token().source()
        );

        return std::nullopt;
    }

    std::vector<ast::Identifier> identifiers;

    auto ident = parse_identifier(ctx, result);

    if (result.has_errors() or not ident) {
        return std::nullopt;
    }

    identifiers.emplace_back(std::move(*ident));

    while (true) {

        skip_spaces(ctx);
        if (not is_scope_access(ctx.token())) {
            break;
        }
        parse_scope_operator(ctx, result);

        auto member = parse_identifier(ctx, result);

        if (result.has_errors() or not member) {
            return std::nullopt;
        }

        identifiers.emplace_back(std::move(*member));
    }

    return ast::ScopedIdentifier {
        std::move(identifiers)
    };
}

std::optional<ast::Identifier> parse_as(parser::Context& ctx, Result& res) {
    skip_spaces(ctx);

    if (not is_as(ctx.token())) {
        return std::nullopt;
    }

    advance(ctx);
    skip_spaces(ctx);

    return parse_identifier(ctx, res);
}

Result parse_empty(parser::Context ctx) {
    Result result {
        ast::Empty { },
        {},
        ctx.current,
    };

    skip_spaces(ctx);
    expect_newline(ctx, result);

    if (result) {
        result.next = ctx.current;
    }

    return result;
}

Result parse_invalid(parser::Context ctx) {
    ast::Invalid invalid { ctx.token().token() };
    advance(ctx);

    Result result {
        std::move(invalid),
        {},
        ctx.current
    };

    std::string token;
    token.reserve(ctx.token().token().size());

    for (char c : ctx.token().token()) {
        if (c == '\n') {
            token += "\\n";
        } else if (c == '\r') {
            token += "\\r";
        } else if (c == '\t') {
            token += "\\t";
        } else {
            token += c;
        }
    }

    result.add_error(
        std::format("Unexpected token '{}'", token),
        ctx.filename,
        ctx.token().source()
    );

    return result;
}

Result parse_import(parser::Context ctx) {
    skip_empty(ctx);

    if (not is_import(ctx.token())) {
        std::println("{}not import{}", colors::gray, colors::standard);
        return Result {};
    }

    Result result { };

    skip_import_keyword(ctx);
    skip_spaces(ctx);

    auto target = parse_scoped_identifier(ctx, result);

    if (not target) {
        std::println("{}missing target{}", colors::gray, colors::standard);
        return result;
    }

    skip_spaces(ctx);

    auto as = parse_as(ctx, result);

    if (not as) {
        result.statement = ast::Statement {
            ast::Import {
                "piko",
                std::move(*target),
                "",
            }
        };
        result.next = ctx.current;
        return result;
    }

    std::string_view as_name = "";

    if (as.has_value()) {
        as_name = as.value().identifier();
    }

    expect_newline(ctx, result);


    result.statement = ast::Statement {
        ast::Import {
            "piko",
            std::move(*target),
            std::move(as_name),
        }
    };
    result.next = ctx.current;


    return result;
}

std::optional<ast::Expression> parse_expression(parser::Context& ctx, Result& result) {
    return std::nullopt;
}

Result parse_fn(parser::Context ctx) {
    skip_empty(ctx);

    bool pure = is_pure(ctx.token());

    if (is_pure(ctx.token())) {
        advance(ctx);
        skip_empty(ctx);
    }

    if (not is_fn(ctx.token()) and not pure) {
        return Result { };
    }

    skip_fn_keyword(ctx);

    skip_empty(ctx);

    Result result;

    auto fn_name = parse_identifier(ctx, result);

    if (not fn_name.has_value()) {
        result.add_error(
            "Expecting function name",
            ctx.filename,
            ctx.token().source()
        );
    }

    skip_empty(ctx);

    expect_open_paren(ctx, result);

    // TODO: Parse args
    // parse_fn_args(ctx, result);

    expect_closed_paren(ctx, result);

    skip_empty(ctx);

    expect_arrow(ctx, result);

    skip_empty(ctx);

    auto ret_type = parse_scoped_identifier(ctx, result);

    skip_empty(ctx);

    expect_assignment(ctx, result);

    skip_empty(ctx);

    auto body = parse_expression(ctx, result);

    return result;
}

}

Result Parser::parse_fn() {
    return parser::parse_fn(create_context());
}

Result Parser::parse_import() {
    return parser::parse_import(create_context());
}

Result Parser::parse_empty() {
    return parser::parse_empty(create_context());
}

Result Parser::parse_invalid() {
    return parser::parse_invalid(create_context());
}

Context Parser::create_context() {
    return Context {
        tokenized.file(),
        current,
        end(),
    };
}

void Parser::process_result(Result& result) {
    if (not result.has_statement()) {
        return;
    }

    for (auto& err : result.errors) {
        errors.emplace_back(std::move(err));
    }

    if (result.statement) {
        statements.emplace_back(std::move(*result.statement));
    }

    current = result.next;
}

ast::AST Parser::parse() {

    while (current != end()) {
        for (auto p : top_level_parsers) {
            Result res = (this->*p)();

            process_result(res);

            if (res.has_statement()) {
                break;
            }
        }
    }

    return ast::AST {
        std::move(statements),
        std::move(errors)
    };
}


ast::AST Parser::parse(const Tokenized& tokenized) {
    Parser parser { tokenized };   

    return parser.parse();
}

}
