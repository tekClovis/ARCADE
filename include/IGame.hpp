/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** IGame
*/

#ifndef IGAME_HPP_
#define IGAME_HPP_

#include "Event.hpp"
#include <string>
#include <vector>

namespace Arc {
    enum class LibraryType {
        Game,
        Renderer
    };

    class IGame {
        public:
            virtual ~IGame() = default;
            virtual void update(Event event) = 0;
            virtual void handleKey(Event event) = 0;
            virtual std::vector<int> getScore() const = 0;
            virtual bool isGameOver() const = 0;

        protected:
        private:
    };
}

#endif /* !IGAME_HPP_ */
