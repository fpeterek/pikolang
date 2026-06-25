#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <vector>

#include "files.hpp"
#include "source_position.hpp"


class Error {

    std::string msg;
    std::string src_file;
    SourcePosition src_pos;

public:

    Error(std::string msg, std::string file, SourcePosition pos) :
        msg { std::move(msg) },
        src_file { std::move(file) },
        src_pos { std::move(pos) } {  }

    const std::string& message() const {
        return msg;
    }
    
    const std::string& file() const {
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
        const std::vector<Error>& errors,
        const Files& files);

};


#endif  // ERRORS_HPP
