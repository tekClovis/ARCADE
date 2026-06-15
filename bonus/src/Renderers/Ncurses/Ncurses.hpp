/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Ncurses
*/

#pragma once
#include "IRenderer.hpp"
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ncurses.h>

namespace Arc {
    class Ncurses : public IRenderer {
        public:
            Ncurses();
            ~Ncurses() override;

            void display(std::map<std::string, Arc::Item>& items) override;
            void createWindow(int width, int height, const std::string& title) override;
            bool pollEvent(Event& event) override;

        private:
            bool _initialized;
            int _logicalWidth;
            int _logicalHeight;

            int mapXToCol(float x) const;
            int mapYToRow(float y) const;
            int mapColToX(int col) const;
            int mapRowToY(int row) const;
            void drawTextCentered(const Arc::Item& item, const std::string& text);
    };
}