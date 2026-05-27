#include <algorithm>
#include <string_view>

#include "files.hpp"
#include "tokenizer.hpp"
#include "print_tokens.hpp"


struct Args {
    std::string_view program;
    std::vector<std::string_view> args;

    std::string_view operator[](size_t idx) const {
        return args.at(idx);
    }
};


void run(const Args& args) {

    Files files;
    const File& file = files.get(std::string { args[0] });

    auto res = Tokenizer::tokenize(file);

    print_tokens(res.tokens());
}


int main(int argc, const char* argv[]) {

    std::vector<std::string_view> vec;
    vec.reserve(static_cast<size_t>(argc - 1));
    std::transform(argv + 1, argv + argc, std::back_inserter(vec),
                   [](const char* arg) { return std::string_view { arg }; });

    Args args {
        argv[0],
        std::move(vec),
    };

    run(args);
}
