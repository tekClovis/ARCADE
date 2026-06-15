/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** IFruit
*/

#ifndef IFRUIT_HPP_
#define IFRUIT_HPP_

#include "Item.hpp"
#include <vector>
#include <utility>

namespace Arc {
    class IFruit {
        public:
            enum FruitType {
                BASIC,
                TEMPORARY
            };

            struct FruitData {
                std::string name;
                std::string path;
                int time;
            };

            virtual ~IFruit() = default;
            virtual void spawn(const std::vector<std::pair<int, int>>& occupiedTiles) = 0;
            virtual void update(const std::vector<std::pair<int, int>>& occupiedTiles) = 0;
            virtual std::pair<int, int> getPosition() const = 0;
            virtual bool isExpired() const = 0;
            virtual void setExpiry() = 0;
            virtual std::string getName() const = 0;
            virtual FruitType getType() const = 0;
    };
}

#endif /* !IFRUIT_HPP_ */
