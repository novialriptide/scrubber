# scrubber

A tool that allows live-streamers to censor out keywords to make their stream more friendly to younger audiences.

We encourage our contributors to follow the [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## Dependencies

```
SDL              ==  2.28.5
imgui            ==  1.90
opencv           ==  4.8.1
opencv_contrib   ==  4.8.1
```

## Build Instructions

### macOS

1. Clone this repository (`git clone --recursive https://github.com/novialriptide/scrubber.git`)
2. Install [HomeBrew](https://brew.sh/)
3. Install SDL2 (`brew install sdl2`)
4. Install clang-format (`brew install clang-format`)
5. Run `make`

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

All contributions are welcome! Before you contribute, make sure you create an issue on our issue tracker before actually working on it. We would need to approve the idea/bug fix before it's actually worked on.

- Run `clang-format -i *.cpp` to format your code
