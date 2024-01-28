#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "imgui.h"
#include "imfilebrowser.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "scrubber.hpp"

#ifdef __APPLE__
#include "macos_helpers.mm"
#endif

int main(int, char**) {
  /*
  Scrubber app;

  app.Init();
  app.Run();
  */
  scrbr_screenshot();

  return 0;
}