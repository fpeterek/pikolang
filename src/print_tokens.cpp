#include "print_tokens.hpp"

#include <print>
#include <string_view>


namespace {

namespace colors {
    using Color = std::string_view;

    constexpr std::string_view standard = "\x1b[38;5;251m";
    constexpr std::string_view identifier = "\x1b[38;5;222m";
    constexpr std::string_view whitespace = "\x1b[38;5;244m";
    constexpr std::string_view operator_ = "\x1b[38;5;39m";
    constexpr std::string_view brace = "\x1b[38;5;140m";
    constexpr std::string_view quote = "\x1b[38;5;48m";
    constexpr std::string_view integer = "\x1b[38;5;224m";
    constexpr std::string_view floating = "\x1b[38;5;216m";
    constexpr std::string_view member_access = "\x1b[38;5;69m";
    constexpr std::string_view type_decl = "\x1b[38;5;210m";

    constexpr std::string_view invalid = "\x1b[38;5;196m";
}

class Printer {

    size_t nesting = 0;
    bool is_on_new_line = true;

    void print_offset();

    void basic_print_no_offset(colors::Color color, std::string_view string);

    void basic_print(colors::Color color, std::string_view string);
    void basic_print(colors::Color color, const Token& tok);

    void print_id(const Token& tok);
    void print_quoted_id(const Token& tok);
    void print_operator(const Token& tok);
    void print_quote(const Token& tok);
    void print_brace(const Token& tok);
    void print_sep(const Token& tok);
    void print_newline(const Token& tok);
    void print_space(const Token& tok);
    void print_integer(const Token& tok);
    void print_float(const Token& tok);
    void print_member_access(const Token& tok);
    void print_type_decl(const Token& tok);
    void print_invalid(const Token& tok);

    void print_impl(const std::vector<Token>& tokens);

    Printer() noexcept = default;
    Printer(const Printer&) noexcept = delete;
    Printer(Printer&&) noexcept = default;
public:

    static void print(const std::vector<Token>& tokens);

};


void Printer::print_offset() {
    if (not is_on_new_line) {
        return;
    }

    for (size_t i = 0; i < nesting; ++i) {
        std::print("    ");
    }

    is_on_new_line = false;
}

void Printer::basic_print_no_offset(colors::Color color, std::string_view string) {
    std::println("{}{}", color, string);
}

void Printer::basic_print(colors::Color color, std::string_view string) {
    print_offset();
    basic_print_no_offset(color, string);
}

void Printer::basic_print(colors::Color color, const Token& tok) {
    basic_print(color, tok.token());
}


void Printer::print_id(const Token& tok) {
    basic_print(colors::identifier, tok);
}

void Printer::print_quoted_id(const Token& tok) {
    print_id(tok);
}

void Printer::print_operator(const Token& tok) {
    basic_print(colors::operator_, tok);
}

void Printer::print_quote(const Token& tok) {
    basic_print(colors::quote, tok);
}

void Printer::print_brace(const Token& tok) {

    if (tok.token() == "}") {
        nesting = std::max(size_t{0}, nesting-1);
    }

    print_offset();

    if (tok.token() == "{") {
        nesting += 1;
    }

    basic_print_no_offset(colors::brace, tok.token());
}

void Printer::print_sep([[maybe_unused]] const Token& tok) {
    basic_print(colors::standard, ", ");
}

void Printer::print_newline([[maybe_unused]] const Token& tok) {
    basic_print(colors::whitespace, "\\n");
    is_on_new_line = true;
}

void Printer::print_space([[maybe_unused]] const Token& tok) {

    if (is_on_new_line) {
        return;
    }

    basic_print_no_offset(colors::whitespace, "·");
}

void Printer::print_integer(const Token& tok) {
    basic_print(colors::integer, tok);
}

void Printer::print_float(const Token& tok) {
    basic_print(colors::floating, tok);
}

void Printer::print_member_access(const Token& tok) {
    basic_print(colors::member_access, tok);
}

void Printer::print_type_decl(const Token& tok) {
    basic_print(colors::type_decl, tok);
}

void Printer::print_invalid(const Token& tok) {
    basic_print(colors::invalid, tok);
}


void Printer::print_impl(const std::vector<Token>& tokens) {

    for (const Token& tok : tokens) {
        switch (tok.type()) {
            case TokenType::Id:           return print_id(tok);
            case TokenType::QuotedId:     return print_quoted_id(tok);
            case TokenType::Operator:     return print_operator(tok);
            case TokenType::Quote:        return print_quote(tok);
            case TokenType::Brace:        return print_brace(tok);
            case TokenType::Sep:          return print_sep(tok);
            case TokenType::Newline:      return print_newline(tok);
            case TokenType::Space:        return print_space(tok);
            case TokenType::Integer:      return print_integer(tok);
            case TokenType::Float:        return print_float(tok);
            case TokenType::MemberAccess: return print_member_access(tok);
            case TokenType::TypeDecl:     return print_type_decl(tok);
            case TokenType::Invalid:      return print_invalid(tok);
        }
    }
    
}

void Printer::print(const std::vector<Token>& tokens) {
    Printer printer{};
    printer.print_impl(tokens);
}

}


void print_tokens(const std::vector<Token>& tokens) {
    Printer::print(tokens);
}
