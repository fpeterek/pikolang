#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <functional>
#include <optional>
#include <string_view>
#include <vector>

#include "files.hpp"
#include "token.hpp"


class Tokenized {

    std::string_view srcfile;
    std::vector<Token> toks;

public:
    Tokenized(std::string_view file, std::vector<Token> tokens) :
        srcfile { file },
        toks { std::move(tokens) } { }

    std::string_view file() const { return srcfile; }
    const std::vector<Token>& tokens() const { return toks; }
};


class Tokenizer {

    using iterator = std::string_view::const_iterator;
    using Parser = std::function<std::optional<Token>()>;

    struct State {
        iterator iter;
        struct {
            size_t x;
            size_t y;
            size_t byte;
        } pos;
    };

    std::vector<Token> tokens;

    std::string_view filename;
    iterator end;

    State token_start;
    State current;

    Tokenizer(const File& file) noexcept :
        filename { file.name() },
        end { file.contents().end() },
        token_start {
            file.contents().begin(),
            { 0, 0, 0 }
        },
        current {
            file.contents().begin(),
            { 0, 0, 0 }
        }
        { }

    iterator previous_iter();
    iterator current_iter();
    iterator next_iter();

    char first_character();
    char previous_character();
    char current_character();
    char next_character();

    bool has_prefix(std::string_view prefix);

    Token consume_current_token(TokenType type);

    Tokenized tokenize();

    bool has_char();
    bool has_next();
    void process_token();
    Token get_next();

    void advance_state(State& state);

    // Helpers
    std::optional<Token> get_integer();
    std::optional<Token> get_float();

    // Token Parsers
    std::optional<Token> get_id();
    std::optional<Token> get_quoted_id();
    std::optional<Token> get_operator();
    std::optional<Token> get_quote();
    std::optional<Token> get_brace();
    std::optional<Token> get_sep();
    std::optional<Token> get_newline();
    std::optional<Token> get_space();
    std::optional<Token> get_number();
    std::optional<Token> get_member_access();
    std::optional<Token> get_type_decl();
    std::optional<Token> get_invalid();

    static constexpr std::array parsers {
        &Tokenizer::get_id,
        &Tokenizer::get_quoted_id,
        &Tokenizer::get_operator,
        &Tokenizer::get_quote,
        &Tokenizer::get_brace,
        &Tokenizer::get_sep,
        &Tokenizer::get_newline,
        &Tokenizer::get_space,
        &Tokenizer::get_number,
        &Tokenizer::get_member_access,
        &Tokenizer::get_type_decl,
        &Tokenizer::get_invalid,
    };


public:
    static Tokenized tokenize(const File& file);
};

#endif  // TOKENIZER_HPP
