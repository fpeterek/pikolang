#ifndef COLLECTIONS_STRING_HPP
#define COLLECTIONS_STRING_HPP

#include <algorithm>
#include <string_view>

#include "allocator.hpp"


struct String {

    static std::string_view create(Allocator& allocator, std::string_view from) {

        char* chars = allocator.allocate<char>(from.size());

        std::copy(from.begin(), from.end(), chars);

        return std::string_view { chars, from.size() };
    }

};


#endif  // COLLECTIONS_STRING_HPP
