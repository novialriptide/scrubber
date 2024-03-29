#include "imgui.h"
#include "imfilebrowser.h"

#include <opencv2/opencv.hpp>

#include <tesseract/baseapi.h>

struct IntVector2 {
  int x;
  int y;
};

struct FoundText {
  cv::Rect boundingBox;
  char* text;
};

class Scrubber {
 public:
  /// @brief A list of censored phrases
  std::vector<std::string> censored_phrases;
  bool display_modifiers;
  char* new_phrase;

  IntVector2 preview_image_size;
  const char* preview_image_path;

  char* kSavePath;

  /// @brief constructor
  Scrubber();

  /// @brief Sets the color theme
  void StyleColorsScrubber();

  /// @brief Starts up the scrubber window
  void Init();

  void LoadImage(const char* file);

  /// @brief Starts the application
  void Run();

  /// @brief Add a phrase to censor. Prevents duplicates.
  /// @param phrase The phrase you want to censor.
  /// @return true if successful
  bool AddCensoredPhrase(const char* phrase);

  /// @brief Uncensors a phrase.
  /// @param phrase The phrase you want to uncensor.
  /// @return true if successful
  bool RemoveCensoredPhrase(const char* phrase);

  FoundText* GetDetectedText();

 private:
  tesseract::TessBaseAPI ocr;

  SDL_Window* window;
  SDL_Renderer* renderer;

  SDL_Texture* preview_texture;

  ImGui::FileBrowser fileDialog;

  void CreateSaveDirectory();

  void CreateSaveFiles();

  void LoadConfig();

  /// @brief Saves config into a JSON file.
  /// @return true if successful
  bool SaveConfig();
};