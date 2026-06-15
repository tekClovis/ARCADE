/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** main.cpp
*/


#include "Core.hpp"
#include <filesystem>
#include <iostream>
#include <dlfcn.h>
#include <list>
#include <algorithm>
#include "IRenderer.hpp"
#include "IGame.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <default_Renderer_library_path>" << std::endl;
        return 84;
    }
    std::string defaultLib = argv[1];
    
    if ( defaultLib.find("./") == 0) 
        defaultLib = defaultLib.substr(2);

    void* defaultHandle = dlopen(defaultLib.c_str(), RTLD_LAZY);

    if (!defaultHandle) {
        std::cerr << "Error: cannot open library '" << defaultLib << "'" << std::endl;
        return 84;
    }

    auto getDefaultLibraryType = reinterpret_cast<int (*)()>(dlsym(defaultHandle, "getLibraryType"));

    if (!getDefaultLibraryType) {
        std::cerr << "Error: invalid library (missing getLibraryType): '" << defaultLib << "'" << std::endl;
        dlclose(defaultHandle);
        return 84;
    }

    const auto defaultType = static_cast<Arc::LibraryType>(getDefaultLibraryType());

    dlclose(defaultHandle);
    if (defaultType != Arc::LibraryType::Renderer) {
        std::cerr << "Error: default library must be a renderer: '" << defaultLib << "'" << std::endl;
        return 84;
    }

    try {
        std::list<std::pair<std::string, std::string>> RendererPaths;
        std::list<std::pair<std::string, std::string>> gamePaths;
        std::list<std::string> rendererNames;
        std::list<std::string> gameNames;

        for (const auto& entry : fs::directory_iterator("./lib/")) {
            if (entry.path().extension() != ".so")
                continue;

            const std::string path = entry.path().string();
            
            void* handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!handle)
                continue;

            auto getLibraryType = reinterpret_cast<int (*)()>(dlsym(handle, "getLibraryType"));

            if (!getLibraryType) {
                dlclose(handle);
                continue;
            }

            auto getName = reinterpret_cast<std::string (*)()>(dlsym(handle, "getName"));

            if (!getName) {
                dlclose(handle);
                continue;
            }

            if (std::find(rendererNames.begin(), rendererNames.end(), getName()) != rendererNames.end() || std::find(gameNames.begin(), gameNames.end(), getName()) != gameNames.end()) {
                std::cerr << "Warning: duplicate library name '" << getName() << "' in '" << path << "'. Skipping." << std::endl;
                dlclose(handle);
                continue;
            }
            
            const auto type = static_cast<Arc::LibraryType>(getLibraryType());

            if (type == Arc::LibraryType::Renderer) {
                RendererPaths.emplace_back(getName(), path);
                rendererNames.push_back(getName());
            } else if (type == Arc::LibraryType::Game) {
                gamePaths.emplace_back(getName(), path);
                gameNames.push_back(getName());
            }
            dlclose(handle);
        }
        Arc::Core core(defaultLib, RendererPaths, gamePaths);

        core.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
