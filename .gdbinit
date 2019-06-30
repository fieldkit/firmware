#
#

python

class FkSegger(gdb.Command):
  "Segger mode."
  def __init__ (self):
    super(FkSegger, self).__init__("js", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    gdb.execute("target extended-remote :2331")
    gdb.execute("load")
    gdb.execute("b Dummy_Handler")
    gdb.execute("b osi_assert")
    gdb.execute("b osi_hard_fault_report")
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

class FkReloadAll(gdb.Command):
  "Reload all."
  def __init__ (self):
    super(FkReloadAll, self).__init__("jra", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

  def invoke(self, arg, from_tty):
    gdb.execute("load build/samd51/bootloader/fkbl.elf")
    gdb.execute("load build/samd51/fk/fk-bundled-fkb.elf")
    gdb.execute("monitor reset")

end

python FkReloadAll()
python FkSegger()
python FkRunHosted()

