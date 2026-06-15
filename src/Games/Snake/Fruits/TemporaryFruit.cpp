/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** TemporaryFruit
*/

#include "TemporaryFruit.hpp"
#include "SnakeMap.hpp"
#include <algorithm>
#include <random>

namespace Arc {

    TemporaryFruit::TemporaryFruit(std::map<std::string, Item>& items, std::string texturePath, std::string name, int lifeTime)
        : _items(items), _texturePath(texturePath), _name(name), _lifeTime(lifeTime), _isExpired(false)
    {
    }

    TemporaryFruit::~TemporaryFruit() {
        if (_items.count(_name))
            _items.erase(_name);
    }

    void TemporaryFruit::spawn(const std::vector<std::pair<int, int>>& occupiedTiles) {
        std::vector<std::pair<int, int>> freeTiles;
        for (int y = 0; y < MAP_SIZE; ++y) {
            for (int x = 0; x < MAP_SIZE; ++x) {
                std::pair<int, int> currentPos = {x, y};
                if (std::find(occupiedTiles.begin(), occupiedTiles.end(), currentPos) == occupiedTiles.end())
                    freeTiles.push_back(currentPos);
            }
        }

        if (!freeTiles.empty()) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, freeTiles.size() - 1);

            _position = freeTiles[dis(gen)];
            _isExpired = false;
            _spawnTime = std::chrono::steady_clock::now();

            _items[_name] = {{ OFFSET_X + _position.first * TILE_SIZE * SCALE, 
                OFFSET_Y + _position.second * TILE_SIZE * SCALE }, {SCALE, SCALE}, 0, {0, 0},
                false, 2, _texturePath, "*", nullptr};
        } else
            _isExpired = true;
    }

    void TemporaryFruit::update(const std::vector<std::pair<int, int>>& occupiedTiles) {
        (void)occupiedTiles;
        if (_isExpired) 
            return;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _spawnTime).count();

        if (elapsed >= _lifeTime) {
            if (_items.count(_name))
                _items.erase(_name);
            _isExpired = true;
        }
    }
}
