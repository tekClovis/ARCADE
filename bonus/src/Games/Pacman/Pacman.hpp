/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** NcursesRenderer.hpp
*/

#pragma once
#include "IGame.hpp"
#include "Item.hpp"
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <map>
#include "AEntity.hpp"
#include "PacmanMap.hpp"
#include "Entities/PacmanPlayer.hpp"
#include "Entities/Blinky.hpp"
#include "Entities/Pinky.hpp"
#include "Entities/Inky.hpp"
#include "Entities/Clyde.hpp"

namespace Arc {
    class Pacman : public IGame {
        private:
            int _score = 0;
            int _level = 1;
            bool _gameOver = false;
            bool _backToMenu = false;
            bool _starting = true;
            PacmanMap _map;
            std::shared_ptr<PacmanPlayer> _player;
            Item _displayPoints;
            std::size_t _phantomEat = 0;
            std::chrono::steady_clock::time_point _pointClock;
            std::chrono::steady_clock::time_point _bonusTime;
            std::chrono::steady_clock::time_point _dieClock;
            std::chrono::steady_clock::time_point _startClock;
            std::chrono::steady_clock::time_point _endLevelClock;
            std::vector<std::unique_ptr<AGhost>> _ghosts;
            std::map<std::string, Item>& _items;
            std::map<std::string, std::pair<float, float>> _outlinePositions = {
                {"Restart", {275, 500}},
                {"Menu", {275, 600}}
            };
            std::string _outlinePos = "Restart";

        public:
            Pacman(std::map<std::string, Arc::Item>& items);
            ~Pacman() override = default;

            void update(Event event) override;
            void handleKey(Event event) override;
            bool isGameOver() const override { return _backToMenu; };
            virtual std::vector<int> getScore() const override { return { _score }; };

            bool handleStart();
            void rebuildItems();
            void restartGame();
        private:
    };

}
