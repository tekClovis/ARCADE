/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** AMap
*/

#pragma once
#include <vector>
#include <cstdint>
#include <map>

namespace Arc {
    // Classic Pac-Man maze  —  28 cols × 31 rows  (868 cells)
// Access pattern (once you add your method):  _map[y * 28 + x]
//
// 0 = void   (empty path / outside playfield)
// 1 = wall   (positions 13–14 on row 12 are the ghost-house door, also encoded as 1)
// 2 = point  (regular pellet)
// 3 = bonus  (energizer / power pellet — 4 total, corners of the playfield)
    class AMap {
        public:
            virtual ~AMap() = default;
            void reset() { _map = _mapsTemplate[0]; };
            // start of level at 0
            void set(std::uint8_t level) {
                if (level >= _mapsTemplate.size())
                level = level % (_mapsTemplate.size() | 1);
                _map = _mapsTemplate[level]; 
            };
            std::uint8_t &operator()(std::size_t x, std::size_t y) {
                if (x >= _width || y >= _height) return _map[0]; // out of bounds, treat as void
                return _map[y * _width + x];
            }
            std::uint8_t getWidth() const {return _width;};
            std::uint8_t getHeight() const {return _height;};
        protected:
            std::uint8_t _width;
            std::uint8_t  _height;
            std::vector<std::uint8_t> _map;
            inline static const std::vector<std::vector<std::uint8_t>> _mapsTemplate;
    };
}
