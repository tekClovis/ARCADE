/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** main.cpp
*/


#include "Core.hpp"
#include "Parsing.hpp"
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <default_Renderer_library_path>" << std::endl;
        return 84;
    }
    
    std::string display = std::getenv("DISPLAY") ? std::getenv("DISPLAY") : "";

    if (display.empty()) {
        std::cerr << "You tried to run the program in TTY, you can't do that unfortunately" << std::endl;
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
        Arc::Parsing parsing("./lib/");
        Arc::Core core(defaultLib, parsing.getRendererPaths(), parsing.getGamePaths());

        core.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
