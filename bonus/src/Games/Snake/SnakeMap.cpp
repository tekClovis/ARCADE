/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SnakeMap
*/

#include "SnakeMap.hpp"

namespace Arc {
    SnakeMap::SnakeMap(std::map<std::string, Item>& items) {
        createMap(items);
    }

    SnakeMap::~SnakeMap()
    {
    }

    void SnakeMap::createMap(std::map<std::string, Item>& items) {
        for (int i = -1; i < MAP_SIZE + 1; i++) {
            for (int j = -1; j < MAP_SIZE + 1; j++) {
                std::string tileName = "tile_" + std::to_string(i) + "_" + std::to_string(j);
                std::string texturePath = "./assets/textures/Snake/background/grass_tile.png";
                std::string tileChar = "";
                if (i == -1 || i == MAP_SIZE || j == -1 || j == MAP_SIZE){
                    tileChar = "#";
                    texturePath = "";
                }
                
                items[tileName] = {{OFFSET_X + static_cast<float>(j * TILE_SIZE * SCALE), OFFSET_Y + static_cast<float>(i * TILE_SIZE * SCALE)},
                    {SCALE, SCALE}, 0, {0, 0}, false, 0, texturePath, tileChar, nullptr};
            }
        }
    }
}
