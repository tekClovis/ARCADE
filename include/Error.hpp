/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Error
*/

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <exception>
#include <string>

class FailedToLoadRenderer : public std::exception
{
    public:
        FailedToLoadRenderer(const std::string &message) : errorMessage("Failed to load renderer: " + message) {}

        const char *what() const noexcept override
        {
            return errorMessage.c_str();
        }

    private:
        std::string errorMessage;
};

class FailedToLoadGame : public std::exception
{
    public:
        FailedToLoadGame(const std::string &message) : errorMessage("Failed to load game: " + message) {}

        const char *what() const noexcept override
        {
            return errorMessage.c_str();
        }

    private:
        std::string errorMessage;
};



#endif /* !ERROR_HPP_ */
