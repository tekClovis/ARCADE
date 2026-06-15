/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Tron
*/

#ifndef TRON_HPP_
#define TRON_HPP_

#define SPEED 320.0f

#include "IGame.hpp"
#include "Item.hpp"
#include"chrono"
#include <list>
#include <cmath>
#include <queue>

#define squarePerRow 32
#define Outline_startX 455.0f
#define Outline_startY 35.0f
#define cellSize 32.5f

namespace Arc {
    class Tron : public IGame {
        public:
            Tron(std::map<std::string, Item>& items);
            ~Tron();
            void update(Event event) override;
            void handleKey(Event event) override;
            std::vector<int> getScore() const override { return { _blueScore, _orangeScore }; };
            bool isGameOver() const override { return _backToMenu; };
            void movePlayer(const std::string& playerName, float moveX, float moveY);
            void checkCollision();
            void paintSquareUnderPlayer(const std::string& playerName, const std::string& texturePath);
            std::pair<float, float> getCell(const std::string& playerName) const;
            bool hitsTrail(const std::list<std::pair<float, float>>& trail, const std::pair<float, float>& cell, bool ignoreHead) const;
            void updateDirection(const std::string& playerName, Key key);
            void snapPlayerToGrid(const std::string& playerName);
            void restartGame();
            void createGame();

        protected:
        private:

        std::map<std::string, Item>& _items;

        int _blueScore;
        int _orangeScore;

        std::chrono::steady_clock::time_point _lastMoveTime;

        bool _gameOver;
        bool _setNewScore;
        bool activateGame;
        bool gameActivated;

        int _blueDirX;
        int _blueDirY;
        int _orangeDirX;
        int _orangeDirY;

        bool _BlueDead;
        bool _OrangeDead;

        std::list<std::pair<float, float>> _blueSquares;
        std::list<std::pair<float, float>> _orangeSquares;

        bool _backToMenu;

        std::map<std::string, int> _gameScores;

        std::map<std::string, std::pair<float, float>> _outlinePositions = {
            {"Restart", {660, 540}},
            {"Menu", {1260, 540}}
        };
        
        
    };
}


#endif /* !TRON_HPP_ */
