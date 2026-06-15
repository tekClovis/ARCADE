/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Parsing
*/

#ifndef PARSING_HPP_
#define PARSING_HPP_

#include "IGame.hpp"
#include <list>
#include <string>

namespace Arc {
    class Parsing {
        public:
            explicit Parsing(const std::string &libraryDirectory);
            void parseLibraries();

            const std::list<std::pair<std::string, std::string>> &getRendererPaths() const;
            const std::list<std::pair<std::string, std::string>> &getGamePaths() const;

        private:
            std::string _libraryDirectory;
            std::list<std::pair<std::string, std::string>> _rendererPaths;
            std::list<std::pair<std::string, std::string>> _gamePaths;
            std::list<std::string> _rendererNames;
            std::list<std::string> _gameNames;
    };
}

#endif /* !PARSING_HPP_ */