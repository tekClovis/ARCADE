/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Parsing
*/

#include "Parsing.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <dlfcn.h>

namespace fs = std::filesystem;

namespace Arc {
    Parsing::Parsing(const std::string &libraryDirectory)
    : _libraryDirectory(libraryDirectory)
    {
        parseLibraries();
    }

    void Parsing::parseLibraries()
    {
        _rendererPaths.clear();
        _gamePaths.clear();
        _rendererNames.clear();
        _gameNames.clear();

        for (const auto &entry : fs::directory_iterator(_libraryDirectory)) {
            if (entry.path().extension() != ".so")
                continue;

            const std::string path = entry.path().string();
            void *handle = dlopen(path.c_str(), RTLD_LAZY);

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

            const std::string name = getName();

            if (std::find(_rendererNames.begin(), _rendererNames.end(), name) != _rendererNames.end() ||
                std::find(_gameNames.begin(), _gameNames.end(), name) != _gameNames.end()) {
                std::cerr << "Warning: duplicate library name '" << name << "' in '" << path << "'. Skipping." << std::endl;
                dlclose(handle);
                continue;
            }

            const auto type = static_cast<Arc::LibraryType>(getLibraryType());

            if (type == Arc::LibraryType::Renderer) {
                _rendererPaths.emplace_back(name, path);
                _rendererNames.push_back(name);
            } else if (type == Arc::LibraryType::Game) {
                _gamePaths.emplace_back(name, path);
                _gameNames.push_back(name);
            }

            dlclose(handle);
        }
    }

    const std::list<std::pair<std::string, std::string>> &Parsing::getRendererPaths() const
    {
        return _rendererPaths;
    }

    const std::list<std::pair<std::string, std::string>> &Parsing::getGamePaths() const
    {
        return _gamePaths;
    }
}