__authors__ = "Andrew Hong and Patrick Cao"
__license__ = "GNU GENERAL PUBLIC LICENSE"

from typing import List
from datetime import datetime
import sys
import platform


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
    def __init__(self) -> None:
        self._operating_system = platform.system()
        self._platform = platform.platform()

    def run(self) -> None:
        print("lmao")
        if self._operating_system == "Darwin":  # macOS
            pass

        elif self._operating_system == "Windows":
            pass

        else:
            raise Exception("Your operating system is not supported.")


Builder().run()
