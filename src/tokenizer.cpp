#include "tokenizer.hpp"
#include "token.hpp"

#include <algorithm>
#include <cctype>
#include <array>
#include <optional>
#include <stdexcept>


namespace {

bool is_id_begin(const char c) {
    return std::isalpha(c) or c == '_';
}

bool is_id_char(const char c) {
    return is_id_begin(c) or std::isdigit(c);
}

bool is_id_quote(const char c) {
    return c == '`';
}

constexpr std::array operator_chars {
    // Assignment
    '=',

    // Comparison
    '<', '>',

    // Arithmetic
    '+', '-', '*', '/', '%', 

    // Optional
    '?', '!',

    // Bitwise
    '&', '|', '^', '~',

};

bool is_operator_char(const char c) {
    return std::ranges::find(operator_chars, c) != operator_chars.end();
}

bool is_quote(const char c) {
    return c == '\'' or c == '"';
}

constexpr std::array brace_chars {
    // Scope
    '{', '}',

    // Apply/Index
    '(', ')',

    // Template/Array
    '[', ']',
};

bool is_brace(const char c) {
    return std::ranges::find(brace_chars, c) != brace_chars.end();
}

bool is_sep(const char c) {
    return c == ',';
}

bool is_space(const char c) {
    return c != '\n' and std::isspace(c);
}

bool is_newline(const char c) {
    return c == '\n';
}

bool is_number_begin(const char c) {
    return std::isdigit(c) or c == '.';
}

bool is_number_char(const char c) {
    return is_number_begin(c) or std::isalpha(c) or c == '#' or c == '_';
}

bool is_member_access_char(const char c) {
    return c == '.' or c == ':';
}

bool is_type_decl_char(const char c) {
    return c == ':';
}

}


Tokenizer::iterator Tokenizer::current_iter() {
    return current.iter;
}


void Tokenizer::advance_state(State& state) {
    if (*(state.iter) == '\n') {
        state.pos.x = 0;
        state.pos.y += 1;
    } else {
        state.pos.x += 1;
    }
    state.pos.byte += 1;

    state.iter += 1;
}


Token Tokenizer::consume_current_token() {
    Token tok {
        std::string_view { token_start.iter, current.iter },
        SourcePosition { current.pos.byte, current.pos.x, current.pos.y }
    };

    token_start = current;

    return tok;
}


std::optional<Token> Tokenizer::get_id() {

    if (not is_id_begin(*current_iter())) {
        return std::nullopt;
    }

    while (has_char() and is_id_char(*current_iter())) {
        advance_state(current);
    }

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_quoted_id() {

    if (not is_id_quote(*current_iter())) {
        return std::nullopt;
    }

    do {
        advance_state(current);
    } while (has_char() and not is_id_quote(*current_iter()));

    if (has_char()) {
        advance_state(current);
    }

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_operator() {
    
    if (not is_operator_char(*current_iter())) {
        return std::nullopt;
    }

    while (has_char() and is_operator_char(*current_iter())) {
        advance_state(current);
    }

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_quote() {

    if (not is_quote(*current_iter())) {
        return std::nullopt;
    }

    char quote = *current_iter();

    // TODO: Escape sequences
    while (has_char() and *current_iter() != quote) {
        advance_state(current);
    }

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_brace() {

    if (not is_brace(*current_iter())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_sep() {

    if (not is_sep(*current_iter())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_newline() {

    if (not is_newline(*current_iter())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_space() {

    if (not is_space(*current_iter())) {
        return std::nullopt;
    }

    while (has_char() and is_space(*current_iter())) {
        advance_state(current);
    }

    return consume_current_token();
}


std::optional<Token> Tokenizer::get_number() {
    // TODO: Implement
    return std::nullopt;
}


std::optional<Token> Tokenizer::get_member_access() {

    if (not is_member_access_char(*current_iter())) {
        return std::nullopt;
    }

    advance_state(current);

    if (*(token_start.iter) == '.') {
        return consume_current_token();
    }

    if (has_char() and *current_iter() == ':') {
        advance_state(current);
        return consume_current_token();
    }

    return std::nullopt;
}


std::optional<Token> Tokenizer::get_type_decl() {

    if (not is_type_decl_char(*current_iter())) {
        return std::nullopt;
    }

    advance_state(current);
    return consume_current_token();
}


std::optional<Token> Tokenizer::get_invalid() {
    advance_state(current);
    return consume_current_token();
}


bool Tokenizer::has_char() {
    return token_start.iter != end;
}


Token Tokenizer::get_next() {

    for (auto parser : parsers) {
        current = token_start;
        auto resp = (this->*parser)();

        if (resp.has_value()) {
            return *resp;
        }
    }


    throw std::runtime_error("Unreachable");
}


void Tokenizer::process_token() {
    tokens.emplace_back(get_next());
}


Tokenized Tokenizer::tokenize() {

    while (has_char()) {
        process_token();
    }
    
    return { filename, std::move(tokens) };
}


Tokenized Tokenizer::tokenize(const File& file) {
    return Tokenizer { file }.tokenize();
}

