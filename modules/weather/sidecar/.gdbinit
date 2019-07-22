#
#

python

class FkSegger(gdb.Command):
  "Segger mode."
  def __init__ (self):
    super(FkSegger, self).__init__("js", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    gdb.execute("target extended-remote :4331")
    gdb.execute("load")
    gdb.execute("b HardFault_Handler")
    gdb.execute("b NonMaskableInt_Handler")
    gdb.execute("monitor reset")

class FkRestart(gdb.Command):
    "Restart."
    def __init__ (self):
      super(FkRestart, self).__init__("rs", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

    def invoke(self, arg, from_tty):
      gdb.execute("monitor reset")
      gdb.execute("c")

class FkReloadAll(gdb.Command):
  "Reload all."
  def __init__ (self):
    super(FkReloadAll, self).__init__("jra", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    made = subprocess.run(["make", "samd09", "-j4"], cwd="../../../")
    if made.returncode != 0:
      return False
    gdb.execute("load")
    gdb.execute("monitor reset")

end

python FkReloadAll()
python FkSegger()
python FkRestart()
