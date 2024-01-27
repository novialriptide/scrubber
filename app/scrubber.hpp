#include "imgui.h"
#include "imfilebrowser.h"

struct IntVector2 {
  int x;
  int y;
};

class Scrubber {
 public:
  // char* censored_words[];
  bool display_modifiers;

  IntVector2 preview_image_size;

  /// @brief constructor
  Scrubber();

  /// @brief Sets the color theme
  void StyleColorsScrubber();

  /// @brief Starts up the scrubber window
  void Init();

  void LoadImage(const char* file);

  /// @brief Starts the application
  void Run();

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Texture* preview_texture;

  ImGui::FileBrowser fileDialog;
};