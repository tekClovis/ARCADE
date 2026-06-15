/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** IEntity
*/

#include "IEntity.hpp"

bool Arc::IEntity::operator==(const Arc::IEntity& other) const {
    return (std::round(static_cast<double>(getPosition().first)) == std::round(static_cast<double>(other.getPosition().first))
        && std::round(static_cast<double>(getPosition().second)) == std::round(static_cast<double>(other.getPosition().second)));
};