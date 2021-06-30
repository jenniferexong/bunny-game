#pragma once

#include <string>
#include <sstream>
#include <string_view>
#include <filesystem>
#include <map>

/**
 * Type of a file
 */
enum struct FileType : int {
    Texture, 
    FontTexture,
    SkyboxTexture,
    Model,
    Shader,
    Data,
    Font, 
    Gui
};

namespace fs = std::filesystem;

namespace FilePath {

    /**
     * The location of files depending on the file type
     */
    const std::map<FileType, fs::path> paths = {
        {FileType::Texture, fs::path("res/textures")},
        {FileType::FontTexture, fs::path("res/font")},
        {FileType::SkyboxTexture, fs::path("res/textures/skybox")},
        {FileType::Model, fs::path("res/objects")},
        {FileType::Shader, fs::path("src/engine/shader-files")},
        {FileType::Data, fs::path("res/data")},
        {FileType::Font, fs::path("res/font")},
        {FileType::Gui, fs::path("res/gui")},
    };

    /**
     * Gets the relative path of a file
     */
    const std::string get(const fs::path& file, FileType type);
};
