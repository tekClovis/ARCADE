/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Core
*/

#ifndef CORE_HPP_
#define CORE_HPP_
#include "IGame.hpp"
#include "IRenderer.hpp"
#include "DLLoader.hpp"
#include "Item.hpp"
#include <list>
#include <string>
#include <memory>
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Error.hpp"


namespace Arc {
    class Core {
        public:
            Core(std::string defaultRendererPath, std::list<std::pair<std::string, std::string>> RendererPaths, std::list<std::pair<std::string, std::string>> gamePaths);
            ~Core();
            void run();
            void launchGame(const std::string& gamePath, const std::string& RendererPath);
            void loadRenderer(const std::string& libPath);
            void loadGame(const std::string& libPath);
            void unloadRenderer();
            void unloadGame();
            std::string cleanPath(const std::string& path);
            void retrieveAllUserScores();
            void writeScoreInFile();
            std::string cropCopyName(std::string& name);
            void moveJoysticks(Event event);

            bool _isRunning;
            std::list<std::pair<std::string, std::string>> _rendererPaths;
            std::list<std::pair<std::string, std::string>> _gamePaths;
            
            std::string _currentUser;
            std::map<std::string, int> _gameScores;
            std::map<std::string, std::map<std::string, int>> _allUserScores;

        protected:
        private:
            std::map<std::string, Item> _items;

            std::string _defaultRendererPath;

            std::unique_ptr<IRenderer> _renderer;
            std::unique_ptr<IGame> _game;

            std::unique_ptr<DLLoader<IRenderer>> _rendererLoader;
            std::unique_ptr<DLLoader<IGame>> _gameLoader;
            std::string _currentRenderer;

            std::unique_ptr<DLLoader<IGame>> _nextGameLoader;


            
    };
}

#endif /* !CORE_HPP_ */
