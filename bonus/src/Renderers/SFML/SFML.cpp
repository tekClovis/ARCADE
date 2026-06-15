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
            bool isVisible = item.size.first > 0.0f && item.size.second > 0.0f;
            if (isVisible && !item.texturePath.empty() && name != "Background" && !name.starts_with("joystick"))
                DrawSprite(item);
            if (isVisible && !item.text.empty() && item.texturePath.empty())
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
            if (name == "Background" || name.starts_with("joystick"))
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

        static const std::unordered_map<sf::Keyboard::Key, Key> keyMap = {
            {sf::Keyboard::Tab, Key::Tab}, {sf::Keyboard::Left, Key::Left},
            {sf::Keyboard::Right, Key::Right}, {sf::Keyboard::Up, Key::Up},
            {sf::Keyboard::Down, Key::Down}, {sf::Keyboard::A, Key::A},
            {sf::Keyboard::B, Key::B}, {sf::Keyboard::C, Key::C},
            {sf::Keyboard::D, Key::D}, {sf::Keyboard::E, Key::E},
            {sf::Keyboard::F, Key::F}, {sf::Keyboard::G, Key::G},
            {sf::Keyboard::H, Key::H}, {sf::Keyboard::I, Key::I},
            {sf::Keyboard::J, Key::J}, {sf::Keyboard::K, Key::K},
            {sf::Keyboard::L, Key::L}, {sf::Keyboard::M, Key::M},
            {sf::Keyboard::N, Key::N}, {sf::Keyboard::O, Key::O},
            {sf::Keyboard::P, Key::P}, {sf::Keyboard::Q, Key::Q},
            {sf::Keyboard::R, Key::R}, {sf::Keyboard::S, Key::S},
            {sf::Keyboard::T, Key::T}, {sf::Keyboard::U, Key::U},
            {sf::Keyboard::V, Key::V}, {sf::Keyboard::W, Key::W},
            {sf::Keyboard::X, Key::X}, {sf::Keyboard::Y, Key::Y},
            {sf::Keyboard::Z, Key::Z}, {sf::Keyboard::Space, Key::Space},
            {sf::Keyboard::Enter, Key::Enter}, {sf::Keyboard::Backspace, Key::Backspace},
            {sf::Keyboard::Num0, Key::Num0}, {sf::Keyboard::Num1, Key::Num1},
            {sf::Keyboard::Num2, Key::Num2}, {sf::Keyboard::Num3, Key::Num3},
            {sf::Keyboard::Num4, Key::Num4}, {sf::Keyboard::Num5, Key::Num5},
            {sf::Keyboard::Num6, Key::Num6}, {sf::Keyboard::Num7, Key::Num7},
            {sf::Keyboard::Num8, Key::Num8}, {sf::Keyboard::Num9, Key::Num9}
        };

        while (_window->pollEvent(sfEvent)) {
            switch (sfEvent.type) {
                case sf::Event::Closed:
                    event.type = Type::Quit;
                    return true;
                case sf::Event::KeyPressed:
                case sf::Event::KeyReleased: {
                    if (sfEvent.key.code == sf::Keyboard::Escape) {
                        event.type = Type::Quit;
                        return true;
                    }

                    auto it = keyMap.find(sfEvent.key.code);
                    if (it != keyMap.end()) {
                        event.code = it->second;
                        event.type = (sfEvent.type == sf::Event::KeyPressed) ? Type::KeyPressed : Type::KeyReleased;
                        return true;
                    }
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    auto worldPosMouse = _window->mapPixelToCoords({sfEvent.mouseButton.x, sfEvent.mouseButton.y});
                    event.type = Type::KeyPressed;
                    event.mouse = {static_cast<int>(worldPosMouse.x), static_cast<int>(worldPosMouse.y)};
                    if (sfEvent.mouseButton.button == sf::Mouse::Left)
                        event.code = Key::MouseLeft;
                    if (sfEvent.mouseButton.button == sf::Mouse::Right)
                        event.code = Key::MouseRight;
                    return true;
                }
                default:
                    break;
            }
        }
        return false;
    }

}

extern "C" {
    Arc::IRenderer* entryPoint(std::map<std::string, Arc::Item>& items) {
        if (items.empty())
            return nullptr;
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

