/*
** EPITECH PROJECT, 2026
** *
** File description:
** Clyde
*/

#pragma once
#include "AGhost.hpp"

namespace Arc {
    class Clyde: public AGhost{
        public:
            Clyde(std::shared_ptr<PacmanPlayer> pacman) {
                _pacman = pacman;
                _speed = 0.1f;
                reset();
            };
            ~Clyde() = default;

            void reset() override {
                _position.first = 15.5;
                _position.second = 14;
                _hitBox = 1;
                _scatterTarget = {2, 29};
                _direction = DIRECTION_UP;
                _nextDirection = DIRECTION_UP;                
                _spriteId = 98;
                _currentFrameId = 0;
                _spawnTiming = std::chrono::milliseconds(10000);
                _moveCycle = std::chrono::steady_clock::now();
               _spawned = false;
                _respawned = true;
                _bonus = false;
                _die = false;
            }

        protected:
        private:
    };
}
