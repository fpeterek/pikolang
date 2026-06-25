#include <algorithm>
#include <print>
#include <string_view>

#include "files.hpp"
#include "tokenizer.hpp"
#include "print_tokens.hpp"


void win_check() {
#if defined(_WIN32)
    std::println(
        "\x1b[38;5;196m"
        R"( _____________________________________ )" "\n"
        R"(/ Windows detected, program rejected. \)" "\n"
        R"(\ Install a proper operating system.  /)" "\n"
        R"( ------------------------------------- )" "\n"
        R"(   \                                   )" "\n"
        R"(    \                                  )" "\n"
        R"(        .--.                           )" "\n"
        R"(       |o_o |                          )" "\n"
        R"(       |:_/ |                          )" "\n"
        R"(      //   \ \                         )" "\n"
        R"(     (|     | )                        )" "\n"
        R"(    /'\_   _/`\                        )" "\n"
        R"(    \___)=(___/                        )" "\n"
    );
    exit(-1);
#endif
}



struct Args {
    std::string_view program;
    std::vector<std::string_view> args;

    std::string_view operator[](size_t idx) const {
        return args.at(idx);
    }
};


void run(const Args& args) {

    Files files;
    const File& file = files.get_or_load(std::string { args[0] });

    auto res = Tokenizer::tokenize(file);

    print_tokens(res.tokens());
}


int main(int argc, const char* argv[]) {

    win_check();

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
