#define SDL_MAIN_HANDLED

#if _WIN32
#include <windows.h>
#include <Lmcons.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "imgui.h"
#include "imfilebrowser.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "nlohmann/json.hpp"

#include "scrubber.hpp"

void get_center_coordinates(int window_width, int window_height, int image_width, int image_height, int& dest_x,
                            int& dest_y) {
  dest_x = (int)(window_width / 2 - image_width / 2);
  dest_y = (int)(window_height / 2 - image_height / 2);
}

char** strlist(std::vector<std::string>& input) {
  char** charArray = new char*[input.size() + 1];  // +1 for the terminating nullptr
  for (size_t i = 0; i < input.size(); ++i) {
    charArray[i] = new char[input[i].size() + 1];
    std::strcpy(charArray[i], input[i].c_str());
  }
  charArray[input.size()] = nullptr;

  return charArray;
}

Scrubber::Scrubber() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    exit(-1);
  }
  IMG_Init(IMG_INIT_JPG);

  // Create window with SDL_Renderer graphics context
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  this->window = SDL_CreateWindow("scrubber", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  if (this->window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    exit(-1);
  }
  this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (this->renderer == nullptr) {
    SDL_Log("Error creating SDL_Renderer!");
    exit(0);
  }

  CreateSaveDirectory();
  CreateSaveFiles();
  LoadConfig();

  // States
  this->display_modifiers = true;
  this->new_phrase = (char*)malloc(sizeof(char) * (64 + 1));
}

void Scrubber::CreateSaveDirectory() {
#if _WIN32
  TCHAR username[UNLEN + 1];
  DWORD size = UNLEN + 1;
  GetUserName((TCHAR*)username, &size);
  this->kSavePath = "C:\\Users\\" + username + "\\AppData\\scrubber";
#elif __APPLE__
  std::string incomplete_save = std::getenv("HOME");
  incomplete_save += "/Library/Application Support/scrubber";

  this->kSavePath = new char[incomplete_save.length() + 1];
  strcpy(this->kSavePath, incomplete_save.c_str());

#elif __linux__
  this->kSavePath = "/etc/scrubber";
#else
  printf("%s", "Unsupported operating system.");
  exit(0);
#endif

  // Make save directory if it doesn't exist
  if (!std::filesystem::exists(this->kSavePath)) {
    std::filesystem::create_directory(this->kSavePath);
  }
}

void Scrubber::CreateSaveFiles() {
  if (this->kSavePath == nullptr) {
    printf("%s", "Something went wrong.");
    exit(1);
  }

  std::string path = this->kSavePath;
  path += "/config.json";

  if (std::filesystem::exists(path)) {
    return;
  }

  nlohmann::json config;
  config["censored_phrases"] = {"ass",  "bitch", "cock", "cunt", "dick", "fuck",
                                "piss", "pussy", "shit", "slut", "whore"};

  std::ofstream file(path.c_str());
  file << config;
}

void Scrubber::LoadConfig() {
  std::string path = this->kSavePath;
  path += "/config.json";
  std::ifstream config_ifstream(path.c_str());
  nlohmann::json config = nlohmann::json::parse(config_ifstream);
  this->censored_phrases = config.at("censored_phrases");
}

bool Scrubber::SaveConfig() {
  nlohmann::json config;
  config["censored_phrases"] = this->censored_phrases;

  std::string path = this->kSavePath;
  path += "/config.json";

  std::ofstream file(path.c_str());
  file << config;

  return true;
}

void Scrubber::StyleColorsScrubber() {
  auto& colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
  colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

  // Border
  colors[ImGuiCol_Border] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
  colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

  // Text
  colors[ImGuiCol_Text] = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
  colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
  colors[ImGuiCol_CheckMark] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

  // Popups
  colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

  // Slider
  colors[ImGuiCol_SliderGrab] = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
  colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.13f, 0.13, 0.17, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

  // Scrollbar
  colors[ImGuiCol_ScrollbarBg] = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
  colors[ImGuiCol_ScrollbarGrab] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

  // Seperator
  colors[ImGuiCol_Separator] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
  colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
  colors[ImGuiCol_SeparatorActive] = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};

  // Resize Grip
  colors[ImGuiCol_ResizeGrip] = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
  colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
  colors[ImGuiCol_ResizeGripActive] = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};

  auto& style = ImGui::GetStyle();
  style.TabRounding = 4;
  style.ScrollbarRounding = 9;
  style.WindowRounding = 7;
  style.GrabRounding = 3;
  style.FrameRounding = 3;
  style.PopupRounding = 4;
  style.ChildRounding = 4;
}

void Scrubber::Init() {
  LoadImage("hRmaYty.png");
}

void Scrubber::LoadImage(const char* file) {
  SDL_Surface* preview_surface = IMG_Load(file);
  this->preview_image_size.x = preview_surface->w;
  this->preview_image_size.y = preview_surface->h;
  this->preview_texture = SDL_CreateTextureFromSurface(this->renderer, preview_surface);
  if (this->preview_texture == NULL) {
    printf("Error creating texture");
    exit(6);
  }

  SDL_FreeSurface(preview_surface);
}

void Scrubber::Run() {
  IMGUI_CHECKVERSION();
  // Setup Dear ImGui context
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)(io);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

  StyleColorsScrubber();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(this->window, this->renderer);
  ImGui_ImplSDLRenderer2_Init(this->renderer);

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

    int toolbar_width;
    int window_width;
    int window_height;
    SDL_GetWindowSize(this->window, &window_width, &window_height);

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
      toolbar_width = ImGui::GetWindowSize().x;

      if (ImGui::Button("Browse files")) {
        this->fileDialog.Open();
      }
      ImGui::SameLine();

      bool is_valid_file = !(std::filesystem::is_directory(this->fileDialog.GetSelected()));
      const char* display_filename = "";
      if (is_valid_file) {
        display_filename = (char*)(this->fileDialog.GetSelected().filename().c_str());
      }
      ImGui::Text("%s", display_filename);

      ImGui::Checkbox("Display modifiers", &display_modifiers);
      ImGui::SliderFloat("Gaussian Blur", &gaussian_blur_weight, 0.0f, 1.0f);

      char** items = strlist(this->censored_phrases);
      static int item_current = 1;
      ImGui::ListBox("Censored Words", &item_current, items, this->censored_phrases.size(), 10);

      if (ImGui::Button("+")) {
        // this->censored_phrases.insert(this->censored_phrases.begin(), this->new_phrase);
        AddCensoredPhrase(this->new_phrase);
      }
      ImGui::SameLine();
      ImGui::InputText("##", this->new_phrase, 64);
      ImGui::SameLine();
      if (ImGui::Button("-")) {
        RemoveCensoredPhrase(this->new_phrase);
      }

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    this->fileDialog.Display();

    if (this->fileDialog.HasSelected()) {
      printf("lmao\n");
    }

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(this->renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(this->renderer, 96, 96, 96, 0);  // app background
    SDL_RenderClear(this->renderer);                        // clear previous frame

    // Render preview image
    window_width -= toolbar_width;  // dont want it to potentially collide with the toolbar

    int preview_image_width = (int)(window_width * 0.5);
    int preview_image_height = (int)(window_width * 0.5 * this->preview_image_size.y / this->preview_image_size.x);

    int preview_image_x;
    int preview_image_y;
    get_center_coordinates(window_width, window_height, preview_image_width, preview_image_height, preview_image_x,
                           preview_image_y);

    SDL_Rect location = {toolbar_width + preview_image_x, preview_image_y, preview_image_width, preview_image_height};
    SDL_RenderCopy(this->renderer, preview_texture, NULL, &location);

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(this->renderer);
  }

  SaveConfig();

  // Cleanup
  delete[] this->kSavePath;

  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyTexture(preview_texture);
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();

  exit(0);
}

bool Scrubber::AddCensoredPhrase(const char* phrase) {
  if ((std::find(this->censored_phrases.begin(), this->censored_phrases.end(), phrase) !=
       this->censored_phrases.end())) {
    return false;
  }

  this->censored_phrases.insert(this->censored_phrases.begin(), phrase);

  return true;
}

bool Scrubber::RemoveCensoredPhrase(const char* phrase) {
  if (!(std::find(this->censored_phrases.begin(), this->censored_phrases.end(), phrase) !=
        this->censored_phrases.end())) {
    return false;
  }

  size_t i;
  for (i = 0; i < this->censored_phrases.size(); i++) {
    if (strcmp(this->censored_phrases.at(i).c_str(), phrase) == 0) {
      this->censored_phrases.erase(this->censored_phrases.begin() + i);
      return true;
    }
  }

  return false;
}