/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Snake
*/

#include "Snake.hpp"
#include <algorithm>

namespace Arc {

    Snake::Snake(std::map<std::string, Item>& items)
        :_items(items), _score(0), _gameState(START), _selected_item(0), _delay(BASE_SPEED), _isBoosted(false), _boostMax(false)
    {
        _map = std::make_unique<SnakeMap>(_items);
        _player = std::make_unique<SnakePlayer>(_items);
        _fruits.push_back(std::make_unique<BasicFruit>(_items, "./assets/textures/Snake/fruits/apple.png", "Apple"));
        _fruits.push_back(std::make_unique<TemporaryFruit>(_items, "./assets/textures/Snake/fruits/cherry.png", "Cherry", 10000));
        _items["score"] = {{290, 200}, {35.0f, 35.0f}, 0, {0, 0}, false, 3, "", "Score: 0", nullptr};
        _items["play"] = {{290, 400}, {40.0f, 40.0f}, 0, {0, 0}, true, 3, "", "Play", nullptr};
        _items["restart"] = {{290, 400}, {0.0f, 0.0f}, 0, {0, 0}, false, -15, "", "Restart", nullptr};
        _items["quit"] = {{290, 500}, {0.0f, 0.0f}, 0, {0, 0}, false, -15, "", "Quit", nullptr};
        _items["selector"] = {{290, 400}, {0.8f, 0.25f}, 0, {0, 0}, false, 4, "./assets/textures/menu/Selected_Outline.png", "", nullptr};
        _items["winLine"] = {{970, 1050}, {45.0f, 45.0f}, 0, {0, 0}, false, 3, "", "", nullptr};
        _items["controls"] = {{1650, 200}, {35.0f, 35.0f}, 0, {0, 0}, false, 3, "", "Arrows\nto move", nullptr};
        _items["rushAdvice"] = {{1650, 400}, {35.0f, 35.0f}, 0, {0, 0}, false, 3, "", "Press\nSpace\nto rush", nullptr};
        _items["selectionAdvice"] = {{290, 700}, {35.0f, 35.0f}, 0, {0, 0}, false, 3, "", "Select\noption\nwith\nEnter", nullptr};

        for (auto& fruit : _fruits)
            fruit->spawn(_player->getBodyPositions());
        _lastUpdate = std::chrono::steady_clock::now();
    }

    Snake::~Snake()
    {
    }

    void Snake::restart() {
        for (auto it = _items.begin(); it != _items.end(); ) {
            if (it->first.rfind("snake", 0) == 0)
                it = _items.erase(it);
            else
                it++;
        }
        _score = 0;
        _delay = BASE_SPEED;
        _items["score"].text = "Score: 0";
        _player = std::make_unique<SnakePlayer>(_items);
        _fruits.clear();
        _fruits.push_back(std::make_unique<BasicFruit>(_items, "./assets/textures/Snake/fruits/apple.png", "Apple"));
        _fruits.push_back(std::make_unique<TemporaryFruit>(_items, "./assets/textures/Snake/fruits/cherry.png", "Cherry", 10000));
        for (auto& fruit : _fruits)
            fruit->spawn(_player->getBodyPositions());
        _gameState = START;
        _items["play"].drawOrder = 3;
        _items["play"].position = {290, 400};
        _items["restart"].position = {960, 540};
        _items["quit"].position = {960, 540};
        _items["selector"].drawOrder = 4;
        _items["winLine"].text = "";
        _isBoosted = false;
        _boostMax = false;
    }

    void Snake::update(Event event) {
        if (event.type == Type::Quit || _gameState == QUIT)
            return;
        if (_delay <= 50 && !_isBoosted) {
            _delay = 50;
            _boostMax = true;
        }

        if (_gameState == PLAYING) {
            _items["play"].drawOrder = -15;
            _items["play"].position = {960, 540};
            _items["play"].size = {0.0f, 0.0f};
            _items["selector"].drawOrder = -15;
            _items["selector"].position = {960, 540};
            _items["winLine"].text = "";
            auto now = std::chrono::steady_clock::now();
            if (_isBoosted && now > _boostTimer && !_boostMax) {
                _delay *= 2;
                _isBoosted = false;
            }
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastUpdate).count();

            if (elapsed > _delay) {
                _player->update();

                if (_player->getBodyPositions().size() == static_cast<size_t>(_map->getMapSize())) {
                    _items["winLine"].text = "You Won!";
                    _gameState = ENDGAME;
                    return;
                }

                if (!_player->isAlive())  {
                    _items["winLine"].text = "You Lost!";
                    _gameState = ENDGAME;
                    return;
                }

                for (auto& fruit : _fruits) {
                    if (!fruit->isExpired() && _player->checkCollision(fruit->getPosition())) {
                        updateScore(_player->eatFruit(*fruit));
                        if (_boostMax)
                            continue;
                        _delay -= (_isBoosted) ? 1 : 2;
                    }
                    fruit->update(_player->getBodyPositions());
                }

                _fruits.erase(std::remove_if(_fruits.begin(), _fruits.end(),
                [](const std::unique_ptr<IFruit>& fruit) { return fruit->isExpired(); }), _fruits.end());

                spawnRandomTemporaryFruit();
                _lastUpdate = now;
            }
        } else if (_gameState == ENDGAME) {
            _items["restart"].position = {290, 400};
            _items["quit"].position = {290, 500};
            _items["restart"].size = {40.0f, 40.0f};
            _items["quit"].size = {40.0f, 40.0f};
            _items["selector"].drawOrder = 4;
            _items["selector"].position = {290, 400};
            _items["play"].size = {0.0f, 0.0f};

            if (_selected_item == 0)
                _items["selector"].position = {290, 400};
            else
                _items["selector"].position = {290, 500};

        } else if (_gameState == START) {
            _items["restart"].position = {960, 540};
            _items["restart"].size = {0.0f, 0.0f};
            _items["quit"].position = {960, 540};
            _items["quit"].size = {0.0f, 0.0f};
            _items["play"].position = {290, 400};
            _items["play"].size = {40.0f, 40.0f};
            _items["selector"].drawOrder = 4;
            _items["selector"].position = {290, 400};
            _items["winLine"].text = "";
        }
    }

    void Snake::spawnRandomTemporaryFruit() {
        size_t totalTiles = static_cast<size_t>(_map->getMapSize());
        size_t freeTiles = totalTiles - _player->getBodyPositions().size() - _fruits.size();

        if (freeTiles <= 10)
            return;

        if (std::rand() % 100 >= 2)
            return;
        for (const auto& fruit : _fruits) {
            if (fruit->getType() == IFruit::TEMPORARY)
                return;
        }

        const std::vector<IFruit::FruitData> possibleFruits = {
            {"Cherry", "./assets/textures/Snake/fruits/cherry.png", 3000},
            {"Banana", "./assets/textures/Snake/fruits/banana.png", 5000},
            {"Orange", "./assets/textures/Snake/fruits/orange.png", 10000},
            {"Grapes", "./assets/textures/Snake/fruits/grapes.png", 7000}
        };
        int index = std::rand() % possibleFruits.size();
        const auto& randomFruit = possibleFruits[index];

        _fruits.push_back(std::make_unique<TemporaryFruit>
            (_items, randomFruit.path, randomFruit.name, randomFruit.time));
        _fruits.back()->spawn(_player->getBodyPositions());
    }

    void Snake::handleKey(Event event) {
        if (event.type != Type::KeyPressed)
            return;
        if (_gameState == START && event.code == Key::Enter) {
            _gameState = PLAYING;
            return;
        }
        if (_gameState == ENDGAME) {
            if (event.code == Key::Z)
                _selected_item = (_selected_item + 1) % 2;
            if (event.code == Key::S)
                _selected_item = (_selected_item - 1) % 2;
            if (event.code == Key::Enter) {
                if (_selected_item == 0)
                    restart();
                else
                    _gameState = QUIT;
            }
        }
        if (_gameState == PLAYING) {
            if (event.code == Key::Space && !_isBoosted && !_boostMax) {
                _delay /= 2;
                _isBoosted = true;
                _boostTimer = std::chrono::steady_clock::now() + std::chrono::seconds(5);
            }
            _player->handleKey(event);
        }
    }

    void Snake::updateScore(int points) {
        _score += points;
        _items["score"].text = "Score: " + std::to_string(_score);
    }
}

extern "C" {

    Arc::IGame* entryPoint(std::map<std::string, Arc::Item>& items) {
        return new Arc::Snake(items);
    }

    int getLibraryType() {
        return static_cast<int>(Arc::LibraryType::Game);
    }

    std::string getName() {
        return "Snake";
    }
}
