/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** AEntity
*/

#pragma once
#include "IEntity.hpp"
#include "AMap.hpp"
#include <cmath>
#define POSITION std::pair<float, float>
#define DIRECTION std::pair<int, int>
#define DIRECTION_LEFT std::pair(-1, 0)
#define DIRECTION_RIGHT std::pair(1, 0)
#define DIRECTION_UP std::pair(0, -1)
#define DIRECTION_DOWN std::pair(0, 1)

namespace Arc {
    class AEntity : public Arc::IEntity{

        protected:
            POSITION _position;
            std::size_t _spriteId = 0;
            DIRECTION _direction = {0, 0};
            DIRECTION _nextDirection = {0, 0};
            std::size_t _hitBox;
            float _speed;
            bool _bonus = false;
            bool _die = false;
        public:
            virtual ~AEntity() = default;
            POSITION getPosition() const override {return _position;};
            void setDirection(DIRECTION direction) override;
            void setNextDirection(DIRECTION direction) {_nextDirection = direction;};
            bool operator==(const Arc::IEntity& other) const override {
                return (std::abs(_position.first - other.getPosition().first) < 0.8f
                    && std::abs(_position.second - other.getPosition().second) < 0.8f);
            };
            int gotoPoint(AMap& map, std::pair<float, float> target) override;
            void animate() override {};
            bool &getBonus() override {return _bonus;};
            bool &getDie() override {return _die;};
            float &getSpeed() override {return _speed;};
            void move(Arc::AMap& map) override;
            virtual void update(AMap& map) override {(void)map;};
            void reset() override {};
    };
}