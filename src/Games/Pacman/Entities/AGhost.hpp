/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** AGhost
*/

#pragma once
#include "AEntity.hpp"
#include "PacmanPlayer.hpp"
#include <chrono>
#include <memory>
#include <string>

namespace Arc {
    class AGhost : public AEntity {
        public:
            virtual ~AGhost() = default;
            void animate() override;
            std::string getTexturePath();
            void update(AMap& map) override;
            void randomChoose(AMap& map);
            std::chrono::steady_clock::time_point &setBonusAnimTime() { return _bonusAnimTime; };
        protected:
            void move(AMap& map) override;
            std::shared_ptr<PacmanPlayer> _pacman = nullptr;
            bool _animation = false;
            bool _spawned = false;
            int _respawned = true;
            POSITION _scatterTarget = {0, 0};
            std::chrono::steady_clock::time_point _lastAnimTime = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point _lastCompute = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point _moveCycle = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point _bonusAnimTime = std::chrono::steady_clock::now();
            std::chrono::milliseconds _spawnTiming = std::chrono::milliseconds(0);
            bool _bonusAnimState = false;
            int _currentFrameId = 0;
        private:
    };
}
