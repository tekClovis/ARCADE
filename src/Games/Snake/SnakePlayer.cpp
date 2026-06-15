/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** SnakePlayer
*/

#include "SnakePlayer.hpp"

namespace Arc {
    SnakePlayer::SnakePlayer(std::map<std::string, Item>& items)
        : _items(items), _isAlive(true)
    {
        _body.push_back({{7, 7}, Direction::RIGHT, Direction::RIGHT, HEAD});
        _body.push_back({{6, 7}, Direction::RIGHT, Direction::RIGHT, BODY});
        _body.push_back({{5, 7}, Direction::RIGHT, Direction::RIGHT, BODY});
        _body.push_back({{4, 7}, Direction::RIGHT, Direction::RIGHT, TAIL});

        _items["snakeHead"] = {{OFFSET_X + _body[0].position.first * TILE_SIZE * SCALE,
            OFFSET_Y + _body[0].position.second * TILE_SIZE * SCALE}, {SCALE, SCALE}, 0, {0, 0}, false, 1,
            "./assets/textures/Snake/player/snakeHead.png", "0", nullptr};
        _items["snakeBody0"] = {{OFFSET_X + _body[1].position.first * TILE_SIZE * SCALE,
            OFFSET_Y + _body[1].position.second * TILE_SIZE * SCALE}, {SCALE, SCALE}, 0, {0, 0}, false, 1,
            "./assets/textures/Snake/player/snakeBody_straight.png", "o", nullptr};
        _items["snakeBody1"] = {{OFFSET_X + _body[2].position.first * TILE_SIZE * SCALE,
            OFFSET_Y + _body[2].position.second * TILE_SIZE * SCALE}, {SCALE, SCALE}, 0, {0, 0}, false, 1,
            "./assets/textures/Snake/player/snakeBody_straight.png", "o", nullptr};
        _items["snakeTail"] = {{OFFSET_X + _body[3].position.first * TILE_SIZE * SCALE,
            OFFSET_Y + _body[3].position.second * TILE_SIZE * SCALE}, {SCALE, SCALE}, 0, {0, 0}, false, 1,
            "./assets/textures/Snake/player/snakeTail_straight.png", ".", nullptr};
    }

    SnakePlayer::~SnakePlayer()
    {
    }

    void SnakePlayer::handleKey(Event event) {
        if (event.type != Type::KeyPressed)
            return;

        const std::map<Key, std::function<void()>> keyHandlers = {
            {Key::Up, [this]() { if (_body[0].lastDirection != Direction::DOWN) _body[0].direction = Direction::UP; }},
            {Key::Down, [this]() { if (_body[0].lastDirection != Direction::UP) _body[0].direction = Direction::DOWN; }},
            {Key::Left, [this]() { if (_body[0].lastDirection != Direction::RIGHT) _body[0].direction = Direction::LEFT; }},
            {Key::Right, [this]() { if (_body[0].lastDirection != Direction::LEFT) _body[0].direction = Direction::RIGHT; }}
        };

        auto it = keyHandlers.find(event.code);
        if (it != keyHandlers.end())
            it->second();
    }

    void SnakePlayer::update() {
        bodyPart newHead = _body[0];

        static const std::map<Direction, std::function<void(std::pair<int, int>&)>> moveHandlers = {
            {Direction::UP, [](std::pair<int, int>& pos) { pos.second--; }},
            {Direction::DOWN, [](std::pair<int, int>& pos) { pos.second++; }},
            {Direction::LEFT, [](std::pair<int, int>& pos) { pos.first--; }},
            {Direction::RIGHT, [](std::pair<int, int>& pos) { pos.first++; }}
        };

        auto it = moveHandlers.find(newHead.direction);
        if (it != moveHandlers.end())
            it->second(newHead.position);
        newHead.lastDirection = newHead.direction;

        auto moveCircular = [](int& position, int max) {
            if (position < 0)
                position = max - 1;
            if (position >= max)
                position = 0;
        };
        moveCircular(newHead.position.first, MAP_SIZE);
        moveCircular(newHead.position.second, MAP_SIZE);

        for (size_t i = _body.size() - 1; i > 0; i--)
            _body[i] = _body[i - 1];
        _body[0] = newHead;

        updateSnakeHead();
        for (size_t i = 1; i < _body.size() - 1; i++)
            updateSnakeBody(i);
        updateSnakeTail();

        for (size_t i = 1; i < _body.size() - 1; i++) {
            if (newHead.position == _body[i].position) {
                _isAlive = false;
                return;
            }
        }
    }

    void SnakePlayer::updateSnakeHead() {
        bodyPart& head = _body[0];
        _items["snakeHead"].texturePath = "./assets/textures/Snake/player/snakeHead.png";

        _items["snakeHead"].rotation = [](Direction direction) {
            static const std::map<Direction, int> rotations =
                { {UP, -90}, {DOWN, 90}, {LEFT, 180}, {RIGHT, 0} };
            return rotations.at(direction);
        }(head.direction);

        _items["snakeHead"].position = {
            OFFSET_X + head.position.first * TILE_SIZE * SCALE,
            OFFSET_Y + head.position.second * TILE_SIZE * SCALE
        };
    }

    std::pair<int, int> SnakePlayer::getRealDistance(std::pair<int, int> from,
        std::pair<int, int> to) const
    {
        int directionX = to.first - from.first;
        int directionY = to.second - from.second;

        if (directionX > MAP_SIZE / 2)
            directionX -= MAP_SIZE;
        if (directionX < -MAP_SIZE / 2)
            directionX += MAP_SIZE;
        if (directionY > MAP_SIZE / 2)
            directionY -= MAP_SIZE;
        if (directionY < -MAP_SIZE / 2)
            directionY += MAP_SIZE;
        return {directionX, directionY};
    }

    void SnakePlayer::updateSnakeBody(size_t index) {
        bodyPart& part = _body[index];
        const bodyPart& prev = _body[index - 1];
        const bodyPart& next = _body[index + 1];
        std::string itemName = "snakeBody" + std::to_string(index - 1);

        auto distancePrev = getRealDistance(part.position, prev.position);
        auto distanceNext = getRealDistance(part.position, next.position);

        int rotation = 0;
        if (distancePrev.first == -distanceNext.first && distancePrev.second == -distanceNext.second) {
            _items[itemName].texturePath = "./assets/textures/Snake/player/snakeBody_straight.png";
            rotation = (distancePrev.first != 0) ? 0 : 90;
        } else {
            _items[itemName].texturePath = "./assets/textures/Snake/player/snakeBody_turn.png";

            auto getTurnRotation = [](const std::pair<int, int>& prev,
                const std::pair<int, int>& next) -> int {
                if ((prev.first == -1 && prev.second ==  0 && next.first ==  0 && next.second == -1) ||
                    (prev.first ==  0 && prev.second == -1 && next.first == -1 && next.second ==  0))
                    return 180;
                if ((prev.first == -1 && prev.second ==  0 && next.first ==  0 && next.second ==  1) ||
                    (prev.first ==  0 && prev.second ==  1 && next.first == -1 && next.second ==  0))
                    return 90;
                if ((prev.first ==  1 && prev.second ==  0 && next.first ==  0 && next.second == -1) ||
                    (prev.first ==  0 && prev.second == -1 && next.first ==  1 && next.second ==  0))
                    return -90;
                if ((prev.first ==  1 && prev.second ==  0 && next.first ==  0 && next.second ==  1) ||
                    (prev.first ==  0 && prev.second ==  1 && next.first ==  1 && next.second ==  0))
                    return 0;
                return 0;
            };
            rotation = getTurnRotation(distancePrev, distanceNext);
        }

        _items[itemName].rotation = rotation;
        _items[itemName].position = {
            OFFSET_X + part.position.first * TILE_SIZE * SCALE,
            OFFSET_Y + part.position.second * TILE_SIZE * SCALE
        };
    }

    void SnakePlayer::updateSnakeTail() {
        bodyPart& tail = _body.back();
        const bodyPart& prev = _body[_body.size() - 2];
        _items["snakeTail"].texturePath = "./assets/textures/Snake/player/snakeTail_straight.png";

        auto distance = getRealDistance(tail.position, prev.position);
        
        Direction tailDirection = [](std::pair<int, int> shift) {
            if (shift.first == 1) return Direction::RIGHT;
            if (shift.first == -1) return Direction::LEFT;
            if (shift.second == 1) return Direction::DOWN;
            return Direction::UP;
        }(distance);

        _items["snakeTail"].rotation = [](Direction direction) {
            static const std::map<Direction, int> rotations =
                { {UP, -90}, {DOWN, 90}, {LEFT, 180}, {RIGHT, 0} };
            return rotations.at(direction);
        }(tailDirection);

        _items["snakeTail"].position = {
            OFFSET_X + tail.position.first * TILE_SIZE * SCALE,
            OFFSET_Y + tail.position.second * TILE_SIZE * SCALE
        };
    }

    bool SnakePlayer::checkCollision(const std::pair<int, int>& pos) {
        return _body[0].position == pos;
    }

    bool SnakePlayer::isAlive() const {
        return _isAlive;
    }

    void SnakePlayer::grow(int length) {
        for (int i = 0; i < length; i++) {
            bodyPart newPart = _body.back();
            _body.push_back(newPart);
            std::string newItemName = "snakeBody" + std::to_string(_body.size() - 3);
            _items[newItemName] = {{0, 0}, {SCALE, SCALE}, 0, {0, 0}, false, 1, "", "o", std::function<void()>()};
        }
    }

    std::vector<std::pair<int, int>> SnakePlayer::getBodyPositions() const {
        std::vector<std::pair<int, int>> positions;

        for (const auto& part : _body)
            positions.push_back(part.position);
        return positions;
    }

    int SnakePlayer::eatFruit(IFruit& fruit) {
        switch (fruit.getType()) {
            case IFruit::BASIC:
                fruit.spawn(getBodyPositions());
                grow(1);
                return 1;
            case IFruit::TEMPORARY:
                fruit.setExpiry();
                grow(2);
                return 2;
        }
        return 0;
    }
}
