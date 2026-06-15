/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Tron
*/

#include "Tron.hpp"
#include <algorithm>

namespace Arc {

    Tron::Tron(std::map<std::string, Item>& items)
        : _items(items)
    {

        _setNewScore = false;
        _gameOver = false;
        _backToMenu = false;
        _BlueDead = false;
        _OrangeDead = false;
        _blueDirX = -1;
        _blueDirY = 0;
        _orangeDirX = 1;
        _orangeDirY = 0;
        _lastMoveTime = std::chrono::steady_clock::now();
        _blueScore = 0;
        _orangeScore = 0;
        _blueSquares = {};
        _orangeSquares = {};
        activateGame = false;
        gameActivated = false;

        _items["Player1text"] = {{460, 300}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "Player 1", nullptr};
        _items["Player2text"] = {{1460, 300}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "Player 2", nullptr};
        _items["Z"] = {{460, 500}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "Z", nullptr};
        _items["S"] = {{460, 600}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "S", nullptr};
        _items["Q"] = {{360, 600}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "Q", nullptr};
        _items["D"] = {{560, 600}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "D", nullptr};
        _items["ArrowUp"] = {{1460, 500}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "UP", nullptr};
        _items["ArrowDown"] = {{1460, 700}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "DOWN", nullptr};
        _items["ArrowLeft"] = {{1260, 600}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "LEFT", nullptr};
        _items["ArrowRight"] = {{1660, 600}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "RIGHT", nullptr};
        _items["Start_Text"] = {{960, 880}, {40.0f, 40.0f}, 0, {0, 0}, false, 10, "", "ENTER TO START", nullptr};

    }
    

    Tron::~Tron()
    {

    }
    
    void Tron::createGame()
    {
        _items.clear();
        _items["Background"] = {{960, 540}, {1.0f, 1.0f}, 0, {0, 0}, false, -1, "./assets/textures/menu/Menu_Background.png", "", nullptr};
        _items["joystick_1"] = {{960, 540}, {1, 1}, 0, {0, 0}, false, -1, "./assets/textures/menu/joysticks/joystick1_idle.png", " ", std::function<void()>()};
        _items["joystick_2"] = {{960, 540}, {1, 1}, 0, {0, 0}, false, -1, "./assets/textures/menu/joysticks/joystick2_idle.png", " ", std::function<void()>()};

        float blueStartX = Outline_startX + (squarePerRow - 1) * cellSize;
        float blueStartY = Outline_startY + (squarePerRow / 2) * cellSize;
        float orangeStartX = Outline_startX;
        float orangeStartY = Outline_startY + (squarePerRow / 2) * cellSize;
    
        _items["MapOutline"] = {{960, 540}, {5.0f, 5.0f}, 0, {0, 0}, false, 1, "./assets/textures/Tron/Tron_Map_Outline.png", "", nullptr};
        _items["Blue_Player"] = {{blueStartX, blueStartY}, {0.1f, 0.1f}, 0, {0, 0}, false, 2, "./assets/textures/Tron/Tron_Blue_Player.png", "", nullptr};
        _items["Orange_Player"] = {{orangeStartX, orangeStartY}, {0.1f, 0.1f}, 180, {0, 0}, false, 2, "./assets/textures/Tron/Tron_Orange_Player.png", "", nullptr};
        _items["Winner_Text"] = {{960, 300}, {100.0f, 100.0f}, 0, {0, 0}, false, 3, "", "", nullptr};
        _items["Restart_Text"] = {{660, 540}, {0.0f, 0.0f}, 0, {0, 0}, false, 1, "", "RESTART", nullptr};
        _items["BackToMenu_Text"] = {{1260, 540}, {0.0f, 0.0f}, 0, {0, 0}, false, 3, "", "MENU", nullptr};
        _items["Blue_Score_Text"] = {{1660, 150}, {40.0f, 40.0f}, 0, {0, 0}, false, 3, "", "BLUE", nullptr};
        _items["Orange_Score_Text"] = {{260, 150}, {40.0f, 40.0f}, 0, {0, 0}, false, 3, "", "ORANGE", nullptr};
        _items["Blue_Score"] = {{1660, 200}, {40.0f, 40.0f}, 0, {0, 0}, false, 3, "", std::to_string(_blueScore), nullptr};
        _items["Orange_Score"] = {{260, 200}, {40.0f, 40.0f}, 0, {0, 0}, false, 3, "", std::to_string(_orangeScore), nullptr};
        _items["outlineSelector"] = {{660, 540}, {0.0f, 0.0f}, 0, {0, 0}, false, 10, "./assets/textures/menu/Selected_Outline.png", "", nullptr};
    
        for (size_t y = 0; y < static_cast<size_t>(squarePerRow); y++) {
            for (size_t x = 0; x < static_cast<size_t>(squarePerRow); x++) {
                std::string textureName = "./assets/textures/Tron/Black_Square.png";
                _items["Square_" + std::to_string(y * squarePerRow + x)] = {{x * cellSize + Outline_startX, y * cellSize + Outline_startY}, {0.126f, 0.126f}, 0, {0, 0}, false, 0, textureName, ".", nullptr};
            }
        }
        snapPlayerToGrid("Blue_Player");
        snapPlayerToGrid("Orange_Player");
    }

    void Tron::update(Event event)
    {
        if (event.type == Type::Quit)
            return;

        const auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - _lastMoveTime).count();
        _lastMoveTime = now;
        deltaTime = std::clamp(deltaTime, 0.0f, 0.05f);

        const float step = SPEED * deltaTime;

        if (activateGame) {
            createGame();
            activateGame = false;
            gameActivated = true;
        }

        if (!gameActivated)
            return;

        if (!_gameOver) {
            movePlayer("Blue_Player", static_cast<float>(_blueDirX) * step, static_cast<float>(_blueDirY) * step);
            movePlayer("Orange_Player", static_cast<float>(_orangeDirX) * step, static_cast<float>(_orangeDirY) * step);
            checkCollision();
            paintSquareUnderPlayer("Blue_Player", "./assets/textures/Tron/Blue_Square.png");
            paintSquareUnderPlayer("Orange_Player", "./assets/textures/Tron/Orange_Square.png");
        }

        if (_gameOver && _setNewScore) {
            if (_BlueDead && _OrangeDead)
                _items["Winner_Text"].text = "DRAW";
            else if (_BlueDead)
            {
                _orangeScore++;
                _items["Orange_Score"].text = std::to_string(_orangeScore);
                _items["Winner_Text"].text = "WINNER : ORANGE";
            } else if (_OrangeDead) {
                _blueScore++;
                _items["Blue_Score"].text = std::to_string(_blueScore);
                _items["Winner_Text"].text = "WINNER : BLUE";
            }
            _setNewScore = false;
            if (_gameOver) {
                _items["BackToMenu_Text"].size = {40.0f, 40.0f};
                _items["Restart_Text"].size = {40.0f, 40.0f};
                _items["outlineSelector"].size = {0.8f, 0.4f};
            }
        }

    }

    std::pair<float, float> Tron::getCell(const std::string& playerName) const
    {
        const float outlineStartX = 455.0f;
        const float outlineStartY = 35.0f;

        std::pair<float, float> position = _items.at(playerName).position;
        int cellX = static_cast<int>(std::lround((position.first - outlineStartX) / cellSize));
        int cellY = static_cast<int>(std::lround((position.second - outlineStartY) / cellSize));
        cellX = std::clamp(cellX, 0, 31);
        cellY = std::clamp(cellY, 0, 31);
        return std::pair<float, float>{
            outlineStartX + static_cast<float>(cellX) * cellSize,
            outlineStartY + static_cast<float>(cellY) * cellSize
        };
    }

    void Tron::checkCollision() {
        const std::pair<float, float> blueCell = getCell("Blue_Player");
        const std::pair<float, float> orangeCell = getCell("Orange_Player");

        if (blueCell == orangeCell) {
            _setNewScore = true;
            _gameOver = true;
            _BlueDead = true;
            _OrangeDead = true;
            return;
        }

        if (hitsTrail(_blueSquares, orangeCell, false) || hitsTrail(_orangeSquares, orangeCell, true)) {
            _setNewScore = true;
            _gameOver = true;
            _OrangeDead = true;
            return;
        }
        if (hitsTrail(_orangeSquares, blueCell, false) || hitsTrail(_blueSquares, blueCell, true)) {
            _setNewScore = true;
            _gameOver = true;
            _BlueDead = true;
            return;
        }
    }

    bool Tron::hitsTrail(const std::list<std::pair<float, float>>& trail, const std::pair<float, float>& cell, bool ignoreHead) const
    {
        if (trail.empty())
            return false;
        auto begin = trail.begin();
        auto end = trail.end();
        if (ignoreHead)
            end = std::prev(trail.end());
        return std::find(begin, end, cell) != end;
    }

    void Tron::movePlayer(const std::string& playerName, float moveX, float moveY) {

        const float outlineStartX = 455.0f;
        const float outlineStartY = 35.0f;
        const float minX = outlineStartX;
        const float maxX = outlineStartX + (31.0f * cellSize);
        const float minY = outlineStartY;
        const float maxY = outlineStartY + (31.0f * cellSize);

        std::pair<float, float> position = _items.at(playerName).position;
        const float targetX = position.first + moveX;
        const float targetY = position.second + moveY;

        if (targetX < minX || targetX > maxX || targetY < minY || targetY > maxY) {
            _setNewScore = true;
            _gameOver= true;
            if (playerName == "Blue_Player")
                _BlueDead = true;
            else if (playerName == "Orange_Player")
                _OrangeDead = true;
            return;
        }

        float nextX = std::clamp(targetX, minX, maxX);
        float nextY = std::clamp(targetY, minY, maxY);
        _items.at(playerName).position = {nextX, nextY};
    }

    void Tron::updateDirection(const std::string& playerName, Key key)
    {
        int& dirX = playerName == "Blue_Player" ? _blueDirX : _orangeDirX;
        int& dirY = playerName == "Blue_Player" ? _blueDirY : _orangeDirY;
        int newX = dirX;
        int newY = dirY;
        float rotation = 0.0f;

        if (key == (playerName == "Blue_Player" ? Key::Left : Key::Q)) {
            newX = -1;
            newY = 0;
            rotation = 0.0f;
        } else if (key == (playerName == "Blue_Player" ? Key::Right : Key::D)) {
            newX = 1;
            newY = 0;
            rotation = 180.0f;
        } else if (key == (playerName == "Blue_Player" ? Key::Up : Key::Z)) {
            newX = 0;
            newY = -1;
            rotation = 90.0f;
        } else if (key == (playerName == "Blue_Player" ? Key::Down : Key::S)) {
            newX = 0;
            newY = 1;
            rotation = -90.0f;
        } else {
            return;
        }

        if ((dirX == newX && dirY == newY) || (dirX == -newX && dirY == -newY))
            return;

        snapPlayerToGrid(playerName);
        dirX = newX;
        dirY = newY;
        _items.at(playerName).texturePath = playerName == "Blue_Player" ? "./assets/textures/Tron/Tron_Blue_Player.png" : "./assets/textures/Tron/Tron_Orange_Player.png";
        _items.at(playerName).rotation = rotation;
    }

    void Tron::snapPlayerToGrid(const std::string& playerName) {
        const float outlineStartX = 455.0f;
        const float outlineStartY = 35.0f;
        std::pair<float, float> position = _items.at(playerName).position;

        float gridX = std::round((position.first - outlineStartX) / cellSize);
        float gridY = std::round((position.second - outlineStartY) / cellSize);
        float snappedX = outlineStartX + (gridX * cellSize);
        float snappedY = outlineStartY + (gridY * cellSize);

        _items.at(playerName).position = {snappedX, snappedY};
    }

    void Tron::paintSquareUnderPlayer(const std::string& playerName, const std::string& texturePath) {
        const float outlineStartX = 455.0f;
        const float outlineStartY = 35.0f;

        std::pair<float, float> position = _items.at(playerName).position;
        int cellX = static_cast<int>(std::lround((position.first - outlineStartX) / cellSize));
        int cellY = static_cast<int>(std::lround((position.second - outlineStartY) / cellSize));

        cellX = std::clamp(cellX, 0, squarePerRow - 1);
        cellY = std::clamp(cellY, 0, squarePerRow - 1);

        std::string squareName = "Square_" + std::to_string(cellY * squarePerRow + cellX);
        _items.at(squareName).texturePath = texturePath;
        _items.at(squareName).text = (playerName == "Blue_Player") ? "b" : "o";

        const std::pair<float, float> currentCell = {
            outlineStartX + cellX * cellSize,
            outlineStartY + cellY * cellSize
        };

        if (playerName == "Blue_Player") {
            if (_blueSquares.empty() || _blueSquares.back() != currentCell)
                _blueSquares.push_back(currentCell);
        } else if (playerName == "Orange_Player") {
            if (_orangeSquares.empty() || _orangeSquares.back() != currentCell)
                _orangeSquares.push_back(currentCell);
        }
    }

    void Tron::handleKey(Event event)
    {
        if (event.type != Type::KeyPressed)
            return;
        Key key = event.code;
        
        if (key == Key::Enter && !gameActivated && !_gameOver) {
            activateGame = true;
            return;
        }
        
        if (key == Key::Enter && _gameOver)
        {
            if (_items["outlineSelector"].position == _outlinePositions["Restart"])
                restartGame();
            else if (_items["outlineSelector"].position == _outlinePositions["Menu"])
                _backToMenu = true;
            return;
        }

        if (_gameOver ) {
            if (key == Key::Q && _items["outlineSelector"].position == _outlinePositions["Menu"])
                _items["outlineSelector"].position = _outlinePositions["Restart"];
            else if (key == Key::D && _items["outlineSelector"].position == _outlinePositions["Restart"])
                _items["outlineSelector"].position = _outlinePositions["Menu"];
            return;
        }

        switch (key) {
            case Key::Left:
                updateDirection("Blue_Player", key);
                break;
            case Key::Right:
                updateDirection("Blue_Player", key);
                break;
            case Key::Up:
                updateDirection("Blue_Player", key);
                break;
            case Key::Down:
                updateDirection("Blue_Player", key);
                break;
            case Key::Q:
                updateDirection("Orange_Player", key);
                break;
            case Key::D:
                updateDirection("Orange_Player", key);
                break;
            case Key::Z:
                updateDirection("Orange_Player", key);
                break;
            case Key::S:
                updateDirection("Orange_Player", key);
                break;
            default:
                break;
        }
    }

    void Tron::restartGame() {
        _setNewScore = false;
        _gameOver = false;
        _backToMenu = false;
        _BlueDead = false;
        _OrangeDead = false;
        _blueDirX = -1;
        _blueDirY = 0;
        _orangeDirX = 1;
        _orangeDirY = 0;
        _blueSquares.clear();
        _orangeSquares.clear();

        createGame();
        gameActivated = true;
    }

}

extern "C" {

    Arc::IGame* entryPoint(std::map<std::string, Arc::Item>& items) {
        return new Arc::Tron(items);
    }

    int getLibraryType() {
        return static_cast<int>(Arc::LibraryType::Game);
    }

    std::string getName()
    {
        return "Tron";
    }
}
