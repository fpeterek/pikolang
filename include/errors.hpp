#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string_view>

#include "collections/array.hpp"
#include "files.hpp"
#include "source_position.hpp"


class Error {

    std::string_view msg;
    std::string_view src_file;
    SourcePosition src_pos;

public:

    Error(std::string_view msg, std::string_view file, SourcePosition pos) :
        msg { msg },
        src_file { file },
        src_pos { std::move(pos) } {  }

    std::string_view message() const {
        return msg;
    }
    
    std::string_view file() const {
        return src_file;
    }

    SourcePosition position() const {
        return src_pos;
    }

};


class ErrorPrinter {

    // static void print_error();

public:

    static void print_errors(
        const Array<Error>& errors,
        const Files& files);

};


#endif  // ERRORS_HPP
