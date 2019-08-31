#
#

python

irq_handlers = [
  "EIC_0_Handler", "EIC_1_Handler", "EIC_2_Handler", "EIC_3_Handler", "EIC_4_Handler",
  # Buttons
  # "EIC_5_Handler", "EIC_6_Handler", "EIC_7_Handler",
  "EIC_8_Handler", "EIC_9_Handler", "EIC_10_Handler",
  # WiFi
  # "EIC_11_Handler"
  "EIC_12_Handler", "EIC_13_Handler", "EIC_14_Handler", "EIC_15_Handler"
]

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
    gdb.execute("b osi_panic")
    gdb.execute("b osi_hard_fault_report")
    gdb.execute("b fk_assert")
    gdb.execute("b __cxa_pure_virtual")
    if False:
      for h in irq_handlers:
        gdb.execute("b " + h)
    gdb.execute("monitor reset")
    gdb.execute("continue")

class FkRunHosted(gdb.Command):
  "Run hosted tests."
  def __init__ (self):
    super(FkRunHosted, self).__init__("jrh", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    made = subprocess.run(["make", "amd64", "-j4"])
    if made.returncode != 0:
      return False
    gdb.execute("run")

class FkRestart(gdb.Command):
    "Restart."
    def __init__ (self):
      super(FkRestart, self).__init__("rs", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

    def invoke(self, arg, from_tty):
      gdb.execute("monitor reset")
      gdb.execute("c")

class FkReloadAllAndRun(gdb.Command):
  "Reload all."
  def __init__ (self):
    super(FkReloadAllAndRun, self).__init__("jrar", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    made = subprocess.run(["make", "fw", "-j4"])
    if made.returncode != 0:
      return False
    gdb.execute("load build/samd51/bootloader/fkbl.elf")
    gdb.execute("load build/samd51/fk/fk-bundled-fkb.elf")
    gdb.execute("monitor reset")
    gdb.execute("c")

class FkReloadAll(gdb.Command):
  "Reload all."
  def __init__ (self):
    super(FkReloadAll, self).__init__("jra", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    made = subprocess.run(["make", "fw", "-j4"])
    if made.returncode != 0:
      return False
    gdb.execute("load build/samd51/bootloader/fkbl.elf")
    gdb.execute("load build/samd51/fk/fk-bundled-fkb.elf")
    gdb.execute("monitor reset")

end

python FkReloadAll()
python FkReloadAllAndRun()
python FkSegger()
python FkRunHosted()
python FkRestart()
