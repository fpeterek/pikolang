#ifndef FILES_HPP
#define FILES_HPP

#include <string_view>
#include <string>
#include <mutex>
#include <unordered_map>

class File {

    std::string_view filename;
    std::string file_contents;

public:

    File(std::string_view file, std::string contents) :
        filename { file },
        file_contents { std::move(contents) }
        { }

    File(const File& other) = delete;
    File(File&& other) = default;

    File& operator=(File&& other) = default;

    std::string_view name() const { return filename; }
    std::string_view contents() const { return file_contents; }

};

class Files {

    std::mutex mutex{};
    std::unordered_map<std::string, File> files;

    const File& load_file(std::string filename);

public:

    const File& get(std::string file);

};

#endif  // FILES_HPP
