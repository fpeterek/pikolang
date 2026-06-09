#include "files.hpp"

#include <mutex>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>


namespace {

    std::ifstream open_file(std::string_view filename) {
        std::ifstream is { std::string { filename }, std::ios::binary | std::ios::ate };

        if (not is) {
            throw std::runtime_error { "Failed to open file" };
        }

        return is;
    }

    std::string read_file(std::ifstream& is) {
        std::stringstream ss{};

        const auto ssize = is.tellg();
        const auto usize = static_cast<size_t>(ssize);

        is.seekg(0);

        std::string contents(usize, '\0');

        is.read(&contents[0], ssize);

        if (not is and not is.eof()) {
            throw std::runtime_error { "Failed to read file" };
        }

        return contents;
    }

    std::string read_file_contents(std::string_view filename) {

        std::ifstream is = open_file(filename);

        return read_file(is);
    }

}


const File& Files::load_file(std::string filename) {

    File file { filename, read_file_contents(filename) };

    const auto result = files.emplace(std::move(filename), std::move(file));

    if (not result.second) {
        throw std::runtime_error { "Failed to store loaded file" };
    }
    return result.first->second;
}


const File& Files::get(std::string file) {
    std::lock_guard lock { mutex };

    const auto it = files.find(file);

    if (it != files.end()) {
        return it->second;
    }

    return load_file(std::move(file));
}
