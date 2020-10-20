#
#

python

import subprocess

class FkSegger(gdb.Command):
  "Segger mode."
  def __init__ (self):
    super(FkSegger, self).__init__("js", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    if arg is None or len(arg) == 0:
      print("Pass JLink port: js 2331")
      return
    gdb.execute("target extended-remote :" + arg)
    gdb.execute("monitor exec SetRTTSearchRanges 0x20000000 64")
    gdb.execute("load")
    gdb.execute("b Dummy_Handler")
    gdb.execute("b fk_assert")
    gdb.execute("b HardFault_Handler")
    gdb.execute("b NonMaskableInt_Handler")
    gdb.execute("b NMI_Handler")
    gdb.execute("b HardFault_Handler")
    gdb.execute("b MemManage_Handler")
    gdb.execute("b BusFault_Handler")
    gdb.execute("b WDT_Handler")
    if False:
      for h in irq_handlers:
        gdb.execute("b " + h)
    gdb.execute("monitor reset")

class FkRestart(gdb.Command):
    "Restart."
    def __init__ (self):
      super(FkRestart, self).__init__("rs", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

    def invoke(self, arg, from_tty):
      gdb.execute("monitor reset")

class FkReloadAllAndRun(gdb.Command):
  "Reload all and run."
  def __init__ (self):
    super(FkReloadAllAndRun, self).__init__("jrar", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    made = subprocess.run(["make", "samd09", "-j4"], cwd="../../../")
    if made.returncode != 0:
      return False
    gdb.execute("load")
    gdb.execute("monitor reset")

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
python FkReloadAllAndRun()
