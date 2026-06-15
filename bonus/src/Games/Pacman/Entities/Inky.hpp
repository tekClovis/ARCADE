/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Blinky
*/

#pragma once
#include "AGhost.hpp"

namespace Arc {
    class Inky: public AGhost {
        public:
            Inky(std::shared_ptr<PacmanPlayer> pacman) {
                _pacman = pacman;
                _speed = 0.1f;
                reset();
            };
            ~Inky() = default;

            void reset() override {
                _position.first = 11.5;
                _position.second = 14;
                _hitBox = 1;
                _scatterTarget = {24, 29};
                _direction =  DIRECTION_UP;
                _nextDirection = DIRECTION_UP;                
                _spriteId = 84;
                _currentFrameId = 0;
                _spawnTiming = std::chrono::milliseconds(5000);
                _moveCycle = std::chrono::steady_clock::now();
               _spawned = false;
            _respawned = true;
            _bonus = false;
            _die = false;            }

        protected:
        private:
    };
}
