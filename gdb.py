#!/usr/bin/python3

import subprocess
import dataclasses
import os


@dataclasses.dataclass
class Env:
    target: str


env = Env("samd51-fkuw") if os.getenv("FKUW") else Env("samd51")

print(f"Env: {env}")


class FkSegger(gdb.Command):
    "Segger mode."

    def __init__(self):
        super(FkSegger, self).__init__(
            "js", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        if arg is None or len(arg) == 0:
            print("Pass JLink port: js 2331")
            return
        if True:
            gdb.execute(
                f"add-symbol-file build/{env.target}/bootloader/fkbl.elf 0x0000"
            )
        gdb.execute("target extended-remote :" + arg)
        gdb.execute("monitor exec SetRTTSearchRanges 0x20000000 64")
        if True:
            gdb.execute("b osi_hard_fault_report")
            gdb.execute("b fk_debugger_break")
            gdb.execute("b fk_assert")
        if False:
            for h in irq_handlers:
                gdb.execute("b " + h)
        gdb.execute("monitor reset")
        # NOTE GDB is crashing with this?!
        if False:
            gdb.execute("continue")


class FkFixLogs(gdb.Command):
    "Fix logs."

    def __init__(self):
        super(FkFixLogs, self).__init__(
            "jfl", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        gdb.execute("monitor exec SetRTTSearchRanges 0x20000000 64")


class FkRunHosted(gdb.Command):
    "Run hosted tests."

    def __init__(self):
        super(FkRunHosted, self).__init__(
            "jrh", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        made = subprocess.run(["make", "amd64", "-j4"])
        if made.returncode != 0:
            return False
        gdb.execute("run")


class FkRestart(gdb.Command):
    "Restart."

    def __init__(self):
        super(FkRestart, self).__init__(
            "rs", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        gdb.execute("monitor reset")
        # NOTE GDB is crashing with this?!
        if False:
            gdb.execute("continue")


class FkReloadAllAndRun(gdb.Command):
    "Reload all."

    def __init__(self):
        super(FkReloadAllAndRun, self).__init__(
            "jrar", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        made = subprocess.run(["make", "fw", "-j4", env.target])
        if made.returncode != 0:
            return False
        gdb.execute(f"load build/{env.target}/bootloader/fkbl-fkb.elf")
        gdb.execute(f"load build/{env.target}/fk/fk-bundled-fkb.elf")
        gdb.execute(f"monitor reset")
        # NOTE GDB is crashing with this?!
        if False:
            gdb.execute("continue")


class FkReloadAll(gdb.Command):
    "Reload all."

    def __init__(self):
        super(FkReloadAll, self).__init__(
            "jra", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True
        )

    def invoke(self, arg, from_tty):
        made = subprocess.run(["make", "fw", "-j4"])
        if made.returncode != 0:
            return False
        gdb.execute(f"load build/{env.target}/bootloader/fkbl-fkb.elf")
        gdb.execute(f"load build/{env.target}/fk/fk-bundled-fkb.elf")
        gdb.execute(f"monitor reset")


# FkReloadAll()
FkReloadAllAndRun()
FkSegger()
# FkRunHosted()
# FkRestart()
# FkFixLogs()
