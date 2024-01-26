#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "imgui.h"
#include "imfilebrowser.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "imgui_theme.hpp"

#include "scrubber.hpp"

int main(int, char**) {
  Scrubber app;

  app.Init();
  app.Run();

  return 0;
}