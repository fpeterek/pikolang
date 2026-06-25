#include "errors.hpp"
#include "token.hpp"

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <print>


namespace {

namespace colors {
    using Color = std::string_view;

    constexpr Color standard = "\x1b[38;5;251m";
    constexpr Color filename = "\x1b[38;5;218m";
    constexpr Color position = "\x1b[38;5;39m";
    constexpr Color gray = "\x1b[38;5;244m";
    constexpr Color error = "\x1b[38;5;196m";

    constexpr Color keyword = "\x1b[38;5;76m";
    constexpr Color identifier = "\x1b[38;5;222m";
    constexpr Color brace = "\x1b[38;5;140m";
    constexpr Color quote = "\x1b[38;5;48m";
    constexpr Color integer = "\x1b[38;5;224m";
    // constexpr Color floating = "\x1b[38;5;216m";
    constexpr Color floating = "\x1b[38;5;218m";
	
    constexpr Color member_access = "\x1b[38;5;69m";
    constexpr Color type_decl = "\x1b[38;5;210m";

}


int max_width() {
    return 80;
}

std::string_view::const_iterator get_line_begin(
    std::string_view str, size_t index) {

    if (index == 0) {
        return str.begin();
    }

    auto it = str.begin() + static_cast<ptrdiff_t>(index);

    // In case the iterator already points to a newline
    it -= 1;

    while (true) {
        if (it == str.begin()) {
            return it;
        }

        if (*it == '\n' or *it == '\r') {
            return it + 1;
        }

        it -= 1;
    }
}

std::string_view::const_iterator get_line_end(
    std::string_view str, size_t index) {

    ptrdiff_t idx = static_cast<ptrdiff_t>(index);

    return std::find(str.begin() + idx, str.end(), '\n');
}

std::string_view get_line(std::string_view str, size_t index) {
    
    return std::string_view {
        get_line_begin(str, index),
        get_line_end(str, index)
    };

}


void print_header(const File& file, SourcePosition pos) {

    std::string msg =
        std::format(
            "{}[{}{}{}, {}({}:{}){}] {}Error encountered",
            colors::standard,
            colors::filename, file.name(),
            colors::standard,
            colors::position, pos.y(), pos.x(),
            colors::standard,
            colors::error
        );

    std::println("{}", msg);
}

void print_line(std::string_view line) {

}

void print_marker(std::string_view line) {

}

void print_message(const Error& error) {

}


void print_error(const Error& error, const Files& files) {

    auto& file = files.get(error.file());

    auto line = get_line(file.contents(), error.position().byte());

    print_header(file, error.position());
    print_line(line);
    print_marker(line);
    print_message(error);
}

} // namespace

void ErrorPrinter::print_errors(
        const std::vector<Error>& errors,
        const Files& files) {

    for (auto& error : errors) {
        print_error(error, files);
    }

}

