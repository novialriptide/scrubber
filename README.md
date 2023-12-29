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
2. Install the [HomeBrew](https://brew.sh/) package manager
3. Install SDL2 (`brew install sdl2`)
4. Install clang-format (`brew install clang-format`)
5. Run `make`

## How to Contribute

All contributions are welcome! Before you contribute, make sure you create an issue on our issue tracker before actually working on it. We would need to approve the idea/bug fix before it's actually worked on.

- Run `clang-format -i *.cpp` to format your code
