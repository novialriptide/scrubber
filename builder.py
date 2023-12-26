__authors__ = "Andrew Hong and Patrick Cao"
__license__ = "GNU GENERAL PUBLIC LICENSE"

from typing import List, Union
from datetime import datetime
import os
import platform
import requests
import zipfile


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
    SDL_VERSION = "2.28.5"

    def __init__(self) -> None:
        self._operating_system = platform.system()
        self._platform = platform.platform()

    @property
    def _is_brew_installed(self) -> bool:
        result = os.system("which brew")
        return result != "brew not found"

    def _download_tar_file(self, url: str, filename: str) -> Union[None, str]:
        response = requests.get(url, stream=True)

        out_path = f"bin/{filename}"
        if response.status_code == 200:
            with open(out_path, "wb") as f:
                f.write(response.raw.read())

            return out_path
        return None

    def _extract_dll_file(self, url: str, filename: str) -> None:
        out_path = self._download_tar_file(url, filename)
        with zipfile.ZipFile(out_path, "r") as zip_ref:
            zip_ref.extractall("./bin")

    def run(self) -> None:
        self._extract_dll_file(
            f"https://github.com/libsdl-org/SDL/releases/download/release-{self.SDL_VERSION}/SDL2-devel-{self.SDL_VERSION}-mingw.zip",
            f"sdl-{self.SDL_VERSION}.zip",
        )

        if self._operating_system == "Darwin":  # macOS
            Log.msg("macOS has been detected as your operating system.")

        elif self._operating_system == "Windows":
            Log.msg("Windows has been detected as your operating system.")

            pass

        else:
            raise Exception("Your operating system is not supported.")


Builder().run()

"""
https://github.com/ocornut/imgui/
https://github.com/libsdl-org/SDL/
"""
