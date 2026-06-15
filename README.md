# Arcade

A modular retro arcade platform written in **C++20**. The core program loads
**graphics libraries** and **games** dynamically at runtime as shared objects
(`.so`), so you can swap the renderer or the game on the fly without restarting.

| Games | Renderers |
|-------|-----------|
| 🟡 Pac-Man · 🐍 Snake · 🏍️ Tron (bonus) | SFML · SDL2 · ncurses |

The architecture is plugin-based: `./arcade` is just a thin core that opens a
renderer shared library (passed as an argument) and then loads game libraries
through a common interface (`IGame` / `IRenderer`). This means renderers and
games are completely decoupled and interchangeable.

---

## Requirements

You need a Linux system (or WSL on Windows) with an X11 display available, plus
a C++20 toolchain and the development packages for each renderer.

| Tool / Library | Purpose | Minimum |
|----------------|---------|---------|
| `g++` | C++20 compiler | GCC 10+ |
| `make` | Build system | any |
| SFML | SFML renderer | 2.5+ |
| SDL2 + SDL2_ttf + SDL2_image | SDL2 renderer | 2.0+ |
| ncurses | terminal renderer | any |

> **Display note:** the program requires a graphical environment (the `DISPLAY`
> environment variable must be set). It will refuse to run from a pure TTY.

### Install the dependencies

**Debian / Ubuntu**

```sh
sudo apt update
sudo apt install -y build-essential \
    libsfml-dev \
    libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev \
    libncurses-dev
```

**Fedora**

```sh
sudo dnf install -y gcc-c++ make \
    SFML-devel \
    SDL2-devel SDL2_ttf-devel SDL2_image-devel \
    ncurses-devel
```

**Arch Linux**

```sh
sudo pacman -S --needed base-devel sfml sdl2 sdl2_ttf sdl2_image ncurses
```

**Windows**

Use [WSL2](https://learn.microsoft.com/windows/wsl/install) with an Ubuntu
distribution and a working X server (WSLg on Windows 11 works out of the box),
then follow the Debian/Ubuntu steps above.

**macOS** (via [Homebrew](https://brew.sh), requires XQuartz for X11)

```sh
brew install sfml sdl2 sdl2_ttf sdl2_image ncurses
```

---

## Build

Clone the repository and build everything (the core, all renderers and all
games):

```sh
git clone https://github.com/<your-username>/arcade.git
cd arcade
make
```

Useful targets:

| Command | Builds |
|---------|--------|
| `make` / `make all` | Core + all renderers + all games |
| `make core` | Only the `./arcade` core binary |
| `make graphicals` | Only the renderer libraries (SFML, SDL2, ncurses) |
| `make games` | Only the game libraries (Pac-Man, Tron, Snake) |
| `make clean` | Remove object files |
| `make fclean` | Remove objects, binary and `.so` libraries |
| `make re` | Full rebuild |

The renderer and game shared libraries are produced in the `lib/` directory.

---

## Run

Launch the core with the path to the renderer library you want to start with:

```sh
./arcade ./lib/arcade_sfml.so      # start with SFML
./arcade ./lib/arcade_sdl2.so      # start with SDL2
./arcade ./lib/arcade_ncurses.so   # start with ncurses
```

From the menu you can enter your player name, pick a game and a renderer, and
start playing. Press **Tab** at any time to cycle through the available
renderers live.

### Controls

| Key | Action |
|-----|--------|
| **Tab** | Switch renderer on the fly |
| **Arrow keys** | Navigate menu / move player |
| **Enter** | Confirm / select |
| **Escape** | Back / quit |

Per-game notes:

- **Snake** — arrow keys to steer, **Space** to boost.
- **Tron** (2 players) — Player 1: arrow keys · Player 2: **Z Q S D**.
- **Pac-Man** — arrow keys to move.

High scores are stored per player in the `Scores/` directory.

---

## Project structure

```
arcade/
├── src/
│   ├── main.cpp            # entry point
│   ├── Core/               # plugin loader & main loop
│   ├── Menu/               # interactive menu
│   ├── Parsing/            # argument / config parsing
│   ├── Renderers/          # SFML, SDL2, ncurses backends
│   └── Games/              # Pac-Man, Snake, Tron (+ shared Utils)
├── include/                # public interfaces (IGame, IRenderer, Event, ...)
├── lib/                    # compiled .so libraries (generated)
├── assets/                 # textures, fonts and sounds
├── doc/                    # architecture manual & integration guide
├── bonus/                  # bonus content (e.g. Minesweeper)
└── Makefile
```

## Documentation

See the [`doc/`](./doc) directory for the architecture manual, the library
integration guide and the class diagram. If you want to write your own renderer
or game, implement the `IRenderer` / `IGame` interfaces in
[`include/`](./include) and drop the resulting `.so` into `lib/`.

## License

This project was originally built as part of the Epitech curriculum. Feel free
to read, learn from and adapt it.
