/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SDL2
*/

#include "SDL2.hpp"
#include <algorithm>

namespace Arc {

    SDL2::SDL2() :_gameTarget(nullptr), _targetWidth(0), _targetHeight(0), _zoomFactor(1.0f)
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
        if (_gameTarget)
            SDL_DestroyTexture(_gameTarget);
        if (_font)
            TTF_CloseFont(_font);
        if (_renderer)
            SDL_DestroyRenderer(_renderer);
        if (_window)
            SDL_DestroyWindow(_window);

        for (auto& pair : _fontCache)
            TTF_CloseFont(pair.second);
        _fontCache.clear();
        for (auto& pair : _textureCache)
            if (pair.second) SDL_DestroyTexture(pair.second);
        _textureCache.clear();

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
        _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (!_window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }
        _renderer = SDL_CreateRenderer(_renderer ? nullptr : _window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        if (!_renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }
        int realWidth;
        int realHeight;
        SDL_GetWindowSize(_window, &realWidth, &realHeight);

        _targetWidth = width;
        _targetHeight = height;
        SDL_RenderSetLogicalSize(_renderer, _targetWidth, _targetHeight);

        _zoomFactor = 1.0f / 3.0f;
        _gameTarget = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, _targetWidth, _targetHeight);
        if (!_gameTarget)
            std::cerr << "Failed to create render target: " << SDL_GetError() << std::endl;
        SDL_SetTextureBlendMode(_gameTarget, SDL_BLENDMODE_BLEND);
    }

    void SDL2::display(std::map<std::string, Arc::Item>& items)
    {
        if (!_renderer || !_gameTarget)
            return;
        auto sortedItems = sortByOrder(items);

        clear();

        for (auto& it : sortedItems) {
            if (it->first == "Background") {
                DrawSprite(it->second);
                break;
            }
        }

        SDL_SetRenderTarget(_renderer, _gameTarget);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0); 
        SDL_RenderClear(_renderer);

        for (auto& it : sortedItems) {
            auto& [name, item] = *it;
            if (name == "Background")
                continue;

            bool spriteVisible = !item.texturePath.empty() && item.size.first > 0.0f && item.size.second > 0.0f;
            bool textVisible = !item.text.empty() && (item.size.first > 0.0f || item.size.second > 0.0f);

            if (spriteVisible)
                DrawSprite(item);
            else if (textVisible)
                DrawText(item);
        }

        SDL_SetRenderTarget(_renderer, nullptr);

        SDL_Rect destRect;
        destRect.w = static_cast<int>(_targetWidth * _zoomFactor);
        destRect.h = static_cast<int>(_targetHeight * _zoomFactor);
        destRect.x = (_targetWidth - destRect.w) / 2;
        destRect.y = (_targetHeight - destRect.h) / 2;

        SDL_RenderCopy(_renderer, _gameTarget, nullptr, &destRect);
        SDL_RenderPresent(_renderer);
    }

    std::vector<std::map<std::string, Arc::Item>::iterator> SDL2::sortByOrder(std::map<std::string, Arc::Item>& items)
    {
        std::vector<std::map<std::string, Arc::Item>::iterator> sortedItems;
        sortedItems.reserve(items.size());
        for (auto it = items.begin(); it != items.end(); ++it)
            sortedItems.push_back(it);
        std::sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b) {
            return a->second.drawOrder < b->second.drawOrder;
        });
        return sortedItems;
    }

    void SDL2::DrawSprite(Arc::Item& item)
    {

        if (_textureCache.find(item.texturePath) == _textureCache.end()) {
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
            _textureCache[item.texturePath] = texture;
        }

        SDL_Texture* texture = _textureCache[item.texturePath];
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
        if (!_font || item.text.empty())
            return;

        int fontSize = static_cast<int>(item.size.first);
        if (fontSize <= 5)
            fontSize = 24;

        TTF_Font* useFont = _font;
        if (_fontCache.count(fontSize))
            useFont = _fontCache[fontSize];
        else {
            TTF_Font* newFont = TTF_OpenFont("./assets/fonts/arcade_font.TTF", fontSize);
            if (newFont)
                _fontCache[fontSize] = newFont, useFont = newFont;
        }

        std::vector<std::string> lines;
        std::stringstream ss(item.text);
        std::string line;

        while (std::getline(ss, line, '\n'))
            lines.push_back(line);

        int lineSpacing = TTF_FontLineSkip(useFont);
        int totalHeight = lines.size() * lineSpacing;
        float currentY = item.position.second - (totalHeight / 2.0f);

        for (const auto& lineText : lines) {
            if (lineText.empty()) {
                currentY += lineSpacing;
                continue;
            }

            SDL_Color color = {255, 255, 255, 255};
            SDL_Surface* textSurface = TTF_RenderUTF8_Blended(useFont, lineText.c_str(), color);
            if (!textSurface)
                continue;

            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
            if (textTexture) {
                SDL_Rect rect;
                rect.w = textSurface->w;
                rect.h = textSurface->h;
                rect.x = static_cast<int>(item.position.first - rect.w / 2);
                rect.y = static_cast<int>(currentY);

                SDL_RenderCopy(_renderer, textTexture, nullptr, &rect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
            currentY += lineSpacing;
        }
    }

    void SDL2::clear()
    {
        if (!_renderer)
            return;
        SDL_SetRenderTarget(_renderer, nullptr);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
    }

    bool SDL2::pollEvent(Event& event) {
        if (!_window || !_renderer)
            return false;

        SDL_Event sdlEvent;
        int mouseX = 0;
        int mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        event.mouse = {mouseX, mouseY};

        static const std::unordered_map<SDL_Keycode, std::function<bool(Event&)>> keyHandlers = {
            {SDLK_TAB, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Tab; return true; }},
            {SDLK_LEFT, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Left; return true; }},
            {SDLK_RIGHT, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Right; return true; }},
            {SDLK_UP, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Up; return true; }},
            {SDLK_DOWN, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Down; return true; }},
            {SDLK_a, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::A; return true; }},
            {SDLK_b, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::B; return true; }},
            {SDLK_c, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::C; return true; }},
            {SDLK_d, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::D; return true; }},
            {SDLK_e, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::E; return true; }},
            {SDLK_f, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::F; return true; }},
            {SDLK_g, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::G; return true; }},
            {SDLK_h, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::H; return true; }},
            {SDLK_i, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::I; return true; }},
            {SDLK_j, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::J; return true; }},
            {SDLK_k, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::K; return true; }},
            {SDLK_l, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::L; return true; }},
            {SDLK_m, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::M; return true; }},
            {SDLK_n, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::N; return true; }},
            {SDLK_o, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::O; return true; }},
            {SDLK_p, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::P; return true; }},
            {SDLK_q, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Q; return true; }},
            {SDLK_r, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::R; return true; }},
            {SDLK_s, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::S; return true; }},
            {SDLK_t, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::T; return true; }},
            {SDLK_u, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::U; return true; }},
            {SDLK_v, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::V; return true; }},
            {SDLK_w, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::W; return true; }},
            {SDLK_x, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::X; return true; }},
            {SDLK_y, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Y; return true; }},
            {SDLK_z, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Z; return true; }},
            {SDLK_1, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num1; return true; }},
            {SDLK_2, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num2; return true; }},
            {SDLK_3, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num3; return true; }},
            {SDLK_4, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num4; return true; }},
            {SDLK_5, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num5; return true; }},
            {SDLK_6, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num6; return true; }},
            {SDLK_7, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num7; return true; }},
            {SDLK_8, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num8; return true; }},
            {SDLK_9, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num9; return true; }},
            {SDLK_0, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num0; return true; }},
            {SDLK_KP_0, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num0; return true; }},
            {SDLK_KP_1, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num1; return true; }},
            {SDLK_KP_2, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num2; return true; }},
            {SDLK_KP_3, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num3; return true; }},
            {SDLK_KP_4, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num4; return true; }},
            {SDLK_KP_5, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num5; return true; }},
            {SDLK_KP_6, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num6; return true; }},
            {SDLK_KP_7, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num7; return true; }},
            {SDLK_KP_8, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num8; return true; }},
            {SDLK_KP_9, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Num9; return true; }},
            {SDLK_SPACE, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Space; return true; }},
            {SDLK_RETURN, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Enter; return true; }},
            {SDLK_BACKSPACE, [](Event& e){ e.type = Type::KeyPressed; e.code = Key::Backspace; return true; }}

            
        };

        static const std::unordered_map<Uint8, std::function<bool(Event&, const SDL_Event&)>> mouseHandlers = {
            {SDL_BUTTON_LEFT, [](Event& e, const SDL_Event& ev){
                e.type = Type::KeyPressed;
                e.code = Key::MouseLeft;
                e.mouse = {ev.button.x, ev.button.y};
                return true;
            }},
            {SDL_BUTTON_RIGHT, [](Event& e, const SDL_Event& ev){
                e.type = Type::KeyPressed;
                e.code = Key::MouseRight;
                e.mouse = {ev.button.x, ev.button.y};
                return true;
            }}
        };

        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    event.type = Type::Quit;
                    return true;
                case SDL_KEYDOWN:
                    if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                        event.type = Type::Quit;
                        return true;
                    }
                    if (auto it = keyHandlers.find(sdlEvent.key.keysym.sym); it != keyHandlers.end())
                        return it->second(event);
                    break;
                case SDL_MOUSEBUTTONDOWN: {
                    if (auto it = mouseHandlers.find(sdlEvent.button.button); it != mouseHandlers.end())
                        return it->second(event, sdlEvent);
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
