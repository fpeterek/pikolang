#include "tokenizer.hpp"
#include "token.hpp"

#include <algorithm>
#include <cctype>
#include <array>
#include <optional>
#include <print>
#include <stdexcept>
#include <utility>


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

bool is_integer_begin(const char c) {
    return std::isdigit(c);
}

bool is_float_begin(const char c) {
    return is_integer_begin(c) or c == '.';
}

bool is_integer_char(const char c) {
    return is_integer_begin(c) or std::isalpha(c) or c == '#' or c == '_';
}

bool is_float_char(const char c) {
    return is_float_begin(c) or c == 'e' or c == '_';
}

bool is_dec(const char c) {
    return std::isdigit(c);
}

bool is_dec_like(const char c) {
    return std::isdigit(c) or c == '_';
}

bool is_num_like(const char c) {
    return std::isalnum(c) or c == '_';
}

bool is_member_access_char(const char c) {
    return c == '.' or c == ':';
}

bool is_type_decl_char(const char c) {
    return c == ':';
}

}

Tokenizer::iterator Tokenizer::previous_iter() {
    return current.iter - 1;
}

Tokenizer::iterator Tokenizer::current_iter() {
    return current.iter;
}

Tokenizer::iterator Tokenizer::next_iter() {
    return current.iter + 1;
}

char Tokenizer::first_character() {
    return *(token_start.iter);
}

char Tokenizer::previous_character() {
    return *previous_iter();
}

char Tokenizer::current_character() {
    return *current_iter();
}

char Tokenizer::next_character() {
    return *next_iter();
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


Token Tokenizer::consume_current_token(TokenType type) {
    Token tok {
        std::string_view { token_start.iter, current.iter },
        SourcePosition { current.pos.byte, current.pos.x, current.pos.y },
        type
    };

    token_start = current;

    return tok;
}


std::optional<Token> Tokenizer::get_id() {

    if (not is_id_begin(current_character())) {
        return std::nullopt;
    }

    while (has_char() and is_id_char(current_character())) {
        advance_state(current);
    }

    return consume_current_token(TokenType::Id);
}


std::optional<Token> Tokenizer::get_quoted_id() {

    if (not is_id_quote(current_character())) {
        return std::nullopt;
    }

    do {
        advance_state(current);
    } while (has_char() and not is_newline(current_character()) and not is_id_quote(current_character()));

    if (not is_id_quote(current_character())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token(TokenType::QuotedId);
}


std::optional<Token> Tokenizer::get_operator() {
    
    if (not is_operator_char(current_character())) {
        return std::nullopt;
    }

    while (has_char() and is_operator_char(current_character())) {
        advance_state(current);
    }

    return consume_current_token(TokenType::Operator);
}


std::optional<Token> Tokenizer::get_quote() {

    if (not is_quote(current_character())) {
        return std::nullopt;
    }

    const char quote = current_character();
    advance_state(current);

    bool is_escaped = false;
    bool is_terminated = false;

    while (has_char()) {

        const char current_char = current_character();

        if (is_newline(current_char)) {
            return std::nullopt;
        }

        advance_state(current);

        if (current_char == '\\' and not is_escaped) {
            is_escaped = true;
        }
        else if (current_char == quote and not is_escaped) {
            is_terminated = true;
            break;
        }
        else {
            is_escaped = false;
        }

    }

    if (not is_terminated) {
        return std::nullopt;
    }

    return consume_current_token(TokenType::Quote);
}


std::optional<Token> Tokenizer::get_brace() {

    if (not is_brace(current_character())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token(TokenType::Brace);
}


std::optional<Token> Tokenizer::get_sep() {

    if (not is_sep(current_character())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token(TokenType::Sep);
}


std::optional<Token> Tokenizer::get_newline() {

    if (not is_newline(current_character())) {
        return std::nullopt;
    }

    advance_state(current);

    return consume_current_token(TokenType::Newline);
}


std::optional<Token> Tokenizer::get_space() {

    if (not is_space(current_character())) {
        return std::nullopt;
    }

    while (has_char() and is_space(current_character())) {
        advance_state(current);
    }

    return consume_current_token(TokenType::Space);
}


std::optional<Token> Tokenizer::get_integer() {

    if (not is_integer_begin(current_character())) {
        return std::nullopt;
    }


    while (has_char() and is_num_like(current_character())) {
        advance_state(current);
    }

    return consume_current_token(TokenType::Integer);
}

std::optional<Token> Tokenizer::get_float() {
    const char previous = previous_character();

    bool accept_e = previous != 'e';

    while (has_next()) {
        advance_state(current);

        if (accept_e and current_character() == 'e') {
            accept_e = false;
        }
        else if (not is_dec_like(current_character())) {
            break;
        }
    }

    return consume_current_token(TokenType::Float);
}

// FIXME: Fix number parsing
std::optional<Token> Tokenizer::get_number() {

    if (not is_integer_begin(current_character()) and not is_float_begin(current_character())) {
        return std::nullopt;
    }

    if (current_character() == '.' and not is_dec_like(next_character())) {
        return std::nullopt;
    }

    // TODO: Negative numbers, negative exponents
    while (has_char()) {

        advance_state(current);
        const char current_char = current_character();

        if (current_char == '#') {
            return get_integer();
        }

        if ((current_char == '.' or current_char == 'e') ) {
            if (is_dec(next_character())) {
                return get_float();
            }

            break;
        }

        if (not is_dec_like(current_char)) {
            break;
        }
    }


    return consume_current_token(TokenType::Integer);
}


std::optional<Token> Tokenizer::get_member_access() {

    if (not is_member_access_char(current_character())) {
        return std::nullopt;
    }

    advance_state(current);

    if (first_character() == '.') {
        return consume_current_token(TokenType::MemberAccess);
    }

    if (has_char() and current_character() == ':') {
        advance_state(current);
        return consume_current_token(TokenType::MemberAccess);
    }

    return std::nullopt;
}


std::optional<Token> Tokenizer::get_type_decl() {

    if (not is_type_decl_char(current_character())) {
        return std::nullopt;
    }

    advance_state(current);
    return consume_current_token(TokenType::TypeDecl);
}


std::optional<Token> Tokenizer::get_invalid() {
    advance_state(current);
    return consume_current_token(TokenType::Invalid);
}


bool Tokenizer::has_char() {
    return current.iter != end;
}

bool Tokenizer::has_next() {
    return current.iter+1 != end;
}


Token Tokenizer::get_next() {

    for (auto parser : parsers) {
        current = token_start;
        auto resp = (this->*parser)();

        if (resp.has_value()) {
            return *resp;
        }
    }

    std::unreachable();
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

