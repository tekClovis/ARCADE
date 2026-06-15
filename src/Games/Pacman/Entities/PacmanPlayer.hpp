/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** PacmanPlayer
*/

#pragma once

#include "AEntity.hpp"
#include "PacmanMap.hpp"

#include <chrono>
#include <cmath>
#include <cstddef>
#include <string>

namespace Arc {
    class PacmanPlayer: public AEntity {
        private:
            bool _animation = false;
            std::chrono::steady_clock::time_point _lastAnimTime{std::chrono::steady_clock::now()};
            std::size_t _life;
            std::size_t _deathFrame;
            bool _endLevel; 
            std::size_t _endAnim;
        public:
            PacmanPlayer() {
                _life = 3;
                _speed = 0.1f;
                reset();
            };
            ~PacmanPlayer() = default;
            
            void move(AMap& map) override;
            void update(AMap& map) override;
            void animate() override;
            std::string getTexturePath();
            std::size_t &getLife() {return _life;};
            bool &getEndLevel() {return _endLevel;};
            void reset() override {
                _position.first = 13.5;
                _position.second = 23;
                _hitBox = 1;
                _spriteId = 3;
                _endLevel = false;
                _endAnim = 0;
                _direction = DIRECTION_LEFT;
                _nextDirection = DIRECTION_LEFT;
                _deathFrame = 4;
            };
    };
}
