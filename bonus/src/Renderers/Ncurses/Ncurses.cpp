/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Ncurses
*/

#include "Ncurses.hpp"

namespace {
    std::vector<std::string> splitLines(const std::string& text)
    {
        std::vector<std::string> lines;
        std::string current;

        for (char c : text) {
            if (c == '\n') {
                lines.push_back(current);
                current.clear();
            } else {
                current.push_back(c);
            }
        }
        lines.push_back(current);
        return lines;
    }
}

namespace Arc {

    Ncurses::Ncurses()
    {
        _initialized = false;
        _logicalWidth = 1920;
        _logicalHeight = 1080;
    }

    Ncurses::~Ncurses()
    {
        if (_initialized)
            endwin();
    }

    void Ncurses::createWindow(int width, int height, const std::string& title)
    {
        if (title == "")
            return;

        _logicalWidth = (width > 0) ? width : 1920;
        _logicalHeight = (height > 0) ? height : 1080;
        if (_initialized)
            return;
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
        _initialized = true;
    }

    int Ncurses::mapXToCol(float x) const
    {
        if (COLS <= 1)
            return 0;
        const float normalized = std::clamp(x / static_cast<float>(_logicalWidth), 0.0f, 1.0f);

        return static_cast<int>(std::round(normalized * static_cast<float>(COLS - 1)));
    }

    int Ncurses::mapYToRow(float y) const
    {
        if (LINES <= 1)
            return 0;
        const float normalized = std::clamp(y / static_cast<float>(_logicalHeight), 0.0f, 1.0f);

        return static_cast<int>(std::round(normalized * static_cast<float>(LINES - 1)));
    }

    int Ncurses::mapColToX(int col) const
    {
        if (COLS <= 1)
            return 0;
        const float normalized = std::clamp(static_cast<float>(col) / static_cast<float>(COLS - 1), 0.0f, 1.0f);

        return static_cast<int>(std::round(normalized * static_cast<float>(_logicalWidth)));
    }

    int Ncurses::mapRowToY(int row) const
    {
        if (LINES <= 1)
            return 0;
        const float normalized = std::clamp(static_cast<float>(row) / static_cast<float>(LINES - 1), 0.0f, 1.0f);

        return static_cast<int>(std::round(normalized * static_cast<float>(_logicalHeight)));
    }

    void Ncurses::drawTextCentered(const Arc::Item& item, const std::string& text)
    {
        
        if (text.empty() || LINES <= 0 || COLS <= 0)
            return;
        const int centerCol = mapXToCol(item.position.first);
        const int centerRow = mapYToRow(item.position.second);
        const std::vector<std::string> lines = splitLines(text);
        const int lineOffset = static_cast<int>(lines.size() / 2);

        for (std::size_t i = 0; i < lines.size(); ++i) {
            const int row = centerRow + static_cast<int>(i) - lineOffset;
            if (row < 0 || row >= LINES)
                continue;
            const std::string& line = lines[i];
            int col = centerCol - static_cast<int>(line.size() / 2);

            for (char c : line) {
                if (col >= 0 && col < COLS)
                    mvaddch(row, col, c);
                ++col;
            }
        }
    }

    void Ncurses::display(std::map<std::string, Arc::Item>& items)
    {
        clear();
        if (!_initialized)
            return;
        erase();

        std::vector<std::map<std::string, Arc::Item>::iterator> sortedItems;

        sortedItems.reserve(items.size());
        for (auto it = items.begin(); it != items.end(); ++it)
            if (!(it->second.size == std::pair<float, float>{0.0f, 0.0f}) && !it->second.text.empty())
                sortedItems.push_back(it);

        std::sort(sortedItems.begin(), sortedItems.end(), [](const auto& first, const auto& second) {
            return first->second.drawOrder < second->second.drawOrder;
        });

        for (auto it : sortedItems) {
            auto& item = it->second;
            if (!item.text.empty())
                drawTextCentered(item, item.text);
        }
        refresh();
    }

    bool Ncurses::pollEvent(Event& event)
    {
        if (!_initialized)
            return false;
        const int ch = getch();

        if (ch == ERR)
            return false;
        event.type = Type::KeyPressed;
        event.code = Key::None;

        if (ch == 27) {
            event.type = Type::Quit;
            return true;
        }
        if (ch == '\t' || ch == KEY_BTAB) {
            event.code = Key::Tab;
            return true;
        }
        if (ch == ' ') {
            event.code = Key::Space;
            return true;
        }
        if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            event.code = Key::Enter;
            return true;
        }
        if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            event.code = Key::Backspace;
            return true;
        }
        if (ch == KEY_LEFT) {
            event.code = Key::Left;
            return true;
        }
        if (ch == KEY_RIGHT) {
            event.code = Key::Right;
            return true;
        }
        if (ch == KEY_UP) {
            event.code = Key::Up;
            return true;
        }
        if (ch == KEY_DOWN) {
            event.code = Key::Down;
            return true;
        }
        if (ch == '-') {
            event.code = Key::Dash;
            return true;
        }
        if (ch == '_') {
            event.code = Key::Underscore;
            return true;
        }
        if (ch >= 'a' && ch <= 'z') {
            event.code = static_cast<Key>(Key::A + (ch - 'a'));
            return true;
        }
        if (ch >= 'A' && ch <= 'Z') {
            event.code = static_cast<Key>(Key::A + (ch - 'A'));
            return true;
        }
        if (ch >= '1' && ch <= '9') {
            event.code = static_cast<Key>(Key::Num1 + (ch - '1'));
            return true;
        }
        if (ch == '0') {
            event.code = Key::Num0;
            return true;
        }
        if (ch == KEY_MOUSE) {
            MEVENT mouseEvent;

            if (getmouse(&mouseEvent) != OK)
                return false;
            event.mouse.x = mapColToX(mouseEvent.x);
            event.mouse.y = mapRowToY(mouseEvent.y);
            if (mouseEvent.bstate & BUTTON1_PRESSED) {
                event.code = Key::MouseLeft;
                return true;
            }
            if (mouseEvent.bstate & BUTTON3_PRESSED) {
                event.code = Key::MouseRight;
                return true;
            }
            return false;
        }
        return false;
    }
}

extern "C" {
    Arc::IRenderer* entryPoint(std::map<std::string, Arc::Item>& items)
    {
        if (items.empty())
            return nullptr;
        return new Arc::Ncurses();
    }

    int getLibraryType()
    {
        return static_cast<int>(Arc::LibraryType::Renderer);
    }

    std::string getName()
    {
        return "Ncurses";
    }
}
