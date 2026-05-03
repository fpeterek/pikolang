#include "files.hpp"

#include <memory>
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

        auto ssize = is.tellg();
        auto usize = static_cast<size_t>(ssize);

        std::string contents(usize, '\0');

        is.read(&contents[0], ssize);

        if (not is) {
            throw std::runtime_error { "Failed to read file" };
        }

        return contents;
    }

    std::string read_file_contents(std::string_view filename) {

        std::ifstream is = open_file(filename);

        return read_file(is);
    }

}


const File& Files::load_file(std::string_view file) {

    std::string contents = read_file_contents(file);

    auto result = files.emplace(file, File{file, std::move(contents)});

    if (not result.second) {
        throw std::runtime_error { "Failed to store loaded file" };
    }
    return result.first->second;
}

const File& Files::get(std::string_view file) {
    std::lock_guard lock { mutex };

    std::string filename { file };
    auto it = files.find(filename);

    if (it != files.end()) {
        return it->second;
    }

    return load_file(std::move(filename));
}
