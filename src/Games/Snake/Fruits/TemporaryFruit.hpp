/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** TemporaryFruit
*/

#ifndef TEMPORARYFRUIT_HPP_
#define TEMPORARYFRUIT_HPP_

#include "IFruit.hpp"
#include <chrono>
#include <string>
#include <map>

namespace Arc {
    class TemporaryFruit : public IFruit {
        public:
            TemporaryFruit(std::map<std::string, Item>& items, std::string texturePath, std::string name, int lifeTime);
            ~TemporaryFruit();

            void spawn(const std::vector<std::pair<int, int>>& occupiedTiles) override;
            void update(const std::vector<std::pair<int, int>>& occupiedTiles) override;
            std::pair<int, int> getPosition() const override { return _position; }
            bool isExpired() const override { return _isExpired; }
            void setExpiry() {_isExpired = true; };
            std::string getName() const override { return _name; }
            FruitType getType() const override { return TEMPORARY; };

        private:
            std::map<std::string, Item>& _items;
            std::pair<int, int> _position;
            std::string _texturePath;
            std::string _name;
            std::chrono::steady_clock::time_point _spawnTime;
            int _lifeTime;
            bool _isExpired;
    };
}

#endif /* !TEMPORARYFRUIT_HPP_ */
