/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SDL2
*/

#include "SDL2.hpp"
#include <algorithm>

namespace Arc {

    SDL2::SDL2()
    {
        _window = nullptr;
        _renderer = nullptr;
        _font = nullptr;
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        if (TTF_Init() == -1)
            std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        if (IMG_Init(IMG_INIT_PNG) == 0)
            std::cerr << "IMG could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        _font = TTF_OpenFont("./assets/fonts/arcade_font.TTF", 24);
        if (!_font)
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    SDL2::~SDL2()
    {
        if (_font)
            TTF_CloseFont(_font);
        if (_renderer)
            SDL_DestroyRenderer(_renderer);
        if (_window)
            SDL_DestroyWindow(_window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void SDL2::createWindow(int width, int height, const std::string& title)
    {
        if (_window) {
            SDL_DestroyRenderer(_renderer);
            SDL_DestroyWindow(_window);
            _window = nullptr;
            _renderer = nullptr;
        }
        _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_FULLSCREEN);
        if (!_window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        if (!_renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    }

    void SDL2::display(std::map<std::string, Arc::Item>& items)
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
        SDL_RenderPresent(_renderer);
    }

    std::vector<std::map<std::string, Arc::Item>::iterator> SDL2::sortByOrder(std::map<std::string, Arc::Item>& items)
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

    void SDL2::DrawSprite(Arc::Item& item)
    {
        static std::map<std::string, SDL_Texture*> textureCache;

        if (textureCache.find(item.texturePath) == textureCache.end()) {
            SDL_Surface* loadedSurface = IMG_Load(item.texturePath.c_str());
            if (!loadedSurface) {
                std::cerr << "Failed to load texture: " << item.texturePath << " IMG_Error: " << IMG_GetError() << std::endl;
                return;
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);
            if (!texture) {
                std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
                return;
            }
            textureCache[item.texturePath] = texture;
        }

        SDL_Texture* texture = textureCache[item.texturePath];
        int textureWidth = 0;
        int textureHeight = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
        item.textureSize = {static_cast<float>(textureWidth), static_cast<float>(textureHeight)};

        SDL_Rect rect;
        rect.x = static_cast<int>(item.position.first - (textureWidth * item.size.first) / 2);
        rect.y = static_cast<int>(item.position.second - (textureHeight * item.size.second) / 2);
        rect.w = static_cast<int>(textureWidth * item.size.first);
        rect.h = static_cast<int>(textureHeight * item.size.second);

        SDL_Point center = {rect.w / 2, rect.h / 2};
        SDL_RenderCopyEx(_renderer, texture, nullptr, &rect, item.rotation, &center, SDL_FLIP_NONE);
    }

    void SDL2::DrawText(Arc::Item& item)
    {
        if (!_font)
            return;
        SDL_Color color = {255, 255, 255, 255};

        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(_font, item.text.c_str(), color);
        if (!textSurface) {
            std::cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
        if (!textTexture) {
            std::cerr << "Unable to create text texture! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(textSurface);
            return;
        }

        int textureWidht = textSurface->w;
        int textureHeight = textSurface->h;
        SDL_Rect rect;
        rect.x = static_cast<int>(item.position.first - textureWidht / 2);
        rect.y = static_cast<int>(item.position.second - textureHeight / 2);
        rect.w = textureWidht;
        rect.h = textureHeight;
        SDL_RenderCopy(_renderer, textTexture, nullptr, &rect);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }

    void SDL2::clear(std::map<std::string, Arc::Item>& items)
    {
        if (!_renderer)
            return;
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
        for (const auto& [name, item] : items) {
            if (name == "Background" || name.starts_with("joystick"))
                DrawSprite(const_cast<Arc::Item&>(item));
        }
        // voir pour comment gérer le gameView (pas built_in en sdl2)
    }

    bool SDL2::pollEvent(Event& event) {
        if (!_window || !_renderer)
            return false;

        SDL_Event sdlEvent;

        int mouseX = 0;
        int mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        event.mouse = {mouseX, mouseY};

        static const std::unordered_map<SDL_Keycode, Key> keyMap = {
            {SDLK_TAB, Key::Tab}, {SDLK_LEFT, Key::Left}, {SDLK_RIGHT, Key::Right},
            {SDLK_UP, Key::Up}, {SDLK_DOWN, Key::Down}, {SDLK_a, Key::A},
            {SDLK_b, Key::B}, {SDLK_c, Key::C}, {SDLK_d, Key::D},
            {SDLK_e, Key::E}, {SDLK_f, Key::F}, {SDLK_g, Key::G},
            {SDLK_h, Key::H}, {SDLK_i, Key::I}, {SDLK_j, Key::J},
            {SDLK_k, Key::K}, {SDLK_l, Key::L}, {SDLK_m, Key::M},
            {SDLK_n, Key::N}, {SDLK_o, Key::O}, {SDLK_p, Key::P},
            {SDLK_q, Key::Q}, {SDLK_r, Key::R}, {SDLK_s, Key::S},
            {SDLK_t, Key::T}, {SDLK_u, Key::U}, {SDLK_v, Key::V},
            {SDLK_w, Key::W}, {SDLK_x, Key::X}, {SDLK_y, Key::Y},
            {SDLK_z, Key::Z}, {SDLK_SPACE, Key::Space}, {SDLK_RETURN, Key::Enter},
            {SDLK_BACKSPACE, Key::Backspace}, {SDLK_MINUS, Key::Dash},
            {SDLK_0, Key::Num0}, {SDLK_1, Key::Num1}, {SDLK_2, Key::Num2},
            {SDLK_3, Key::Num3}, {SDLK_4, Key::Num4}, {SDLK_5, Key::Num5},
            {SDLK_6, Key::Num6}, {SDLK_7, Key::Num7}, {SDLK_8, Key::Num8},
            {SDLK_9, Key::Num9},
            {SDLK_KP_0, Key::Num0}, {SDLK_KP_1, Key::Num1}, {SDLK_KP_2, Key::Num2},
            {SDLK_KP_3, Key::Num3}, {SDLK_KP_4, Key::Num4}, {SDLK_KP_5, Key::Num5},
            {SDLK_KP_6, Key::Num6}, {SDLK_KP_7, Key::Num7}, {SDLK_KP_8, Key::Num8},
            {SDLK_KP_9, Key::Num9}
        };

        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    event.type = Type::Quit;
                    return true;

                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                        event.type = Type::Quit;
                        return true;
                    }

                    auto it = keyMap.find(sdlEvent.key.keysym.sym);
                    if (it != keyMap.end()) {
                        event.code = it->second;
                        event.type = (sdlEvent.type == SDL_KEYDOWN) ? Type::KeyPressed : Type::KeyReleased;
                        return true;
                    }
                    break;
                }

                case SDL_MOUSEBUTTONDOWN: {
                    event.type = Type::KeyPressed;
                    event.mouse = {sdlEvent.button.x, sdlEvent.button.y};
                    if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                        event.code = Key::MouseLeft;
                    else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
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
        return new Arc::SDL2();
    }

    int getLibraryType() {
        return static_cast<int>(Arc::LibraryType::Renderer);
    }

    std::string getName()
    {
        return "SDL2";
    }
}
