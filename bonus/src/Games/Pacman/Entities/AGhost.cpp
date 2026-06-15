/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** AGhost
*/

#include "AGhost.hpp"

void Arc::AGhost::animate()
{
    std::map<DIRECTION, int> dieToSpriteId = {
        {DIRECTION_RIGHT, 78},
        {DIRECTION_UP, 80},
        {DIRECTION_LEFT, 79},
        {DIRECTION_DOWN, 81}
    };
    using clock = std::chrono::steady_clock;
    constexpr auto framePeriod = std::chrono::milliseconds(100);
    constexpr auto bonusBlinkStartDelay = std::chrono::milliseconds(7000);
    constexpr auto bonusBlinkPeriod = std::chrono::milliseconds(300);
    const auto now = clock::now();
    if (now - _lastAnimTime >= framePeriod) {
        _lastAnimTime = now;
        _animation = !_animation;
    }

    _bonusAnimState = false;
    if (_bonus && now - _bonusAnimTime >= bonusBlinkStartDelay) {
        const auto blinkElapsed = (now - _bonusAnimTime) - bonusBlinkStartDelay;
        _bonusAnimState = ((blinkElapsed / bonusBlinkPeriod) % 2) == 1;
    }

    int id = 56;
    if (_die) {
        id = dieToSpriteId[_direction];
    } else if (_bonus) {
        id = _animation + 64 + _bonusAnimState * 2;
    } else {
        id = _spriteId;
        if (_direction.first < 0)
            id = _animation + id + 2;
        else if (_direction.second < 0)
            id = _animation + id + 4;
        else if (_direction.second > 0)
            id = _animation + id + 6;
        else
            id += _animation;
    }
    _currentFrameId = id;
}

std::string Arc::AGhost::getTexturePath()
{
    if (_currentFrameId == 0)
        animate();
    return "./assets/textures/pacman/ghost/" + std::to_string(_currentFrameId) + ".png";
}

void Arc::AGhost::move(AMap& map)
{
    // Helper: is cell walkable (not a wall)?
    auto walkable = [&](float x, float y) -> bool {
        if (map((int)std::round(x), (int)std::round(y)) == 4 && (_spawned && !_die && _respawned)) // allow walking through spawn gate if not spawned yet or if dead 
            return false;
        return map((int)std::round(x), (int)std::round(y)) != 1;
    };
    auto tpPoint = [&](float x, float y) -> bool {
        return map((int)std::round(x), (int)std::round(y)) == static_cast<std::uint8_t>(Arc::PacmanMap::TpPoint);
    };

   
    float nx = _position.first  + (float)_nextDirection.first ;
    float ny = _position.second + (float)_nextDirection.second;

    if (walkable(nx, ny)) {
        const DIRECTION previousDirection = _direction;
        setDirection(_nextDirection);
        const bool changedDirection = (previousDirection.first != _direction.first) || (previousDirection.second != _direction.second);
        const bool isMoving = !(_direction.first == 0 && _direction.second == 0);
        if (changedDirection && isMoving && _spawned) {
            // Snap the perpendicular axis to prevent diagonal drift on turns
            if (_direction.first != 0)      
                _position.second = std::round(_position.second);
            else                            
                _position.first  = std::round(_position.first);
        }
    }
    if (tpPoint(_position.first, _position.second)) {
        if (_position.first < map.getWidth() / 2)
            _position.first = map.getWidth() - 1.5;
        else
            _position.first = 0.5;
    }
    float fx = _position.first  + (float)_direction.first  * (0.5f + 0.5f * !_spawned);
    float fy = _position.second + (float)_direction.second * (0.5f + 0.5f * !_spawned);

    if (!walkable(fx, fy)){
        setDirection({0, 0});
        return;
    }
    float speed = _bonus ? 0.07f : _speed;
    _position.first  += (float)_direction.first  * speed;
    _position.second += (float)_direction.second * speed;
}

void Arc::AGhost::randomChoose(AMap& map) {
    DIRECTION lastDir = _direction;
    if (lastDir.first == 0 && lastDir.second == 0)
        lastDir = _nextDirection;

    std::map<DIRECTION, std::vector<DIRECTION>> dirAuth = {
        {{0, 0}, {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT}},
        {DIRECTION_UP, {DIRECTION_UP, DIRECTION_LEFT, DIRECTION_RIGHT}},
        {DIRECTION_DOWN, {DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT}},
        {DIRECTION_LEFT, {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT}},
        {DIRECTION_RIGHT, {DIRECTION_UP, DIRECTION_DOWN, DIRECTION_RIGHT}}
    };

    std::vector<DIRECTION>& possibleDirs = dirAuth[lastDir];
    std::vector<DIRECTION> validDirs;

    // Test all allowed directions to see which ones don't hit a wall
    for (const auto& testDir : possibleDirs) {
        POSITION tmpPos = _position;
        DIRECTION tmpDir = _direction;
        _nextDirection = testDir;
        move(map);
        if (_position != tmpPos) { // Successfully moved, so it's a valid path
            validDirs.push_back(testDir);
        }
        // Revert entity state
        _position = tmpPos;
        _direction = tmpDir;
    }

    if (!validDirs.empty()) {
        _nextDirection = validDirs[std::rand() % validDirs.size()];
    } else {
        // We are in a dead-end, the only way out is to reverse
        _nextDirection = {-lastDir.first, -lastDir.second};
    }
}

void Arc::AGhost::update(AMap& map)
{
        if (!_spawned){
            if (_moveCycle + _spawnTiming < std::chrono::steady_clock::now()) {
                if (gotoPoint(map, {13.5f, 11.0f}) == 0) {
                    _spawned = true;
                    _moveCycle = std::chrono::steady_clock::now();
                    return;
                }
            } else {
                if (_direction == std::make_pair(0, 0))
                    _nextDirection = _nextDirection == DIRECTION_UP ? DIRECTION_DOWN : DIRECTION_UP;
            }
            move(map);
            animate();
            return;
        }
        if (_respawned == 0 && gotoPoint(map, {13.5f, 11.0f}) == 0)
            _respawned = true;
        if (_die) {
            if ((_respawned = gotoPoint(map, {13.5f, 14.0f})) == 0){
                _die = false;
                _bonus = false;
            };
        } else if (
            _lastCompute + std::chrono::milliseconds(150) < std::chrono::steady_clock::now()
            || _nextDirection == std::make_pair(0, 0)
        ) {
            if (_bonus)
                randomChoose(map);
            else {
                // scatter cycle
                if (_moveCycle + std::chrono::milliseconds(15000) > std::chrono::steady_clock::now())
                    gotoPoint(map, _scatterTarget);
                // chase cycle
                else if (_moveCycle + std::chrono::milliseconds(27000) < std::chrono::steady_clock::now()){
                    _moveCycle = std::chrono::steady_clock::now(); // force a move to prevent idle behavior
                } else
                    gotoPoint(map, _pacman->getPosition());
            }
            _lastCompute = std::chrono::steady_clock::now();
        }
        move(map);
        animate();
};