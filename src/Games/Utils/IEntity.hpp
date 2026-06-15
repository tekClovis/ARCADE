/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Entity
*/

#pragma once

#include <utility>
#include <cstddef>
#include <cmath>
#include "AMap.hpp"

namespace Arc
{
    class IEntity {
        public:
            virtual ~IEntity() = default;
            virtual std::pair<float, float> getPosition() const = 0;
            virtual void setDirection(std::pair<int, int> direction) = 0;
            virtual bool operator==(const Arc::IEntity& other)const = 0;
            virtual void move(AMap& map) = 0;
            virtual void animate() = 0;
            virtual bool &getBonus() = 0;
            virtual bool &getDie() = 0;
            virtual float &getSpeed() = 0;
            virtual int gotoPoint(AMap& map, std::pair<float, float> target) = 0;
            virtual void update(AMap& map) = 0;
            virtual void reset() = 0;
        };    
}
