/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SFML
*/

#ifndef SFML_HPP_
#define SFML_HPP_
#include "IRenderer.hpp"
#include "IGame.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <map>
#include <memory>

namespace Arc {
    class SFML : public IRenderer {
        public:
            SFML();
            ~SFML();
            void display(std::map<std::string, Arc::Item>& items) override;
            bool pollEvent(Event& event) override;
            void createWindow(int width, int height, const std::string& title) override;
            std::vector<std::map<std::string, Arc::Item>::iterator> sortByOrder(std::map<std::string, Arc::Item>& items);
            void DrawSprite(Arc::Item& item);
            void DrawText(Arc::Item& item);
            void clear(std::map<std::string, Arc::Item>& items);

        protected:
        private:
            sf::RenderWindow* _window;
            sf::Font _font;
            std::map<std::string, std::shared_ptr<sf::Texture>> _textureCache;
    };
}

#endif /* !SFML_HPP_ */
