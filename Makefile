CC = g++ -g

CFLAGS = -std=c++11 -Wall

SRC = app/main.cpp
SRC += libs/imgui/*.cpp
SRC += libs/imgui/backends/imgui_impl_sdlrenderer2.cpp
SRC += libs/imgui/backends/imgui_impl_sdl2.cpp

LIBS =
LIBS += -I libs/imgui
LIBS += -I libs/imgui/backends

ifeq ($(OS), Windows_NT)

	LIBS += -I x86_64-w64-mingw32/include/SDL2
	LIBS += -L x86_64-w64-mingw32/lib

	LIBS += -lmingw32 -lSDL2main -lSDL2

else ifeq ($(shell uname -s), Darwin)

	LIBS += -I libs/SDL2
	LIBS += `sdl2-config --cflags --libs`

endif

all: $(scrubber)
	$(CC) $(SRC) $(LIBS) $(CFLAGS)