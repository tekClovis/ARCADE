/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Menu
*/

#include "Menu.hpp"

namespace Arc {
    Menu::Menu(std::map<std::string, Item>& items, Core* core)
        : _items(items), _core(core)
    {
        _slideGameLogos = false;
        _slideDirection = 0;

        if (_core->_gamePaths.size() == 2) {
            auto first = _core->_gamePaths.begin();
            auto second = std::next(_core->_gamePaths.begin());
            _core->_gamePaths.push_back({first->first + "_copy", first->second});
            _core->_gamePaths.push_back({second->first + "_copy", second->second});
        }

        _chosenGame = cropCopyName(_core->_gamePaths.begin()->first);
        _chosenRenderer = cropCopyName(_core->_rendererPaths.begin()->first);

        _selectorRow = 0;

        _gameLogos["Snake"] = "./assets/textures/menu/Snake_Logo.png";
        _gameLogos["Pacman"] = "./assets/textures/menu/PacMan_Logo.png";
        _gameLogos["Tron"] = "./assets/textures/menu/Tron_Logo.png";
        _gameLogos["Unknown"] = "./assets/textures/menu/Unknown_Logo.png";

        if (_core->_currentUser.empty())
            _userNameEntered = false;
        else {
            _userNameEntered = true;
            createMenuItems();
            return;
        }

        _items["enterYourName"] = {{960, 300}, {50.0f, 50.0f}, 0, {0, 0}, false, 1, "", "ENTER YOUR USERNAME", nullptr};
        _items["nameInput"] = {{960, 560}, {50.0f, 50.0f}, 0, {0, 0}, false, 1, "", "", nullptr};
        _items["inputOutline"] = {{960, 560}, {3.0f, 1.0f}, 0, {0, 0}, false, 2, "./assets/textures/menu/White_Outline.png", "", nullptr};
        _items["enterToStart"] = {{960, 800}, {30.0f, 30.0f}, 0, {0, 0}, false, 1, "", "Press Enter", [this]() { if (!_core->_currentUser.empty()) { _userNameEntered = true; _items.erase("enterYourName"); _items.erase("inputOutline"); _items.erase("nameInput"); createMenuItems(); } }};
    }

    Menu::~Menu()
    {
    }

    std::string Arc::Menu::cropCopyName(std::string& name)
    {
        if (name.size() > 5 && name.substr(name.size() - 5) == "_copy")
            return name.substr(0, name.size() - 5);
        return name;
    }

    void Menu::update(Event event)
    {
        if (!_userNameEntered)
            return;
        updateArrowPulse();
        if (_slideGameLogos)
            slideGameLogos();
        checkButtonHover(event.mouse.x, event.mouse.y, false);
    }

    void Menu::handleKey(Event event)
    {
        if (!_userNameEntered && event.type == Type::KeyPressed) {
            if (event.code == Key::Enter) {
                if (!_core->_currentUser.empty()) {
                    _userNameEntered = true;
                    _items.erase("enterYourName");
                    _items.erase("inputOutline");
                    _items.erase("nameInput");
                    _items.erase("enterToStart");
                    createMenuItems();
                }
            } else if (event.code == Key::Backspace) {
                if (!_core->_currentUser.empty())
                    _core->_currentUser.pop_back();
            } else if (event.code >= Key::Num1 && event.code <= Key::Num0) {
                if (_core->_currentUser.size() < 16) {
                    char digit = (event.code == Key::Num0)
                        ? '0'
                        : static_cast<char>('1' + (event.code - Key::Num1));
                    _core->_currentUser += digit;
                }
            } else if (event.code >= Key::A && event.code <= Key::Z) {
                if (_core->_currentUser.size() < 16)
                    _core->_currentUser += static_cast<char>('A' + (event.code - Key::A));
            }
            _items["nameInput"].text = _core->_currentUser;
            return;
        }

        if (event.type == Type::KeyPressed && event.code == Key::MouseLeft)
            checkButtonHover(event.mouse.x, event.mouse.y, true);
        if (event.type == Type::KeyPressed && event.code == Key::D) {

            if (_selectorRow == 0)
                switchGame(1);
            else if (_selectorRow == 1)
                switchRenderer(1);
        }
        if (event.type == Type::KeyPressed && event.code == Key::Q) {
            if (_selectorRow == 0)
                switchGame(-1);
            else if (_selectorRow == 1)
                switchRenderer(-1);
        }
        if (event.type == Type::KeyPressed && event.code == Key::S)
            moveSelector(-1);
        if (event.type == Type::KeyPressed && event.code == Key::Z)
            moveSelector(1);

        if (event.type == Type::KeyPressed && event.code == Key::Enter) {
            if (_selectorRow == 2)
                _items["Start_Button"].buttonAction();
        }
        
    }

    void Menu::createMenuItems()
    {
        _items["Quit_Button"] = {{1700, 0}, {0.8f, 0.8f}, 0, {0, 0}, false, 2, "./assets/textures/menu/Quit.png", "ESC", [this]() { quit(); }};
        _items["previousGameButton"] = { {500, 300}, {0.2f, 0.2f}, 0, {0, 0}, false, 1, "./assets/textures/menu/Left_Arrow.png", "<", [this]() { switchGame(-1); } };
        _items["nextGameButton"] = { {1420, 300}, {0.2f, 0.2f}, 0, {0, 0}, false, 1, "./assets/textures/menu/Right_Arrow.png", ">", [this]() { switchGame(1); } };
        _items["previousRendererButton"] = {{650, 700}, {0.15f, 0.15f}, 0, {0, 0}, true, 0, "./assets/textures/menu/Left_Arrow.png", "<", [this]() { switchRenderer(-1); }};
        _items["nextRendererButton"] = {{1270, 700}, {0.15f, 0.15f}, 0, {0, 0}, true, 0, "./assets/textures/menu/Right_Arrow.png", ">", [this]() { switchRenderer(1); }};
        _items["Game_Name"] = {{960, 0}, {50.0f, 50.0f}, 0, {0, 0}, false, 1, "", _chosenGame, nullptr};
        _items["Renderer_Name"] = {{960, 700}, {30.0f, 30.0f}, 0, {0, 0}, false, 1, "", _chosenRenderer, nullptr};
        _items["Start_Button"] = {{960, 1000}, {1.5f, 1.5f}, 0, {0, 0}, false, 1, "./assets/textures/menu/Start_Button.png", "START", [this]() {  _core->launchGame(_core->_gamePaths.begin()->second, _core->_rendererPaths.begin()->second); }};
        _items["userName"] = {{350, 0}, {30.0f, 30.0f}, 0, {0, 0}, false, 1, "", _core->_currentUser, nullptr};

        int GlobalBest = getGlobalBestScore(_chosenGame);
        int PersonalBest = getPersonalBestScore(_chosenGame);

        _items["globalBestText"] = {{350, 880}, {35.0f, 35.0f}, 0, {0, 0}, false, 1, "", "GLOBAL BEST: ", nullptr};
        _items["personnalBestText"] = {{1560, 880}, {35.0f, 35.0f}, 0, {0, 0}, false, 1, "", "PERSONAL BEST: ", nullptr};
        _items["globalBestScore"] = {{350, 980}, {35.0f, 35.0f}, 0, {0, 0}, false, 1, "", GlobalBest == 0 ? "N/A" : std::to_string(GlobalBest), nullptr};
        _items["personnalBestScore"] = {{1560, 980}, {35.0f, 35.0f}, 0, {0, 0}, false, 1, "", PersonalBest == 0 ? "N/A" : std::to_string(PersonalBest), nullptr};
        _items["globalBestUser"] = {{350, 1080}, {35.0f, 35.0f}, 0, {0, 0}, false, 1, "", getGlobalBestUser(_chosenGame), nullptr};

        auto selectedScoreIterator = _core->_gameScores.find(_chosenGame);
        int selectedScore = (selectedScoreIterator != _core->_gameScores.end()) ? selectedScoreIterator->second : 0;

        if (_chosenGame != "Tron") {
            _items["gameScore"] = {{960, 550}, {30.0f, 30.0f}, 0, {0, 0}, false, 1, "", "SCORE: " + std::to_string(selectedScore), nullptr};
            _items["Tron_Blue_Score"] = {{1260, 550}, {0.0f, 0.0f}, 0, {0, 0}, false, 1, "", "BLUE SCORE: " + std::to_string(_core->_gameScores["Tron_Blue_Player"]), nullptr};
            _items["Tron_Orange_Score"] = {{660, 550}, {0.0f, 0.0f}, 0, {0, 0}, false, 1, "", "ORANGE SCORE: " + std::to_string(_core->_gameScores["Tron_Orange_Player"]), nullptr}; 
        }
        else {
            _items["Tron_Blue_Score"] = {{1260, 550}, {30.0f, 30.0f}, 0, {0, 0}, false, 1, "", "BLUE SCORE: " + std::to_string(_core->_gameScores["Tron_Blue_Player"]), nullptr};
            _items["Tron_Orange_Score"] = {{660, 550}, {30.0f, 30.0f}, 0, {0, 0}, false, 1, "", "ORANGE SCORE: " + std::to_string(_core->_gameScores["Tron_Orange_Player"]), nullptr}; 
            _items["gameScore"] = {{960, 550}, {0.0f, 0.0f}, 0, {0, 0}, false, 1, "", "SCORE: " + std::to_string(selectedScore), nullptr};
        }

        if (!_core->_gamePaths.empty()) {
            for (auto& [ game, _] : _core->_gamePaths) {
                std::string baseName = game;
                baseName = cropCopyName(baseName);
                if (_gameLogos.find(baseName) != _gameLogos.end())
                    _items[game + "_logo"] = {{960, 300}, {0.4f, 0.4f}, 0, {0, 0}, false, 99, _gameLogos.at(baseName), baseName, nullptr};
                else
                    _items[game + "_logo"] = {{960, 300}, {0.4f, 0.4f}, 0, {0, 0}, false, 99, _gameLogos.at("Unknown"), baseName, nullptr};
            }
            placeGameLogos();
        }
        _items["selectedOutline"] = {{960, 300}, {1.0f, 1.0f}, 0, {0, 0}, false, 200, "./assets/textures/menu/Selected_Outline.png", "", nullptr};
    }

    std::string Menu::getGlobalBestUser(const std::string& game)
    {
        std::string bestUser = "No User";

        int bestScore = getGlobalBestScore(game);

        if (bestScore == 0)
            return "";

        for (const auto& [user, scores] : _core->_allUserScores) {
            auto gameIt = scores.find(game);
            if (gameIt != scores.end() && gameIt->second == bestScore) {
                bestUser = user;
                break;
            }
        }

        return bestUser;
    }

    int Menu::getPersonalBestScore(const std::string& game)
    {
        auto userIt = _core->_allUserScores.find(_core->_currentUser);
        if (userIt != _core->_allUserScores.end()) {
            auto gameIt = userIt->second.find(game);
            if (gameIt != userIt->second.end())
                return gameIt->second;
        }
        return 0;
    }

     int Menu::getGlobalBestScore(const std::string& game)
    {
        int bestScore = 0;
        for (const auto& [user, scores] : _core->_allUserScores) {
            auto gameIt = scores.find(game);
            if (gameIt != scores.end() && gameIt->second > bestScore)
                bestScore = gameIt->second;
        }
        return bestScore;
    }


    void Menu::checkButtonHover(int x, int y, bool click)
    {
        for (auto& [name, item] : _items) {

            if (!item.buttonAction || name != "Quit_Button")
                continue;

            float displayWidth  = item.textureSize.first  * item.size.first;
            float displayHeight = item.textureSize.second * item.size.second;

            float left   = item.position.first  - displayWidth / 2.0f;
            float right  = item.position.first  + displayWidth / 2.0f;
            float top    = item.position.second - displayHeight / 2.0f;
            float bottom = item.position.second + displayHeight / 2.0f;

            bool isHovered =
                x >= left && x <= right &&
                y >= top  && y <= bottom;

            if (isHovered) {
                if (!item.isHovered) {
                    item.isHovered = true;
                    item.size.first  *= 1.1f;
                    item.size.second *= 1.1f;
                }
                if (click)
                    item.buttonAction();
            }
            else {
                if (item.isHovered) {
                    item.isHovered = false;
                    item.size.first  /= 1.1f;
                    item.size.second /= 1.1f;
                }
            }
        }
    }

    void Arc::Menu::switchRenderer(int orientation)
{
    if (_core->_rendererPaths.empty())
        return;

    if (orientation == 1) {
        _core->_rendererPaths.splice(_core->_rendererPaths.end(), _core->_rendererPaths, _core->_rendererPaths.begin());
        startArrowPulse("nextRendererButton");
    } else if (orientation == -1) {
        auto last = std::prev(_core->_rendererPaths.end());
        _core->_rendererPaths.splice(_core->_rendererPaths.begin(), _core->_rendererPaths, last);
        startArrowPulse("previousRendererButton");
    } else
        return;

    _chosenRenderer = _core->_rendererPaths.begin()->first;
    _items["Renderer_Name"].text = _chosenRenderer;
    
    
}

    void Menu::switchGame(int direction)
    {
         if (_core->_gamePaths.empty() || _slideGameLogos)
            return;
        if (direction == -1) {
            _core->_gamePaths.splice(_core->_gamePaths.end(), _core->_gamePaths, _core->_gamePaths.begin());
            startArrowPulse("previousGameButton");
        } else if (direction == 1) {
            auto last = std::prev(_core->_gamePaths.end());
            _core->_gamePaths.splice(_core->_gamePaths.begin(), _core->_gamePaths, last);
            startArrowPulse("nextGameButton");
        } else
            return;

        _chosenGame = cropCopyName(_core->_gamePaths.begin()->first);
        _items["Game_Name"].text = _chosenGame;
        _slideDirection = direction;
        _slideGameLogos = true;
        if (_chosenGame == "Tron") {
            _items["Tron_Blue_Score"].size = {30.0f, 30.0f};
            _items["Tron_Orange_Score"].size = {30.0f, 30.0f};
            _items["gameScore"].size = {0.0f, 0.0f};
        } else {
            _items["Tron_Blue_Score"].size = {0.0f, 0.0f};
            _items["Tron_Orange_Score"].size = {0.0f, 0.0f};
            _items["gameScore"].size = {30.0f, 30.0f};
         }

        int GlobalBest = getGlobalBestScore(_chosenGame);
        int PersonalBest = getPersonalBestScore(_chosenGame);
        std::string GlobalBestUser = getGlobalBestUser(_chosenGame);

        _items["globalBestScore"].text = GlobalBest == 0 ? "N/A" : std::to_string(GlobalBest);
        _items["personnalBestScore"].text = PersonalBest == 0 ? "N/A" : std::to_string(PersonalBest);
        _items["gameScore"].text = "SCORE: " + std::to_string(_core->_gameScores[_chosenGame]);
        _items["globalBestUser"].text = GlobalBestUser;
    }

    void Menu::placeGameLogos()
    {
        if (_core->_gamePaths.empty())
            return;
        for (const auto& game : _core->_gamePaths) {
            if (game.first == _core->_gamePaths.begin()->first) {
                _items[game.first + "_logo"].position = {960, 300};
                _items[game.first + "_logo"].size = {0.4f, 0.4f};
            }
            else if (_core->_gamePaths.size() > 1 && game.first == std::next(_core->_gamePaths.begin())->first) {
                _items[game.first + "_logo"].position = {660, 300};
                _items[game.first + "_logo"].size = {0.15f, 0.15f};
            }
            else if (_core->_gamePaths.size() > 1 && game.first == _core->_gamePaths.rbegin()->first) {
                _items[game.first + "_logo"].position = {1260, 300};
                _items[game.first + "_logo"].size = {0.15f, 0.15f};
            }
            else {
                _items[game.first + "_logo"].size = {0, 0};
            }
        }
    }

    void Arc::Menu::updateArrowPulse()
    {
        const auto now = std::chrono::steady_clock::now();

        if (_pulsedButton.empty())
            return;
        if (now >= _arrowPulseEnd) {

            _items[_pulsedButton].size = _originalArrowSize;
            _pulsedButton.clear();
        }
    }

    void Arc::Menu::startArrowPulse(const std::string& buttonName)
    {
        if (!_pulsedButton.empty() && _items.find(_pulsedButton) != _items.end())
            _items[_pulsedButton].size = _originalArrowSize;

        _pulsedButton = buttonName;
        _arrowPulseEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
        _originalArrowSize = _items[_pulsedButton].size;

        if (_pulsedButton == "previousGameButton" || _pulsedButton == "nextGameButton")
            _items[_pulsedButton].size = {0.25f, 0.25f};
        else if (_pulsedButton == "previousRendererButton" || _pulsedButton == "nextRendererButton")
            _items[_pulsedButton].size = {0.18f, 0.18f};
    }

    void Arc::Menu::moveSelector(int orientation)
    {  
        if (orientation == 1 && _selectorRow != 0)
            _selectorRow--;
        else if (orientation == -1 && _selectorRow != 2)
            _selectorRow++;
        
        switch (_selectorRow) {
            case 0:
                _items["selectedOutline"].position = {960, 300};
                _items["selectedOutline"].size = {1.0f, 1.0f};
                break;
            case 1:
                _items["selectedOutline"].position = {960, 700};
                _items["selectedOutline"].size = {0.75f, 0.5f};
                break;
            case 2:
                _items["selectedOutline"].position = {960, 1000};
                _items["selectedOutline"].size = {1.5f, 0.75f};
                break;
        }
    }
    
    void Arc::Menu::slideGameLogos()
    {
        if (_core->_gamePaths.size() < 3)
            return;

        
        const std::string activeGame = _core->_gamePaths.begin()->first;
        const std::string previousGame = std::next(_core->_gamePaths.begin())->first;
        const std::string nextGame = _core->_gamePaths.rbegin()->first;

        _items[activeGame + "_logo"].drawOrder = 102;
        if (_slideDirection == 1) {
            _items[previousGame + "_logo"].drawOrder = 100;
            _items[nextGame + "_logo"].drawOrder = 101;
        } else if (_slideDirection == -1) {
            _items[previousGame + "_logo"].drawOrder = 101;
            _items[nextGame + "_logo"].drawOrder = 100;
        }
        slideUniqueLogo(activeGame + "_logo", 8.0f, 960, 300, 0.4f, 0.4f);
        slideUniqueLogo(previousGame + "_logo", 8.0f, 660, 300, 0.15f, 0.15f);
        slideUniqueLogo(nextGame + "_logo", 8.0f, 1260, 300, 0.15f, 0.15f);

        for (const auto& game : _core->_gamePaths) {
            if (game.first != activeGame && game.first != previousGame && game.first != nextGame) {
                _items[game.first + "_logo"].size = {0, 0};
                _items[game.first + "_logo"].drawOrder = 99;
            }
        }

        if (_items[activeGame + "_logo"].position == std::make_pair(960.0f, 300.0f) &&
            _items[previousGame + "_logo"].position == std::make_pair(660.0f, 300.0f) &&
            _items[nextGame + "_logo"].position == std::make_pair(1260.0f, 300.0f)) {
            _slideGameLogos = false;
        }
    }

    void Arc::Menu::slideUniqueLogo(const std::string& logo, int speed, float targetX, float targetY, float targetSizeX, float targetSizeY)
    {
        Item& item = _items[logo];

        if (speed < 0.0f)
            speed = 0.0f;
        if (speed > 1.0f)
            speed = 1.0f;

        item.position.first += (targetX - item.position.first) * speed * 0.2f;
        item.position.second += (targetY - item.position.second) * speed * 0.2f;
        item.size.first += (targetSizeX - item.size.first) * speed * 0.2f;
        item.size.second += (targetSizeY - item.size.second) * speed * 0.2f;

        if (std::abs(targetX - item.position.first) < 0.5f)
            item.position.first = targetX;
        if (std::abs(targetY - item.position.second) < 0.5f)
            item.position.second = targetY;
        if (std::abs(targetSizeX - item.size.first) < 0.001f)
            item.size.first = targetSizeX;
        if (std::abs(targetSizeY - item.size.second) < 0.001f)
            item.size.second = targetSizeY;
    }

}
