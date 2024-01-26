#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "imgui.h"
#include "imfilebrowser.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "imgui_theme.hpp"

int main(int, char**) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }
  IMG_Init(IMG_INIT_JPG);

  // Create window with SDL_Renderer graphics context
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window* window =
      SDL_CreateWindow("scrubber", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    SDL_Log("Error creating SDL_Renderer!");
    return 0;
  }

  SDL_Surface* preview_surface = IMG_Load("hRmaYty.png");
  int src_preview_image_width = preview_surface->w;
  int src_preview_image_height = preview_surface->h;
  SDL_Texture* preview_texture = SDL_CreateTextureFromSurface(renderer, preview_surface);
  if (preview_texture == NULL) {
    printf("Error creating texture");
    return 6;
  }

  SDL_FreeSurface(preview_surface);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

  StyleColorsScrubber();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  ImGui::FileBrowser fileDialog;

  // Our state
  bool display_modifiers = true;

  // Main loop
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    int window_width;
    int window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
      static float gaussian_blur_weight = 0.0f;

      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(ImVec2(0, window_height));
      ImGui::Begin("Tools", 0,
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoCollapse);

      if (ImGui::Button("Browse files")) {
        fileDialog.Open();
      }
      ImGui::SameLine();

      bool is_valid_file = !(std::filesystem::is_directory(fileDialog.GetSelected()));
      const char* display_filename = "";
      if (is_valid_file) {
        display_filename = fileDialog.GetSelected().filename().c_str();
      }
      ImGui::Text("%s", display_filename);

      ImGui::Checkbox("Display modifiers", &display_modifiers);
      ImGui::SliderFloat("Gaussian Blur", &gaussian_blur_weight, 0.0f, 1.0f);

      const char* items[] = {"ass", "bitch", "cock", "cunt", "dick", "fuck", "piss", "pussy", "shit", "slut", "whore"};
      static int item_current = 1;
      ImGui::ListBox("Censored Words", &item_current, items, IM_ARRAYSIZE(items), 10);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    fileDialog.Display();

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(renderer, 96, 96, 96, 0);
    SDL_RenderClear(renderer);

    // Render preview image
    int preview_image_width = (int)(window_width * 0.5);
    int preview_image_height = (int)(window_width * 0.5 * src_preview_image_height / src_preview_image_width);
    int preview_image_x = (int)(window_width / 2 - preview_image_width / 2);
    int preview_image_y = (int)(window_height / 2 - preview_image_height / 2);
    SDL_Rect location = {preview_image_x, preview_image_y, preview_image_width, preview_image_height};
    SDL_RenderCopy(renderer, preview_texture, NULL, &location);

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  }

  // Cleanup
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyTexture(preview_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}