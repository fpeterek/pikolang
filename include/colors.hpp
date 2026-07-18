#ifndef COLORS_HPP
#define COLORS_HPP

#include <string_view>


// https://talyian.github.io/ansicolors/
namespace colors {
    using Color = std::string_view;

    constexpr Color light_gray   = "\x1b[38;5;251m";
    constexpr Color gray         = "\x1b[38;5;244m";

    constexpr Color light_purple = "\x1b[38;5;218m";
    constexpr Color dark_purple  = "\x1b[38;5;140m";

    constexpr Color light_blue   = "\x1b[38;5;39m";
    constexpr Color blue         = "\x1b[38;5;69m";

    constexpr Color red          = "\x1b[38;5;196m";

    constexpr Color light_green  = "\x1b[38;5;48m";
    constexpr Color dark_green   = "\x1b[38;5;76m";

    constexpr Color light_orange = "\x1b[38;5;222m";
    constexpr Color orange       = "\x1b[38;5;210m";

    constexpr Color light_pink   = "\x1b[38;5;224m";

    // General

    constexpr Color standard = light_gray;

    // Errors

    constexpr Color filename = light_purple;
    constexpr Color position = light_blue;
    constexpr Color error    = red;

    // Code

    constexpr Color keyword       = dark_green;
    constexpr Color identifier    = light_orange;
    constexpr Color whitespace    = gray;
    constexpr Color operator_     = light_blue;
    constexpr Color brace         = dark_purple;
    constexpr Color quote         = light_green;
    constexpr Color integer       = light_pink;
    constexpr Color floating      = light_purple;
    constexpr Color member_access = blue;
    constexpr Color type_decl     = orange;
    constexpr Color invalid       = red;
}

#endif // COLORS_HPP
