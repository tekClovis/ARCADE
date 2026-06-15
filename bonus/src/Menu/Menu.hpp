/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_
#include "IGame.hpp"
#include "IRenderer.hpp"
#include "Item.hpp"
#include <list>
#include "Core.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Arc {
    class Menu : public IGame {
    public:
        Menu(std::map<std::string, Item>& items, Core* core);
        ~Menu();
        void update(Event event) override;
        void handleKey(Event event) override;

        std::vector<int> getScore() const override { return { 0 }; };
        bool isGameOver() const override { return false; };

        void checkButtonHover(int x, int y, bool click);
        void switchRenderer(int direction);
        void switchGame(int direction);
        void quit() { _core->_isRunning = false; };
        void placeGameLogos();
        void updateArrowPulse();
        void startArrowPulse(const std::string& buttonName);
        void moveSelector(int orientation);
        void slideGameLogos();
        void slideUniqueLogo(const std::string& logo, int speed, float targetX, float targetY, float targetSizeX, float targetSizeY);
        std::string cropCopyName(std::string& name);
        void createMenuItems();
        int getGlobalBestScore(const std::string& game);
        int getPersonalBestScore(const std::string& game);
        std::string getGlobalBestUser(const std::string& game);

    protected:
    private:

        std::map<std::string, Item>& _items;
        Core* _core;
        std::string _chosenRenderer;
        std::string _chosenGame;
        std::map<std::string, std::string> _gameLogos;

        std::string _pulsedButton;
        std::chrono::steady_clock::time_point _arrowPulseEnd;
        std::pair<float, float> _originalArrowSize;

        int _selectorRow;
        bool _slideGameLogos;
        int _slideDirection;
        bool _userNameEntered;
    };
}

#endif /* !MENU_HPP_ */
