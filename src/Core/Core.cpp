/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Core
*/

#include "Core.hpp"
#include <algorithm>
#include <thread>
#include "Menu.hpp"

namespace Arc {

    
    Core::Core(std::string defaultRendererPath, std::list<std::pair<std::string, std::string>> RendererPaths, std::list<std::pair<std::string, std::string>> gamePaths)
    : _isRunning(false), _rendererPaths(RendererPaths), _gamePaths(gamePaths), _defaultRendererPath(defaultRendererPath)
    {
        _items.clear();
        _items["Background"] = {{960, 540}, {1, 1}, 0, {0, 0}, false, -1, "./assets/textures/menu/Menu_Background.png", " ", std::function<void()>()};

        const std::string normalizedDefault = cleanPath(defaultRendererPath);

        auto renderer = _rendererPaths.end();

        for (auto iterator = _rendererPaths.begin(); iterator != _rendererPaths.end(); ++iterator) {
             if (cleanPath(iterator->second) == normalizedDefault) {
                renderer = iterator;
                break;
            }
        }

        if (renderer != _rendererPaths.end())
            loadRenderer(renderer->second);
        else if (!_rendererPaths.empty())
            loadRenderer(_rendererPaths.begin()->second);

        for (auto& [game,_] : gamePaths) {
            if (game == "Tron") {
                _gameScores["Tron_Blue_Player"] = 0;
                _gameScores["Tron_Orange_Player"] = 0;
            }
            else
                _gameScores[game] = 0;
        }

        retrieveAllUserScores();

        _currentUser = "";
        _game = std::make_unique<Menu>(_items, this);
    }

    Core::~Core()
    {
        unloadRenderer();
        unloadGame();
    }

    std::string Core::cropCopyName(std::string& name)
    {
        if (name.size() > 5 && name.substr(name.size() - 5) == "_copy")
            return name.substr(0, name.size() - 5);
        return name;
    }

    std::string Core::cleanPath(const std::string& path)
    {
        if (path.rfind("./", 0) == 0)
            return path.substr(2);
        return path;
    }

    void Core::retrieveAllUserScores() {
        _allUserScores.clear();

        for (auto& save : std::filesystem::directory_iterator("./Scores")) {

            if (save.path().extension() != ".txt")
                continue;

            std::ifstream readFile (save.path(), std::ios::in);

            if (!readFile.is_open()) {
                std::cerr << "Failed to open " << save.path() << " for reading." << std::endl;
                continue;
            }

            std::string userName = save.path().stem().string();

            std::map<std::string, int> savedScores;

            for (auto line = std::string(); std::getline(readFile, line); ) {

                size_t separator = line.find(": ");

                if (separator != std::string::npos) {
                    std::string game = line.substr(0, separator);
                    game = cropCopyName(game);
                    int score = std::stoi(line.substr(separator + 2));
                    savedScores[game] = score;
                }
            }
            readFile.close();
            _allUserScores[userName] = savedScores;
        }
    }

    void Core::loadRenderer(const std::string& libPath) {
        _rendererLoader = std::make_unique<DLLoader<IRenderer>>(libPath, "entryPoint");
        _renderer.reset(_rendererLoader->getInstance(_items));
        _currentRenderer = cleanPath(libPath);
        if (!_renderer)
            throw FailedToLoadRenderer(libPath);
        _renderer->createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Arcade");
    }

    void Core::loadGame(const std::string& libPath) {
        _gameLoader = std::make_unique<DLLoader<IGame>>(libPath, "entryPoint");
        _game.reset(_gameLoader->getInstance(_items));
        if (!_game)
            throw FailedToLoadGame(libPath);
    }
    
    void Core::unloadRenderer()
    {
        _renderer.reset();
        _rendererLoader.reset();
    }

    void Core::unloadGame()
    {
        _game.reset();
        _gameLoader.reset();
    }

    void Core::run()
    {
        _isRunning = true;

        auto frameDuration = std::chrono::milliseconds(16); // 60 FPS
        auto nextFrameTime = std::chrono::steady_clock::now();

        while (_isRunning) {
            Event event = {Type::KeyPressed, Key::None, {0, 0}};
            while (_renderer->pollEvent(event)) {
                if (event.type == Type::Quit)
                    _isRunning = false;
                if (event.type == Type::KeyPressed && event.code == Key::Tab && std::chrono::steady_clock::now() - _lastRendererSwitchTime > std::chrono::milliseconds(500)) {
                    std::string nextRenderer;

                    for (auto iterator = _rendererPaths.begin(); iterator != _rendererPaths.end(); ++iterator) {
                         if (cleanPath(iterator->second) == _currentRenderer) {
                            auto nextIterator = std::next(iterator);
                            if (nextIterator == _rendererPaths.end())
                                nextIterator = _rendererPaths.begin();
                            nextRenderer = nextIterator->second;
                            break;
                        }
                    }
                    _lastRendererSwitchTime = std::chrono::steady_clock::now();


                    unloadRenderer();
                    loadRenderer(nextRenderer);
                    break;
                }
                _game->handleKey(event);
            }

            
            _game->update(event);
            _renderer->display(_items);
            
            if (_game->isGameOver()) {
                _items.clear();
                _items["Background"] = {{960, 540}, {1, 1}, 0, {0, 0}, false, -1, "./assets/textures/menu/Menu_Background.png", "", std::function<void()>()};
                const std::string currentGame = cropCopyName(_gamePaths.begin()->first);

                if (currentGame == "Tron") {
                    _gameScores["Tron_Blue_Player"] += _game->getScore()[0];
                    _gameScores["Tron_Orange_Player"] += _game->getScore()[1];
                }
                else {
                    _gameScores[currentGame] = _game->getScore()[0];
                    if (_gameScores[currentGame] > _allUserScores[_currentUser][currentGame])
                        _allUserScores[_currentUser][currentGame] = _gameScores[currentGame];
                }
                _game = std::make_unique<Menu>(_items, this);
            }

            nextFrameTime += frameDuration;
            auto now = std::chrono::steady_clock::now();
            if (nextFrameTime > now)
                std::this_thread::sleep_until(nextFrameTime);
            else
                nextFrameTime = now;
        }
        writeScoreInFile();
    }

    void Core::writeScoreInFile() {

        if (_currentUser.empty())
            return;

        std::map<std::string, int> savedScores;

        if (std::filesystem::exists("./Scores/" + _currentUser + ".txt")) {
            std::ifstream readFile ("./Scores/" + _currentUser + ".txt", std::ios::in);

            if (!readFile.is_open()) {
                std::cerr << "Failed to open " << _currentUser << ".txt for reading." << std::endl;
                return;
            }

            for (auto line = std::string(); std::getline(readFile, line); ) {

                size_t separator = line.find(": ");

                if (separator != std::string::npos) {
                    std::string game = line.substr(0, separator);
                    game = cropCopyName(game);
                    int score = std::stoi(line.substr(separator + 2));
                    if (savedScores.find(game) == savedScores.end() || score > savedScores[game])
                        savedScores[game] = score;
                }
            }
            readFile.close();
        }
        
        std::ofstream writeFile ( "./Scores/" + _currentUser + ".txt", std::ios::out | std::ios::trunc);

        if (!writeFile.is_open()) {
            std::cerr << "Failed to open " << _currentUser << ".txt for writing." << std::endl;
            return;
        }

        for (const auto& [game, score] : savedScores) {
            if (_gameScores.find(game) != _gameScores.end() && _gameScores[game] > score)
                writeFile << game << ": " << _gameScores[game] << std::endl;
            else 
                writeFile << game << ": " << savedScores[game] << std::endl;
        }

        for (const auto& [game, score] : _gameScores) {
            if (savedScores.find(game) == savedScores.end())
                writeFile << game << ": " << score << std::endl;
        }


        writeFile.close();
    }

    void Core::launchGame(const std::string& gamePath, const std::string& RendererPath) {

        _items.clear();
        _items["Background"] = {{960, 540}, {1, 1}, 0, {0, 0}, false, -1, "./assets/textures/menu/Menu_Background.png", " ", std::function<void()>()};
        
        const std::string requestedRenderer = cleanPath(RendererPath);

        if (!requestedRenderer.empty() && (_renderer == nullptr || requestedRenderer != _currentRenderer)) {
            try {
                unloadRenderer();
                loadRenderer(requestedRenderer);
            } catch (const std::exception& e) {
                std::cerr << "Failed to load Renderer: " << e.what() << std::endl;
                return;
            }
        }
        
        try {
            _nextGameLoader = std::make_unique<DLLoader<IGame>>(gamePath, "entryPoint");
            auto newGame = _nextGameLoader->getInstance(_items);
            _game = std::unique_ptr<IGame>(newGame);
        } catch (const std::exception& e) {
            std::cerr << "Error launching game: " << e.what() << std::endl;
            _nextGameLoader.reset();
                _game.reset();
        }
    }

}
