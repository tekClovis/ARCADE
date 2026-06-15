/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** AEntity
*/

#include "AEntity.hpp"

#include <cstdint>
#include <limits>
#include <vector>

void Arc::AEntity::setDirection(DIRECTION direction)
{
    _direction = direction;
}


void Arc::AEntity::move(AMap& map)
{
    const int width = static_cast<int>(map.getWidth());
    const int height = static_cast<int>(map.getHeight());

    auto inBounds = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < width && y < height;
    };

    auto walkable = [&](float x, float y) -> bool {
        const int ix = static_cast<int>(std::round(x));
        const int iy = static_cast<int>(std::round(y));
        if (!inBounds(ix, iy))
            return false;
        return map(static_cast<std::size_t>(ix), static_cast<std::size_t>(iy)) != 1;
    };

    float nx = _position.first  + (float)_nextDirection.first;
    float ny = _position.second + (float)_nextDirection.second;

    if (walkable(nx, ny)) {
        const DIRECTION previousDirection = _direction;
        setDirection(_nextDirection);
        const bool changedDirection = (previousDirection.first != _direction.first) || (previousDirection.second != _direction.second);
        const bool isMoving = !(_direction.first == 0 && _direction.second == 0);
        if (changedDirection && isMoving) {
            if (_direction.first != 0)
                _position.second = std::round(_position.second);
            else                              
                _position.first  = std::round(_position.first);
        }
    }
    float fx = _position.first  + (float)_direction.first  * 0.5f;
    float fy = _position.second + (float)_direction.second * 0.5f;

    if (!walkable(fx, fy))
        _direction = {0, 0};
    _position.first  += (float)_direction.first  * _speed;
    _position.second += (float)_direction.second * _speed;
}

int Arc::AEntity::gotoPoint(AMap& map, POSITION target) {
    const int width = static_cast<int>(map.getWidth());
    const int height = static_cast<int>(map.getHeight());

    const int startX = static_cast<int>(std::round(_position.first));
    const int startY = static_cast<int>(std::round(_position.second));
    const int targetX = static_cast<int>(std::round(target.first));
    const int targetY = static_cast<int>(std::round(target.second));

    auto inBounds = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < width && y < height;
    };
    auto walkable = [&](int x, int y) {
        if (!inBounds(x, y)) return false;
        return map(static_cast<std::size_t>(x), static_cast<std::size_t>(y)) != 1;
    };

    if (!inBounds(startX, startY) || !inBounds(targetX, targetY))
        return -1;
    if (!walkable(startX, startY) || !walkable(targetX, targetY))
        return -1;
    if (startX == targetX && startY == targetY)
        return 0;

    for (int pass = 0; pass < 2; pass++) {
        bool allowReverse = (pass == 1);

        struct Node { int x, y; DIRECTION first_dir; DIRECTION last_dir; int dist; };
        std::vector<Node> queue;
        std::vector<bool> visited(width * height, false);

        const std::vector<DIRECTION> dirs = {
            DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_UP
        };

        for (const auto& dir : dirs) {
            if (!allowReverse) {
                if (_direction.first == 0 && _direction.second == 0) {
                    if (dir.first == -1 * _nextDirection.first && dir.second == -1 * _nextDirection.second) {
                        continue;
                    }
                } else if (dir.first == -1 * _direction.first && dir.second == -1 * _direction.second) {
                    continue;
                }
            }

            int nx = startX + dir.first;
            int ny = startY + dir.second;
            if (walkable(nx, ny)) {
                queue.push_back({nx, ny, dir, dir, 1});
                visited[ny * width + nx] = true;
            }
        }
        visited[startY * width + startX] = true;

        int head = 0;
        while (head < static_cast<int>(queue.size())) {
            Node curr = queue[head++];

            if (curr.x == targetX && curr.y == targetY) {
                _nextDirection = curr.first_dir;
                return curr.dist;
            }

            for (const auto& dir : dirs) {
                if (dir.first == -1 * curr.last_dir.first && dir.second == -1 * curr.last_dir.second) {
                    continue;
                }

                int nx = curr.x + dir.first;
                int ny = curr.y + dir.second;
                
                if (walkable(nx, ny) && !visited[ny * width + nx]) {
                    visited[ny * width + nx] = true;
                    queue.push_back({nx, ny, curr.first_dir, dir, curr.dist + 1});
                }
            }
        }
    }

    return -1;
}
