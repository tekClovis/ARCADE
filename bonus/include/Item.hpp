/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Item
*/

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <string>
#include <map>
#include <functional>
#define ZOOM_FACTOR 3.0f

namespace Arc {

        struct Item {
            std::pair<float, float> position;
            std::pair<float, float> size;
            int rotation;
            std::pair<float, float> textureSize;
            bool isHovered;
            int drawOrder;
            std::string texturePath;
            std::string text;
            std::function<void()> buttonAction;
    };
}

#endif /* !ITEM_HPP_ */
