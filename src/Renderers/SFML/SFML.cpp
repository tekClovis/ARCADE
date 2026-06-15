/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SFML
*/

#include "SFML.hpp"
#include <algorithm>
#include <type_traits>
#include <vector>

namespace Arc {

    SFML::SFML()
    {
        _window = nullptr;
        if (!_font.loadFromFile("./assets/fonts/arcade_font.TTF"))
            std::cerr << "Failed to load font" << std::endl;
    }

    SFML::~SFML()
    {
        _textureCache.clear();
        if (_window && _window->isOpen())
            _window->close();
        delete _window;
    }

    void SFML::createWindow(int width, int height, const std::string& title)
    {
        if (_window) {
            if (_window->isOpen())
                _window->close();
            delete _window;
            _window = nullptr;
        }
        _window = new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Fullscreen);
        _window->setFramerateLimit(60);
    }

    void SFML::display(std::map<std::string, Arc::Item>& items)
    {
        clear(items);

        std::vector<std::map<std::string, Arc::Item>::iterator> sortedItems = sortByOrder(items);

        for (auto& iterator : sortedItems) {
            auto& [name, item] = *iterator;
            bool spriteVisible = item.size.first > 0.0f && item.size.second > 0.0f;
            bool textVisible = item.size.first > 0.0f || item.size.second > 0.0f;

            if (spriteVisible && !item.texturePath.empty() && name != "Background")
                DrawSprite(item);
            if (textVisible && !item.text.empty() && item.texturePath.empty())
                DrawText(item);
        }
        _window->display();
    }

    std::vector<std::map<std::string, Arc::Item>::iterator> SFML::sortByOrder(std::map<std::string, Arc::Item>& items)
    {
        std::vector<std::map<std::string, Arc::Item>::iterator> sortedItems;

        sortedItems.reserve(items.size());
        for (auto iterator = items.begin(); iterator != items.end(); ++iterator)
            sortedItems.push_back(iterator);

        std::sort(sortedItems.begin(), sortedItems.end(), [](const auto& first, const auto& second) {
            return first->second.drawOrder < second->second.drawOrder;
        });
        return sortedItems;
    }

    void SFML::DrawSprite(Arc::Item& item)
    {
        if (_textureCache.find(item.texturePath) == _textureCache.end()) {
            auto texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(item.texturePath)) {
                std::cerr << "Failed to load texture: " << item.texturePath << std::endl;
                return;
            }
            _textureCache[item.texturePath] = texture;
        }

        sf::Sprite sprite(*_textureCache[item.texturePath]);
        sprite.setPosition(item.position.first, item.position.second);
        sprite.setScale(item.size.first, item.size.second);
        item.textureSize = {static_cast<float>(_textureCache[item.texturePath]->getSize().x), 
                           static_cast<float>(_textureCache[item.texturePath]->getSize().y)};
        sprite.setOrigin(_textureCache[item.texturePath]->getSize().x / 2.f, 
                        _textureCache[item.texturePath]->getSize().y / 2.f);
        sprite.setRotation(static_cast<float>(item.rotation));
        _window->draw(sprite);
    }

    void SFML::DrawText(Arc::Item& item)
    {
        sf::Text text;

        text.setFont(_font);
        text.setString(item.text);
        text.setCharacterSize(static_cast<unsigned int>(item.size.first));
        text.setPosition(item.position.first, item.position.second);
        text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
        text.setFillColor(sf::Color::White);
        _window->draw(text);
    }

    void SFML::clear(std::map<std::string, Arc::Item>& items)
    {
        if (!_window || !_window->isOpen())
            return;
        _window->setView(_window->getDefaultView());
        _window->clear(sf::Color::Black);
        for (const auto& [name, item] : items) {
            if (name == "Background")
                DrawSprite(const_cast<Arc::Item&>(item));
        }
        sf::View gameView = _window->getDefaultView();
        gameView.zoom(3.0f);
        _window->setView(gameView);
    }

    bool SFML::pollEvent(Event& event) {
        if (!_window || !_window->isOpen())
            return false;

        sf::Vector2i pixelPos = sf::Mouse::getPosition(*_window);
        sf::Vector2f worldPos = _window->mapPixelToCoords(pixelPos);

        event.mouse = {static_cast<int>(worldPos.x), static_cast<int>(worldPos.y)};

        sf::Event sfEvent;

        static const std::unordered_map<sf::Keyboard::Key, std::function<bool(Event&)>> keyHandlers = {
            {sf::Keyboard::Tab, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Tab; return true; }},
            {sf::Keyboard::Left, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Left; return true; }},
            {sf::Keyboard::Right, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Right; return true; }},
            {sf::Keyboard::Up, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Up; return true; }},
            {sf::Keyboard::Down, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Down; return true; }},
            {sf::Keyboard::A, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::A; return true; }},
            {sf::Keyboard::B, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::B; return true; }},
            {sf::Keyboard::C, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::C; return true; }},
            {sf::Keyboard::D, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::D; return true; }},
            {sf::Keyboard::E, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::E; return true; }},
            {sf::Keyboard::F, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::F; return true; }},
            {sf::Keyboard::G, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::G; return true; }},
            {sf::Keyboard::H, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::H; return true; }},
            {sf::Keyboard::I, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::I; return true; }},
            {sf::Keyboard::J, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::J; return true; }},
            {sf::Keyboard::K, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::K; return true; }},
            {sf::Keyboard::L, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::L; return true; }},
            {sf::Keyboard::M, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::M; return true; }},
            {sf::Keyboard::N, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::N; return true; }},
            {sf::Keyboard::O, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::O; return true; }},
            {sf::Keyboard::P, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::P; return true; }},
            {sf::Keyboard::Q, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Q; return true; }},
            {sf::Keyboard::R, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::R; return true; }},
            {sf::Keyboard::S, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::S; return true; }},
            {sf::Keyboard::T, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::T; return true; }},
            {sf::Keyboard::U, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::U; return true; }},
            {sf::Keyboard::V, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::V; return true; }},
            {sf::Keyboard::W, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::W; return true; }},
            {sf::Keyboard::X, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::X; return true; }},
            {sf::Keyboard::Y, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Y; return true; }},
            {sf::Keyboard::Z, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Z; return true; }},
            {sf::Keyboard::Num1, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num1; return true; }},
            {sf::Keyboard::Num2, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num2; return true; }},
            {sf::Keyboard::Num3, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num3; return true; }},
            {sf::Keyboard::Num4, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num4; return true; }},
            {sf::Keyboard::Num5, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num5; return true; }},
            {sf::Keyboard::Num6, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num6; return true; }},
            {sf::Keyboard::Num7, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num7; return true; }},
            {sf::Keyboard::Num8, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num8; return true; }},
            {sf::Keyboard::Num9, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num9; return true; }},
            {sf::Keyboard::Num0, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num0; return true; }},
            {sf::Keyboard::Numpad0, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num0; return true; }},
            {sf::Keyboard::Numpad1, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num1; return true; }},
            {sf::Keyboard::Numpad2, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num2; return true; }},
            {sf::Keyboard::Numpad3, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num3; return true; }},
            {sf::Keyboard::Numpad4, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num4; return true; }},
            {sf::Keyboard::Numpad5, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num5; return true; }},
            {sf::Keyboard::Numpad6, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num6; return true; }},
            {sf::Keyboard::Numpad7, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num7; return true; }},
            {sf::Keyboard::Numpad8, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num8; return true; }},
            {sf::Keyboard::Numpad9, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num9; return true; }},

            {sf::Keyboard::Space, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Space; return true; }},
            {sf::Keyboard::Enter, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Enter; return true; }},
            {sf::Keyboard::Backspace, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Backspace; return true; }}
        };

        const std::unordered_map<sf::Mouse::Button, std::function<bool(Event&, const sf::Event&)>> mouseHandlers = {
            {sf::Mouse::Left, [this](Event& e, const sf::Event& ev){
                auto worldPos = _window->mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
                e.type = Type::KeyPressed;
                e.code = Key::MouseLeft;
                e.mouse = {static_cast<int>(worldPos.x), static_cast<int>(worldPos.y)};
                return true;
            }},
            {sf::Mouse::Right, [this](Event& e, const sf::Event& ev){
                auto worldPos = _window->mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
                e.type = Type::KeyPressed;
                e.code = Key::MouseRight;
                e.mouse = {static_cast<int>(worldPos.x), static_cast<int>(worldPos.y)};
                return true;
            }}
        };

        while (_window->pollEvent(sfEvent)) {
            auto eventKey = keyHandlers.end();
            switch (sfEvent.type) {
                case sf::Event::Closed:
                    event.type = Type::Quit;
                    return true;
                case sf::Event::KeyPressed:
                    if (sfEvent.key.code == sf::Keyboard::Escape) {
                        event.type = Type::Quit;
                        return true;
                    }
                    eventKey = keyHandlers.find(sfEvent.key.code);
                    if (eventKey != keyHandlers.end())
                        return eventKey->second(event);
                    break;
                case sf::Event::MouseButtonPressed: {
                    auto mouseIt = mouseHandlers.find(sfEvent.mouseButton.button);
                    if (mouseIt != mouseHandlers.end())
                        return mouseIt->second(event, sfEvent);
                    break;
                }
                default:
                    break;
            }
        }
        return false;
    }

}

extern "C" {
    Arc::IRenderer* entryPoint([[maybe_unused]] std::map<std::string, Arc::Item>& items) {
        return new Arc::SFML();
    }

    int getLibraryType() {
        return static_cast<int>(Arc::LibraryType::Renderer);
    }

    std::string getName()
    {
        return "SFML";
    }
}

