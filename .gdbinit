#
#

python

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

target extended-remote :2331
load
monitor reset
continue
