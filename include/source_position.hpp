#ifndef SOURCE_POSITION_HPP
#define SOURCE_POSITION_HPP

#include <cstddef>


class SourcePosition {

    size_t src_byte;
    size_t src_x;
    size_t src_y;

public:
    SourcePosition(size_t byte, size_t x, size_t y) :
        src_byte { byte },
        src_x { x },
        src_y { y } { }

    SourcePosition(const SourcePosition& other) = default;

    size_t x() const { return src_x; }
    size_t y() const { return src_y; };

    size_t line() const { return src_y; };
    size_t col() const { return src_x; };

    size_t byte() const { return src_byte; };
};


#endif  // SOURCE_POSITION_HPP
