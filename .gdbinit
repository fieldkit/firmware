add-symbol-file build/samd51/bootloader/fkbl.elf 0x0000
add-symbol-file build/samd51/fk/fk-bundled.elf 0x4000

target extended-remote :2331
load
b cm_shim_hard_fault
b cm_shim_nmi
b cm_shim_pendsv
b cm_shim_svc
b try_launch
b invoke_pic
monitor reset
continue
