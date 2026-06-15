##
## EPITECH PROJECT, 2026
## Arcade
## File description:
## Makefile
##

NAME = arcade

CC = g++

SRC =	src/main.cpp \
		src/Core/Core.cpp \
		src/Parsing/Parsing.cpp \
		src/Menu/Menu.cpp \

CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -fPIC -g \
	-I src/ \
	-I src/Menu/ \
	-I src/Core/ \
	-I include/ \
	-I src/Renderers/ \
	-I src/Games/ \
	-I src/Games/Pacman/ \
	-I src/Games/Utils/ \
	-I src/Games/Tron/ \
	-I src/Games/Pacman/Entities/ \
	-I src/Games/Snake/ \
	-I src/Games/Snake/Fruits/ \
	-I src/Parsing/ \

NCURSES_SRC = src/Renderers/Ncurses/Ncurses.cpp
SFML_SRC = src/Renderers/SFML/SFML.cpp
SDL2_SRC = src/Renderers/SDL2/SDL2.cpp

PACMAN_SRC = src/Games/Pacman/Pacman.cpp \
			 src/Games/Pacman/PacmanMap.cpp \
			 src/Games/Pacman/Entities/PacmanPlayer.cpp \
			 src/Games/Pacman/Entities/AGhost.cpp \
			 src/Games/Utils/AEntity.cpp \
			 src/Games/Utils/IEntity.cpp \
			 src/Games/Utils/AMap.cpp
TRON_SRC = src/Games/Tron/Tron.cpp
SNAKE_SRC = src/Games/Snake/Snake.cpp \
			src/Games/Snake/SnakeMap.cpp \
			src/Games/Snake/SnakePlayer.cpp \
			src/Games/Snake/Fruits/BasicFruit.cpp \
			src/Games/Snake/Fruits/TemporaryFruit.cpp \


OBJ = $(SRC:.cpp=.o)
NCURSES_OBJ = $(NCURSES_SRC:.cpp=.o)
SFML_OBJ = $(SFML_SRC:.cpp=.o)
SDL2_OBJ = $(SDL2_SRC:.cpp=.o)

SNAKE_OBJ = $(SNAKE_SRC:.cpp=.o)
PACMAN_OBJ = $(PACMAN_SRC:.cpp=.o)
TRON_OBJ = $(TRON_SRC:.cpp=.o)

LDLIBS = -ldl

LIBDIR = lib
SFML_LIB = $(LIBDIR)/arcade_sfml.so
SDL2_LIB = $(LIBDIR)/arcade_sdl2.so
NCURSES_LIB = $(LIBDIR)/arcade_ncurses.so

PACMAN_LIB = $(LIBDIR)/arcade_pacman.so
TRON_LIB = $(LIBDIR)/arcade_tron.so
SNAKE_LIB = $(LIBDIR)/arcade_snake.so


all: $(NAME) $(NCURSES_LIB) $(SFML_LIB) $(SDL2_LIB) $(TRON_LIB) $(PACMAN_LIB) $(SNAKE_LIB)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDLIBS)

$(NCURSES_LIB): $(NCURSES_OBJ)
	mkdir -p $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared -o $@ $^ -lncurses

$(SFML_LIB): $(SFML_OBJ)
	mkdir -p $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

$(SDL2_LIB): $(SDL2_OBJ)
	mkdir -p $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared -o $@ $^ -lSDL2 -lSDL2_ttf -lSDL2_image

$(PACMAN_LIB): $(PACMAN_OBJ)
	mkdir -p $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared -o $@ $^

$(TRON_LIB): $(TRON_OBJ)
	mkdir -p $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared -o $@ $^

$(SNAKE_LIB): $(SNAKE_OBJ)
	mkdir -p $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared -o $@ $^

clean:
	rm -f $(OBJ) $(NCURSES_OBJ) $(SFML_OBJ) $(SDL2_OBJ) $(PACMAN_OBJ) $(TRON_OBJ) $(SNAKE_OBJ)

fclean: clean
	rm -f $(NAME) $(NCURSES_LIB) $(SFML_LIB) $(SDL2_LIB) $(PACMAN_LIB) $(TRON_LIB) $(SNAKE_LIB)

re: fclean all

.PHONY: all clean fclean re core games graphicals

core: $(NAME)

games: $(PACMAN_LIB) $(TRON_LIB) $(SNAKE_LIB)

graphicals: $(NCURSES_LIB) $(SFML_LIB) $(SDL2_LIB)
