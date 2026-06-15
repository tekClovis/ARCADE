/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SnakeMap
*/

#ifndef SNAKEMAP_HPP_
#define SNAKEMAP_HPP_

#include "Item.hpp"
#include <vector>
#include <map>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAP_SIZE 15
#define TILE_SIZE 32
#define SCALE 2.0f
#define MAP_WIDTH (MAP_SIZE * TILE_SIZE * SCALE)
#define MAP_HEIGHT (MAP_SIZE * TILE_SIZE * SCALE)
#define OFFSET_X ((WINDOW_WIDTH - MAP_WIDTH) / 2) + 50
#define OFFSET_Y ((WINDOW_HEIGHT - MAP_HEIGHT) / 2) + 10

namespace Arc {
    class SnakeMap {
        public:
            SnakeMap(std::map<std::string, Item>& items);
            ~SnakeMap();
            int getMapSize() {return MAP_SIZE * MAP_SIZE; };

        protected:
        private:
            void createMap(std::map<std::string, Item>& items);
            std::vector<std::vector<int>> _map;
    };
}

#endif /* !SNAKEMAP_HPP_ */
