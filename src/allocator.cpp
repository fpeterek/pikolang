#include "allocator.hpp"

#include <cctype>
#include <cstring>
#include <fstream>
#include <string>


size_t Allocator::available_system_memory() {
    
    static constexpr size_t default_size = 1ul * 1024 * 1024 * 1024;


    std::string line;
    std::ifstream is { "/proc/meminfo" };

    if (not is) {
        return default_size;
    }

    while (true) {
        std::getline(is, line);

        if (not is) {
            return default_size;
        }

        if (line.starts_with("MemAvailable:")) {
            break;
        }
    }

    const char* data = line.c_str() + std::strlen("MemAvailable:");

    while (not std::isdigit(*data)) {
        ++data;
    }

    std::string mem_avail { data };

    const auto kbs = std::stoull(mem_avail);

    size_t available = static_cast<size_t>(kbs) * 1024;

    // We will rely on virtual memory semantics here, but still try not to go beyond what's
    // currently available.
    return std::max(available, default_size);
}


