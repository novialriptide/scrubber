SOURCES = app/main.cpp
SOURCES += libs/imgui/*.cpp
SOURCES += libs/imgui/backends/imgui_impl_sdlrenderer2.cpp
SOURCES += libs/imgui/backends/imgui_impl_sdl2.cpp

LIBS = 
LIBS += -I libs/imgui
LIBS += -I libs/imgui/backends
LIBS += -I libs/SDL2 `sdl2-config --cflags --libs`

scrubber:
	g++ $(SOURCES) $(LIBS) -std=c++11