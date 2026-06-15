/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SDL2
*/

#ifndef SDL2_HPP_
#define SDL2_HPP_
#include "IRenderer.hpp"
#include "IGame.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

namespace Arc {
    class SDL2 : public IRenderer {
        public:
            SDL2();
            ~SDL2();
            void display(std::map<std::string, Arc::Item>& items) override;
            bool pollEvent(Event& event) override;
            void createWindow(int width, int height, const std::string& title) override;
            std::vector<std::map<std::string, Arc::Item>::iterator> sortByOrder(std::map<std::string, Arc::Item>& items);
            void DrawSprite(Arc::Item& item);
            void DrawText(Arc::Item& item);
            void clear(std::map<std::string, Arc::Item>& items);

        protected:
        private:
            SDL_Window* _window;
            SDL_Renderer* _renderer;
            TTF_Font* _font;
    };
}

#endif /* !SDL2_HPP_ */
