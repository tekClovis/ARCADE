/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** BasicFruit
*/

#ifndef BASICFRUIT_HPP_
#define BASICFRUIT_HPP_

#include "IFruit.hpp"

namespace Arc {
    class BasicFruit : public IFruit {
    public:
        BasicFruit(std::map<std::string, Item>& items, std::string texture, std::string name);
        ~BasicFruit();

        void spawn(const std::vector<std::pair<int, int>>& occupiedTiles) override;
        void update(const std::vector<std::pair<int, int>>& occupiedTiles) override;
        std::pair<int, int> getPosition() const override { return _position; }
        bool isExpired() const override { return false; }
        void setExpiry() override {}
        std::string getName() const override { return _name; }
        FruitType getType() const override { return BASIC; };

    private:
        std::map<std::string, Item>& _items;
        std::string _texturePath;
        std::string _name;
        std::pair<int, int> _position;
};
}

#endif /* !BASICFRUIT_HPP_ */
