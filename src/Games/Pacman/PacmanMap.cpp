/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** PacmanMap
*/

#include "PacmanMap.hpp"

namespace {

    std::string getMapCellTexture(std::size_t index)
    {
        return "./assets/textures/pacman/map/" + std::to_string(index) + ".png";
    }

    std::string getPointTexture(std::uint8_t cell)
    {
        switch (cell) {
            case Arc::PacmanMap::Point:
                return "./assets/textures/pacman/map/30.png";
            case Arc::PacmanMap::Bonus:
                return "./assets/textures/pacman/map/85.png";
            case Arc::PacmanMap::Void:
                return "./assets/textures/pacman/map/87.png";
            case Arc::PacmanMap::TpPoint:
                return "./assets/textures/pacman/map/87.png";
            default:
                return "";
        }
    }

    std::string getMapCellText(std::uint8_t cell)
    {
        switch (cell) {
            case Arc::PacmanMap::Wall:
                return "#";
            case Arc::PacmanMap::GhostHouseDoor:
                return "=";
            default:
                return " ";
        }
    }

    std::string getPointText(std::uint8_t cell)
    {
        switch (cell) {
            case Arc::PacmanMap::Point:
                return ".";
            case Arc::PacmanMap::Bonus:
                return "o";
            case Arc::PacmanMap::TpPoint:
                return "T";
            case Arc::PacmanMap::Void:
                return " ";
            default:
                return "";
        }
    }
}

void Arc::PacmanMap::draw(std::map<std::string, Arc::Item>& items)
{
    const float mapScale = CELL_SIZE / SPRITE_SIZE;

    for (std::uint8_t y = 0; y < _height; ++y) {
        for (std::uint8_t x = 0; x < _width; ++x) {
            const std::size_t idx = static_cast<std::size_t>(y) * _width + x;
            const std::uint8_t cell = (idx < _map.size()) ? _map[idx] : 0;
            const std::string mapKey = "Pacman_Map_" + std::to_string(x) + "_" + std::to_string(y);
            const std::string pointKey = "Pacman_Point_" + std::to_string(x) + "_" + std::to_string(y);

            const float posX = ORIGIN_X + (static_cast<float>(x) * CELL_SIZE);
            const float posY = ORIGIN_Y + (static_cast<float>(y) * CELL_SIZE);

            items[mapKey] = {
                {posX, posY},
                {mapScale, mapScale},
                0,
                {0, 0},
                false,
                0,
                getMapCellTexture(idx),
                getMapCellText(cell),
                nullptr
            };

            const std::string pointTexture = getPointTexture(cell);
            if (pointTexture.empty()) {
                items.erase(pointKey);
                continue;
            }

            items[pointKey] = {
                {posX, posY},
                {mapScale, mapScale},
                0,
                {0, 0},
                false,
                1,
                pointTexture,
                getPointText(cell),
                nullptr
            };
        }
    }
}

int Arc::PacmanMap::countPoints() const {
    int pointCount = 0;
    int bonusCount = 0;
    std::find_if(_map.begin(), _map.end(), [&](std::uint8_t cell) {
        if (cell == Point)
            pointCount++;
        if (cell == Bonus)
            bonusCount++;
        return false;
    });
    return pointCount + bonusCount;
}