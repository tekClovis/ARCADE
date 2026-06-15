/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** IRenderer
*/

#ifndef IRENDERER_HPP_
#define IRENDERER_HPP_

#include "Item.hpp"
#include "IGame.hpp"
#include "Event.hpp"
#include <iostream>
#include <string>


namespace Arc {
    class IRenderer {
        public:
            virtual ~IRenderer() = default;
            virtual void display(std::map<std::string, Arc::Item>& items) = 0;
            virtual void createWindow(int width, int height, const std::string& title) = 0;
            virtual bool pollEvent(Event& event) = 0;

        protected:
        private:
    };
}

#endif /* !IRENDERER_HPP_ */