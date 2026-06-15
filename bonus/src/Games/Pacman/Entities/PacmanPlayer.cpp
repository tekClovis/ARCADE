/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** PacmanPlayer
*/

#include "PacmanPlayer.hpp"
#include <chrono>

void Arc::PacmanPlayer::move(AMap& map)
{
    auto walkable = [&](float x, float y) -> bool {
        return map((int)std::round(x), (int)std::round(y)) != 1 && map((int)std::round(x), (int)std::round(y)) != 4;
    };
    auto tpPoint = [&](float x, float y) -> bool {
        return map((int)std::round(x), (int)std::round(y)) == static_cast<std::uint8_t>(Arc::PacmanMap::TpPoint);
    };

    float nx = _position.first  + (float)_nextDirection.first;
    float ny = _position.second + (float)_nextDirection.second;

    if (walkable(nx, ny)) {
        const DIRECTION previousDirection = _direction;
        setDirection(_nextDirection);
        const bool changedDirection = (previousDirection.first != _direction.first) || (previousDirection.second != _direction.second);
        const bool isMoving = !(_direction.first == 0 && _direction.second == 0);
        if (changedDirection && isMoving) {
            if (_direction.first != 0)        // going horizontal → lock Y
                _position.second = std::round(_position.second);
            else                              // going vertical   → lock X
                _position.first  = std::round(_position.first);
        }
    }
    if (tpPoint(_position.first, _position.second)) {
        if (_position.first < map.getWidth() / 2)
            _position.first = map.getWidth() - 1.5;
        else
            _position.first = 0.5;
    }
    float fx = _position.first  + (float)_direction.first  * 0.5f;
    float fy = _position.second + (float)_direction.second * 0.5f;

    if (!walkable(fx, fy))
        return;
    _position.first  += (float)_direction.first  * _speed;
    _position.second += (float)_direction.second * _speed;
}

void Arc::PacmanPlayer::update(AMap& map)
{
    animate();
    if (_die || _endLevel)
        return;
    move(map);
}

void Arc::PacmanPlayer::animate()
{
    const std::vector<std::size_t> endSprites = {16, 19, 2, 21};
    std::map<DIRECTION, std::size_t> directionToSpriteId = {
        {{-1, 0}, 15}, // Left
        {{0, -1}, 18}, // Up
        {{1, 0}, 1},  // Right
        {{0, 1}, 20}   // Down
    };
    using clock = std::chrono::steady_clock;
    std::size_t framePeriod = 150;

    const auto now = clock::now();
    if (now - _lastAnimTime >= std::chrono::milliseconds(framePeriod)) {
        _lastAnimTime = now;
        if (_die){
            _deathFrame++;
            _spriteId = _deathFrame  < 15 ? static_cast<int>(_deathFrame) : 0;
            return;
        }
        if (_endLevel) {
            _animation = 0;
            _spriteId = endSprites[_endAnim];
            _endAnim = (_endAnim + 1) % directionToSpriteId.size();
            return;
        }
        if (!(_direction.first == 0 && _direction.second == 0) && !_die)
            _animation = !_animation;
        else
            _animation = false;
        }
        if (!_die && !_endLevel)
            _spriteId = directionToSpriteId[_direction] + _animation;
    }

std::string Arc::PacmanPlayer::getTexturePath()
{
    return "./assets/textures/pacman/pac/" + std::to_string(_spriteId) + ".png";
}