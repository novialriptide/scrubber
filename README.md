# scrubber [![macOS](https://github.com/novialriptide/scrubber/actions/workflows/build-from-macos.yml/badge.svg)](https://github.com/novialriptide/scrubber/actions/workflows/build-from-macos.yml) [![Ubuntu](https://github.com/novialriptide/scrubber/actions/workflows/build-from-ubuntu.yml/badge.svg)](https://github.com/novialriptide/scrubber/actions/workflows/build-from-ubuntu.yml)

A C++17 tool that allows live-streamers to censor out keywords to make their stream more friendly to younger audiences.

## Dependencies

```
SDL                ==  2.28.5
SDL_image          ==  2.8.2
imgui              ==  1.90
imgui-filebrowser  ==  7cc76a0
nlohmann/json      ==  3.11.3
opencv             ==  4.8.1
```

## Build Instructions

### macOS

1. Clone this repository (`git clone --recursive https://github.com/novialriptide/scrubber.git`)
2. Install [HomeBrew](https://brew.sh/)
3. Install dependencies (`brew install sdl2 sdl2_image opencv clang-format`)
4. Run `make`

### Ubuntu

1. Clone this repository (`git clone --recursive https://github.com/novialriptide/scrubber.git`)
2. Run `sudo apt-get update`
3. Install dependencies (`sudo apt-get install libsdl2-dev libsdl2-image-dev libopencv-dev`)
4. Run `make`

### Windows 11

1. Clone this repository (`git clone --recursive https://github.com/novialriptide/scrubber.git`)
2. Install [MSYS2](https://www.msys2.org/)
   1. Make sure you set your installation folder to `C:/msys64`
3. Install SDL2 from the MSYS2 terminal (`pacman -S mingw-w64-i686-SDL2`)
4. Install clang-format
   1. Install the latest version of [Python](https://www.python.org/)
   2. Install clang-format (`python -m pip install clang-format`)
5. Run `mingw32-make`

## How to Contribute

All contributions are welcome! Before you contribute, make sure you create an issue on our issue tracker before actually working on it. We would need to approve the idea/bug fix before it's actually worked on. We also encourage our contributors to follow the [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

It is highly recommended to use [Visual Studio Code](https://code.visualstudio.com/), as tools such as intellisense and auto-formatting are setup to enhance the developer experience. Contributions to add support to other IDEs are welcome!

Use `clang-format -i app/*.cpp` to format your code

## Disclaimer

There will be some instances of foul language contained within the source code that could be considered as derogatory or offensive. This is not an attempt to attack anyone, as the language in the repository is necessary to make sure the software would work properly.
