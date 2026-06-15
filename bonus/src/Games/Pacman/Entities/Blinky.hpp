/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Blinky
*/

#pragma once
#include "AGhost.hpp"

namespace Arc {
    class Blinky: public AGhost {
        public:
            Blinky(std::shared_ptr<PacmanPlayer> pacman) {
                _pacman = pacman;
                _speed = 0.1f;
               reset();
            };
            ~Blinky() = default;

            void reset() override {
                _position.first = 13.5;
                _position.second = 11;
                _hitBox = 1;
                _scatterTarget = {24, 1};
                _direction =  {0, 0};
                _nextDirection = {0, 0};
                _spriteId = 56;
                _currentFrameId = 0;
                _spawned = false;
                _respawned = true;
                _bonus = false;
                _die = false;
                _spawnTiming = std::chrono::milliseconds(0);
                _moveCycle = std::chrono::steady_clock::now();
            }

        private:
    };
}
