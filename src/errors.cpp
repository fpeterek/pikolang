#include "errors.hpp"
#include "source_position.hpp"

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <print>

#include "colors.hpp"

namespace {


size_t max_width() {
    return 120;
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

std::string_view extract_line(std::string_view str, size_t index) {
    
    return std::string_view {
        get_line_begin(str, index),
        get_line_end(str, index)
    };

}

struct PrintBoundaries {
    size_t begin_ommitted;
    size_t end_ommitted;
};

struct ErrorLine {

    std::string_view full_line;
    std::string_view trimmed_line;

    size_t marker_position;

    PrintBoundaries boundaries;

};

ErrorLine get_line(std::string_view str, SourcePosition pos) {
    auto line = extract_line(str, pos.byte());

    const size_t x = pos.x();

    const size_t left_max = max_width() / 2;

    const size_t marker_offset = std::min(x, left_max);

    const size_t left = x - marker_offset;

    std::string_view trimmed_left = std::string_view { line.begin() + left, line.end() };
    std::string_view trimmed = trimmed_left.subview(0, max_width());

    return ErrorLine {
        line,
        trimmed,
        marker_offset,
        PrintBoundaries {
            left,
            trimmed_left.size() - trimmed.size(),
        }
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


void print_line(const ErrorLine& line) {

    std::string_view fully_trimmed = line.trimmed_line;
    bool left_ellipsis  = false;
    bool right_ellipsis = false;

    if (line.boundaries.begin_ommitted > 0) {
        fully_trimmed = fully_trimmed.subview(3);
        left_ellipsis = true;
    }
    if (line.boundaries.end_ommitted > 0) {
        fully_trimmed = fully_trimmed.subview(0, fully_trimmed.size() - 3);
        right_ellipsis = true;
    }

    if (left_ellipsis) {
        std::print("{}...", colors::gray);
    }

    std::print("{}{}", colors::standard, fully_trimmed);

    if (right_ellipsis) {
        std::print("{}...{}", colors::gray, colors::standard);
    }

    std::println();
}

void print_offset(const size_t offset) {
    static constexpr std::string_view spaces = "                                                                                                    ";

    size_t printed = 0;

    std::print("{}", colors::standard);
    while (true) {
        const size_t to_print = offset - printed;

        if (to_print <= spaces.size()) {
            std::print("{}", spaces.subview(0, to_print));
            break;
        }

        std::print("{}", spaces);
        printed += spaces.size();
    }
}

void pad_until_index(size_t idx) {
    if (idx == 0) {
        return;
    }
    print_offset(idx - 1);
}

void print_marker(const ErrorLine& line) {
    pad_until_index(line.marker_position);
    std::println("^");
}

void print_message(const ErrorLine& line, const Error& error) {
    pad_until_index(line.marker_position);
    std::println("{}", error.message());
}


void print_error(const Error& error, const Files& files) {

    auto& file = files.get(std::string { error.file() });

    auto line = get_line(file.contents(), error.position());

    print_header(file, error.position());
    print_line(line);
    print_marker(line);
    print_message(line, error);
}

} // namespace

void ErrorPrinter::print_errors(
        const std::vector<Error>& errors,
        const Files& files) {

    for (auto& error : errors) {
        print_error(error, files);
    }

}

