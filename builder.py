"""
As opposed to having a CMake or make file, we have decided to contain all of our
tools to help ourselves develop this program in one Python file.

To install all of the dependencies, run `python3 builder.py install`.
To compile this program to an executable, run `python3 builder.py build`.

It's that simple!
"""
__authors__ = "Andrew Hong and Patrick Cao"
__license__ = "GNU GENERAL PUBLIC LICENSE"

from typing import List, Union
from datetime import datetime
import argparse
import os
import platform
import requests
import zipfile
import shutil


class Log:
    @staticmethod
    def prefix() -> str:
        now = datetime.now()
        return now.strftime("%H:%M:%S | ")

    @staticmethod
    def msg(text: str) -> None:
        print(f"{Log.prefix()}{text}")

    @staticmethod
    def input(text: str, valid_inputs: List[str]) -> str:
        display_inputs = "/".join(valid_inputs)
        user_input = input(f"{Log.prefix()}{text} [{display_inputs}]")
        return user_input


class Builder:
    IMGUI_VERSION = "1.90"
    SDL_VERSION = "2.28.5"

    BIN_DIR = "bin"

    def __init__(self) -> None:
        self._operating_system = platform.system()
        self._platform = platform.platform()

    @property
    def _is_brew_installed(self) -> bool:
        result = os.system("which brew")
        return result != "brew not found"

    def _download_file(self, url: str, filename: str) -> Union[None, str]:
        response = requests.get(url, stream=True)

        out_path = f"{self.BIN_DIR}/{filename}"
        if response.status_code == 200:
            with open(out_path, "wb") as f:
                f.write(response.raw.read())

            return out_path
        return None

    def _extract_sdl_dll_file(
        self, url: str, filename: str, dll_path: str
    ) -> Union[None, str]:
        out_path = self._download_file(url, filename)
        with zipfile.ZipFile(out_path, "r") as zip_ref:
            zip_ref.extractall(self.BIN_DIR)

            out_file = zip_ref.namelist()[0]
            os.replace(
                f"{self.BIN_DIR}/{out_file}/i686-w64-mingw32/bin/{dll_path}.dll",
                f"{self.BIN_DIR}/{dll_path}.dll",
            )
            os.replace(
                f"{self.BIN_DIR}/{out_file}/i686-w64-mingw32/include/{dll_path}",
                f"include/{dll_path}",
            )

            zip_ref.close()

        os.remove(f"{out_path}")
        shutil.rmtree(f"{self.BIN_DIR}/{out_file}")

    def _download_imgui(self) -> None:
        out_path = self._download_file(
            f"https://github.com/ocornut/imgui/archive/refs/tags/v{self.IMGUI_VERSION}.zip",
            f"imgui-{self.IMGUI_VERSION}",
        )
        with zipfile.ZipFile(out_path, "r") as zip_ref:
            zip_ref.extractall("include")

        os.replace(f"include/imgui-{self.IMGUI_VERSION}", "include/imgui")

    def _clear_directories(self) -> None:
        if os.path.exists("bin"):
            shutil.rmtree("bin")

        if os.path.exists("include"):
            shutil.rmtree("include")

        os.mkdir("bin")
        os.mkdir("include")

    def install_deps(self) -> None:
        self._clear_directories()

        self._extract_sdl_dll_file(
            f"https://github.com/libsdl-org/SDL/releases/download/release-{self.SDL_VERSION}/SDL2-devel-{self.SDL_VERSION}-mingw.zip",
            f"sdl-{self.SDL_VERSION}.zip",
            "SDL2",
        )
        self._download_imgui()

        os.replace(
            "include/imgui/backends/imgui_impl_sdl2.cpp",
            "include/imgui/imgui_impl_sdl2.cpp",
        )
        os.replace(
            "include/imgui/backends/imgui_impl_sdlrenderer2.cpp",
            "include/imgui/imgui_impl_sdlrenderer2.cpp",
        )

        os.replace(
            "include/imgui/backends/imgui_impl_sdl2.h",
            "include/imgui/imgui_impl_sdl2.h",
        )
        os.replace(
            "include/imgui/backends/imgui_impl_sdlrenderer2.h",
            "include/imgui/imgui_impl_sdlrenderer2.h",
        )

    def compile(self) -> None:
        files_to_compile = " ".join(
            [
                "app/main.cpp",
                "include/imgui/*.cpp",
            ]
        )

        os.system(f"g++ {files_to_compile} -Iinclude/imgui -Iinclude/SDL2 -std=c++11")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers(dest="sub_command")
    install_parser = subparsers.add_parser(
        "install", help="Install all C++ dependencies"
    )
    build_parser = subparsers.add_parser(
        "build", help="Compile the program into an executable"
    )

    args = parser.parse_args()
    builder = Builder()

    match args.sub_command:
        case "install":
            Log.msg("Installing dependencies")
            builder.install_deps()
            Log.msg("Finished installing dependencies!")
        case "build":
            Log.msg("Compiling...")
            builder.compile()
            Log.msg("Done!")
