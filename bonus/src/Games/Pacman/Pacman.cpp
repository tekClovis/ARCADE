/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** pacman
*/

#include "Pacman.hpp"

namespace {
    std::pair<float, float> gridToScreen(std::pair<float, float> grid)
    {
        return {
            ORIGIN_X + (grid.first * CELL_SIZE),
            ORIGIN_Y + (grid.second * CELL_SIZE)
        };
    }
}

Arc::Pacman::Pacman(std::map<std::string, Arc::Item>& items) : _items(items)
{
    _score = 0;
    _map = PacmanMap();
    _player = std::make_shared<PacmanPlayer>();
    _ghosts.push_back(std::make_unique<Blinky>(_player));
    _ghosts.push_back(std::make_unique<Pinky>(_player));
    _ghosts.push_back(std::make_unique<Inky>(_player));
    _ghosts.push_back(std::make_unique<Clyde>(_player));
    _displayPoints = {{0, 0}, {DEFAULT_SCALE, DEFAULT_SCALE}, 0, {0, 0}, false, 30, "./assets/textures/pacman/point/1.png", "200", nullptr};
    _startClock = std::chrono::steady_clock::now();
    _endLevelClock = std::chrono::steady_clock::now();
    rebuildItems();
}

void Arc::Pacman::handleKey(Event event)
{
    if (event.type != Type::KeyPressed)
        return;
        
    Key key = event.code;
    
    if (key == Key::Enter && _gameOver)
    {
        if (_outlinePos == "Restart")
            restartGame();
        if (_outlinePos == "Menu")
            _backToMenu = true;
    }

    if (_gameOver) {
        if (key == Key::Up && _outlinePos == "Menu")
            _outlinePos = "Restart";
        if (key == Key::Down && _outlinePos == "Restart")
            _outlinePos = "Menu";
        return;
    }

    switch (event.code){
        case Key::Q:
            _player->setNextDirection(DIRECTION_LEFT);
            break;
        case Key::D:
            _player->setNextDirection(DIRECTION_RIGHT);
            break;
        case Key::Z:
            _player->setNextDirection(DIRECTION_UP);
            break;
        case Key::S:
            _player->setNextDirection(DIRECTION_DOWN);
            break;
        default:
            break;
    }
}

void Arc::Pacman::update(Event event)
{
    if (event.type == Type::Quit)
        return;
    
    if (_gameOver) {
        rebuildItems();
        return;
    }

    if (_displayPoints.position.first != 0 || _displayPoints.position.second != 0) {
        rebuildItems();
        return;
    }
    if (handleStart()) {
        rebuildItems();
        return;
    }
    _player->update(_map);
    if (_player->getEndLevel()) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _endLevelClock).count() >= 3000) {
            _level++;
            _map.set(_level);
            _player->reset();
            for (auto& ghost : _ghosts)
                ghost->reset();
            _startClock = std::chrono::steady_clock::now();
            _endLevelClock = std::chrono::steady_clock::now();
            _starting = true;
            _player->reset();
        }
        rebuildItems();
        return;
    }
    if (_player->getDie()) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _dieClock).count() >= 2500) {
            _player->reset();
            for (auto& ghost : _ghosts){
                ghost->reset();
                ghost->getSpeed() *= 1.05;
            }
            _player->getSpeed() *= 1.05;
            _player->getDie() = false;
            _player->getLife()--;
            _startClock = std::chrono::steady_clock::now() - std::chrono::seconds(3);
            _starting = true;
            if (_player->getLife() == 0)
                _gameOver = true;
        }
        rebuildItems();
        return;
    }

    for (auto& ghost : _ghosts)
        ghost->update(_map);


    if (_player->getBonus()) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - _bonusTime).count() >= 10) {
            _player->getBonus() = false;
            for (auto& ghost : _ghosts)
                ghost->getBonus() = false;
        }
    }

    for (auto& ghost : _ghosts) {
        if (ghost && (static_cast<IEntity&>(*ghost) == static_cast<IEntity&>(*_player))) {
            if (ghost->getDie())
                continue;
            if (ghost->getBonus()) {
                ghost->getDie() = true;
                ghost->getBonus() = false;
                _score += 200 * (1 << _phantomEat);
                _phantomEat++;
                _displayPoints = {gridToScreen({ghost->getPosition().first + 1, ghost->getPosition().second - 1})
                    , {DEFAULT_SCALE, DEFAULT_SCALE}, 0, {0, 0}, false, 30,
                    "./assets/textures/pacman/point/" + std::to_string(_phantomEat) + ".png", std::to_string(200 * (1 << (_phantomEat - 1))), nullptr};
                ghost->update(_map);
                _pointClock = std::chrono::steady_clock::now();
            } else {
                _player->getDie() = true;
                _dieClock = std::chrono::steady_clock::now();
            }
        }
    }

    if (_map(std::round(_player->getPosition().first), std::round(_player->getPosition().second)) == 2) {
        _map(std::round(_player->getPosition().first), std::round(_player->getPosition().second)) = 0;
        _score += 10;
    }
    if (_map(std::round(_player->getPosition().first), std::round(_player->getPosition().second)) == 3) {
        _map(std::round(_player->getPosition().first), std::round(_player->getPosition().second)) = 0;
        _score += 50;
        _player->getBonus() = true;
        _bonusTime = std::chrono::steady_clock::now();
        _phantomEat = 0;
        for (auto& ghost : _ghosts){
            ghost->getBonus() = true;
            ghost->setBonusAnimTime() = std::chrono::steady_clock::now();
        }
    }
    if (_map.countPoints() == 0) {
        _player->getEndLevel() = true;
        _endLevelClock = std::chrono::steady_clock::now();
        return;
    }
    rebuildItems();
}

void Arc::Pacman::rebuildItems(){
    for (auto it = _items.begin(); it != _items.end();) {
        if (it->first.rfind("Pacman_", 0) == 0 || (!_gameOver && it->first == "outlineSelector"))
            it = _items.erase(it);
        else
            ++it;
    }

    _map.draw(_items);
    
    if (_gameOver) {
        _items["Pacman_GameOver"] = {{960, 540 + CELL_SIZE * 2.0}, {DEFAULT_SCALE * 1.05, DEFAULT_SCALE * 1.05}, 0, {0, 0}, false, 30, "./assets/textures/pacman/gameOver.png", "GAME OVER", nullptr};
        _items["Pacman_ScoreLabel"] = {{275, 100}, {35, 35}, 0, {0, 0}, false, 50, "", "Your\nScore:", nullptr};
        _items["Pacman_ScoreValue"] = {{275, 220}, {30, 30}, 0, {0, 0}, false, 50, "", std::to_string(_score), nullptr};

        _items["Pacman_Restart_Text"] = {{275, 500}, {40.0f, 40.0f}, 0, {0, 0}, false, 50, "", "RESTART", nullptr};
        _items["Pacman_BackToMenu_Text"] = {{275, 600}, {40.0f, 40.0f}, 0, {0, 0}, false, 50, "", "MENU", nullptr};

        _items["outlineSelector"] = {_outlinePositions[_outlinePos], {0.8f, 0.2f}, 0, {0, 0}, false, 60, "./assets/textures/menu/Selected_Outline.png", "", nullptr};
        return;
    }
    
    
    _items["Pacman_LevelLabel"] = {{275, 100}, {35, 35}, 0, {0, 0}, false, 50, "", "Level", nullptr};
    _items["Pacman_LevelValue"] = {{275, 150}, {35, 35}, 0, {0, 0}, false, 50, "", std::to_string(_level), nullptr};
    _items["Pacman_LivesLabel"] = {{275, 300}, {35, 35}, 0, {0, 0}, false, 50, "", "Lives", nullptr};
    _items["Pacman_LivesValue"] = {{275, 350}, {35, 35}, 0, {0, 0}, false, 50, "", std::to_string(_player->getLife()), nullptr};
    _items["Pacman_ScoreLabel"] = {{275, 500}, {35, 35}, 0, {0, 0}, false, 50, "", "Score", nullptr};
    _items["Pacman_ScoreValue"] = {{275, 550}, {30, 30}, 0, {0, 0}, false, 50, "", std::to_string(_score), nullptr};
    
    if (_starting)
    _items["Pacman_Ready"] = {{960, 540 + CELL_SIZE * 2.0}, {DEFAULT_SCALE, DEFAULT_SCALE}, 0, {0, 0}, false, 30, "./assets/textures/pacman/ready.png", "READY", nullptr};
    if (_gameOver)
    _items["Pacman_GameOver"] = {{960, 540}, {48, 48}, 0, {0, 0}, false, 30, "", "GAME OVER", nullptr};
    
    if (!_player->getDie() && !_player->getEndLevel()){
        for (std::size_t i = 0; i < _ghosts.size(); ++i) {
            AGhost* ghost = dynamic_cast<AGhost*>(_ghosts[i].get());
            if (!ghost)
                continue;
            const auto ghostPos = gridToScreen(ghost->getPosition());
            _items["Pacman_Ghost_" + std::to_string(i)] = {
                ghostPos,
                {DEFAULT_SCALE, DEFAULT_SCALE},
                0,
                {0, 0},
                false,
                11,
                ghost->getTexturePath(),
                "G",
                nullptr
            };
        }
    }
    const auto playerPos = gridToScreen(_player->getPosition());
    _items["Pacman_Player"] = {
        playerPos,
        {DEFAULT_SCALE, DEFAULT_SCALE},
        0,
        {0, 0},
        false,
        12,
        _player->getTexturePath(),
        "C",
        nullptr
    };
    if (_displayPoints.position !=  std::make_pair<float, float>(0.0, 0.0)){
        if (std::chrono::steady_clock::now() - _pointClock >= std::chrono::milliseconds(400)){
            _displayPoints.position = {0, 0};
            _bonusTime += std::chrono::milliseconds(400);
             for (auto& ghost : _ghosts)
                if (ghost->getBonus())
                    ghost->setBonusAnimTime() += std::chrono::milliseconds(400);
        } else {
            _items["Pacman_DisplayPoints"] = _displayPoints;
        }
    }
}

bool Arc::Pacman::handleStart()
{
    if (!_starting)
        return false;
    if (_startClock + std::chrono::seconds(4) < std::chrono::steady_clock::now()) {
        _starting = false;
        _player->reset();
        for (auto& ghost : _ghosts)
            ghost->reset();
    }
    return true;
}

void Arc::Pacman::restartGame()
{
    _score = 0;
    _level = 1;
    _gameOver = false;
    _backToMenu = false;
    _map = PacmanMap();
    _player = std::make_shared<PacmanPlayer>();
    _ghosts.clear();
    _ghosts.push_back(std::make_unique<Blinky>(_player));
    _ghosts.push_back(std::make_unique<Pinky>(_player));
    _ghosts.push_back(std::make_unique<Inky>(_player));
    _ghosts.push_back(std::make_unique<Clyde>(_player));
    _displayPoints = {{0, 0}, {DEFAULT_SCALE, DEFAULT_SCALE}, 0, {0, 0}, false, 30, "./assets/textures/pacman/point/1.png", "200", nullptr};
    _startClock = std::chrono::steady_clock::now();
    _endLevelClock = std::chrono::steady_clock::now();
    _starting = true;
    _items.erase("outlineSelector");
    rebuildItems();
}


extern "C" {
    Arc::IGame* entryPoint(std::map<std::string, Arc::Item>& items) {
        return new Arc::Pacman(items);
    }

    int getLibraryType() {
        return static_cast<int>(Arc::LibraryType::Game);
    }

    std::string getName()
    {
        return "Pacman";
    }
}