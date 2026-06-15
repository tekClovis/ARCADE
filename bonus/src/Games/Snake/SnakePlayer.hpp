/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SnakePlayer
*/

#ifndef SNAKEPLAYER_HPP_
#define SNAKEPLAYER_HPP_

#include "Item.hpp"
#include "SnakeMap.hpp"
#include "Fruits/IFruit.hpp"
#include "Event.hpp"
#include <vector>
#include <functional>
#include <map>

namespace Arc {
    class SnakePlayer {
        public:
            enum Direction { UP, DOWN, LEFT, RIGHT };
            enum BodyPartType { HEAD, BODY, TAIL };

            struct bodyPart {
                std::pair<int, int> position;
                Direction direction;
                Direction lastDirection;
                BodyPartType type;
            };

            SnakePlayer(std::map<std::string, Item>& items);
            ~SnakePlayer();

            void handleKey(Event event);
            void update();
            int eatFruit(IFruit& fruit);
            bool isAlive() const;
            bool checkCollision(const std::pair<int, int>& position);
            void grow(int length);
            std::vector<std::pair<int, int>> getBodyPositions() const;

        private:
            void updateSnakeHead();
            void updateSnakeBody(size_t index);
            void updateSnakeTail();
            std::pair<int, int> getRealDistance(std::pair<int, int> from,
                std::pair<int, int> to) const;

            std::map<std::string, Item>& _items;
            std::vector<bodyPart> _body;
            bool _isAlive;
    };
}

#endif /* !SNAKEPLAYER_HPP_ */
