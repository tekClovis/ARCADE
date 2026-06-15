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

// namespace {
//     // Recursive shortest-path (DFS + memoization).
//     // Returns distance in steps from (x,y) to (tx,ty), or -1 if unreachable.
//     int dfsShortest(Arc::AMap& map,
//         int x, int y,
//         int tx, int ty,
//         int width, int height,
//         std::vector<int>& memo,
//         std::vector<std::uint8_t>& state)
//     {
//         auto inBounds = [&](int px, int py) {
//             return px >= 0 && py >= 0 && px < width && py < height;
//         };
//         auto walkable = [&](int px, int py) {
//             return map(static_cast<std::size_t>(px), static_cast<std::size_t>(py)) != 1;
//         };

//         if (!inBounds(x, y))
//             return -1;
//         if (!walkable(x, y))
//             return -1;
//         if (x == tx && y == ty)
//             return 0;

//         const int i = y * width + x;
//         if (state[i] == 1)
//             return -1; // cycle in current recursion path
//         if (state[i] == 2)
//             return memo[i]; // cached

//         state[i] = 1;

//         static const int dirs[4][2] = {
//             {0, -1},
//             {0, 1},
//             {-1, 0},
//             {1, 0},
//         };

//         int best = std::numeric_limits<int>::max();
//         for (const auto& d : dirs) {
//             const int nx = x + d[0];
//             const int ny = y + d[1];
//             const int v = dfsShortest(map, nx, ny, tx, ty, width, height, memo, state);
//             if (v >= 0 && v + 1 < best)
//                 best = v + 1;
//         }

//         state[i] = 2;
//         memo[i] = (best == std::numeric_limits<int>::max()) ? -1 : best;
//         return memo[i];
//     }
// }

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
        // Snap only when we actually start/turn into a direction.
        // If we are standing still (0,0), snapping here would destroy fractional positions.
        const bool changedDirection = (previousDirection.first != _direction.first) || (previousDirection.second != _direction.second);
        const bool isMoving = !(_direction.first == 0 && _direction.second == 0);
        if (changedDirection && isMoving) {
            // Snap the perpendicular axis to prevent diagonal drift on turns
            if (_direction.first != 0)        // going horizontal → lock Y
                _position.second = std::round(_position.second);
            else                              // going vertical   → lock X
                _position.first  = std::round(_position.first);
        }
    }
    // ── 2. Wall check in current direction (0.5 cell lookahead) ──────────
    float fx = _position.first  + (float)_direction.first  * 0.5f;
    float fy = _position.second + (float)_direction.second * 0.5f;

    if (!walkable(fx, fy))
        _direction = {0, 0};
    // ── 3. Move ───────────────────────────────────────────────────────────
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

    if (!inBounds(startX, startY) || !inBounds(targetX, targetY)) return -1;
    if (!walkable(startX, startY) || !walkable(targetX, targetY)) return -1;
    if (startX == targetX && startY == targetY) {
        // float dx = target.first - _position.first;
        // float dy = target.second - _position.second;
        
        // if (std::abs(dx) < 0.10f && std::abs(dy) < 0.10f) {
        //     return 0;
        // }

        // if (std::abs(dx) > std::abs(dy)) {
        //     _nextDirection = {dx > 0 ? 1 : -1, 0};
        // } else {
        //     _nextDirection = {0, dy > 0 ? 1 : -1};
        // }
        return 0;
    }

    // 2-Pass BFS: 
    // Pass 0 -> Forbid 180-degree turn relative to the entity's current real direction.
    // Pass 1 -> If no valid path is found (trapped), allow a U-Turn on the first step.
    for (int pass = 0; pass < 2; pass++) {
        bool allowReverse = (pass == 1);

        struct Node { int x, y; DIRECTION first_dir; DIRECTION last_dir; int dist; };
        std::vector<Node> queue;
        std::vector<bool> visited(width * height, false);

        const std::vector<DIRECTION> dirs = {
            DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_UP
        };

        for (const auto& dir : dirs) {
            // Block turning directly backwards on the very first move.
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
                    continue; // No U-turns allowed along the path!
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

    return -1; // Target unreachable
}
