#include "FilePath.h"
#include "Log.h"

const std::string FilePath::get(const fs::path& file, FileType type) {

    // get absolute path of game project
    fs::path absolute = fs::current_path();
    while (!absolute.generic_string().ends_with("/build")) {
        absolute = absolute.parent_path();
    }
    absolute = absolute.parent_path();
    absolute /= paths.at(type);
    absolute /= file;

    if (!fs::exists(absolute)) {
        Error::file(type, absolute.generic_string());
    }

    return absolute.generic_string();
}
