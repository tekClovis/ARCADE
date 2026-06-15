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
    class AMap {
        public:
            virtual ~AMap() = default;
            void reset() { _map = _mapsTemplate[0]; };
            void set(std::uint8_t level) {
                if (level >= _mapsTemplate.size())
                level = level % (_mapsTemplate.size() | 1);
                _map = _mapsTemplate[level]; 
            };
            std::uint8_t &operator()(std::size_t x, std::size_t y) {
                if (x >= _width || y >= _height) return _map[0];
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
