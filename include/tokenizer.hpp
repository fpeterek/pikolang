#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string_view>
#include <vector>

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

    Tokenized tokenize();

};

#endif  // TOKENIZER_HPP
