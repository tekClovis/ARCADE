/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Snake
*/

#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#define BASE_SPEED 150
#include "IGame.hpp"
#include "Item.hpp"
#include "SnakeMap.hpp"
#include "SnakePlayer.hpp"
#include "Fruits/IFruit.hpp"
#include "Fruits/BasicFruit.hpp"
#include "Fruits/TemporaryFruit.hpp"
#include <memory>
#include <chrono>
#include <random>

namespace Arc {
    class Snake : public IGame {
        public:
            enum State { START, PLAYING, ENDGAME, QUIT };

            Snake(std::map<std::string, Item>& items);
            ~Snake();
            void update(Event event) override;
            void handleKey(Event event) override;
            std::vector<int> getScore() const override { return {_score}; };
            bool isGameOver() const override { return _gameState == QUIT; };

        protected:
        private:
            std::map<std::string, Item>& _items;
            std::unique_ptr<SnakeMap> _map;
            std::unique_ptr<SnakePlayer> _player;
            std::vector<std::unique_ptr<IFruit>> _fruits;
            std::chrono::steady_clock::time_point _lastUpdate;
            int _score;
            State _gameState;
            int _selected_item;
            int _delay;
            std::chrono::steady_clock::time_point _boostTimer;
            bool _isBoosted;
            bool _boostMax;

            void updateScore(int points);
            void restart();
            void spawnRandomTemporaryFruit();
    };
}

#endif /* !SNAKE_HPP_ */
