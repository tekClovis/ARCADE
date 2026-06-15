/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Blinky
*/

#pragma once
#include "AGhost.hpp"

namespace Arc {
    class Pinky: public AGhost {
        public:
            Pinky(std::shared_ptr<PacmanPlayer> pacman) {
                _pacman = pacman;
                _speed = 0.1f;
                reset();
            };
            ~Pinky() = default;

            void reset() override {
                _position.first = 13.5;
                _position.second = 14;
                _hitBox = 1;
                _scatterTarget = {2, 1};
                _direction = DIRECTION_DOWN;
                _nextDirection = {0, 0};                
                _spriteId = 70;
                _currentFrameId = 0;
                _spawnTiming = std::chrono::milliseconds(200);
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

