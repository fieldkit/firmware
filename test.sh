#!/bin/bash

set -xe

make

rm -f blink.fkb

# arm-none-eabi-readelf --segments build/m0-fk/blink/blink-pic.elf
arm-none-eabi-readelf --sections build/m0-fk/shim/shim.elf
arm-none-eabi-objcopy --dump-section .text=shim.text build/m0-fk/shim/shim.elf
# arm-none-eabi-objcopy --set-section-flags .symtab=load,alloc --dump-section .symtab=shim.symtab build/m0-fk/shim/shim.elf

arm-none-eabi-objcopy \
    --debugging \
    --add-section .text.shim=shim.text \
    --set-section-flags .text.shim=code,contents,alloc,load,readonly \
    --change-section-lma .text.shim=0x8000 \
    --change-section-vma .text.shim=0x8000 \
    --change-section-lma .text=0x9000 \
    --change-section-vma .text=0x9000 \
    build/m0-fk/blink/blink-pic.fkb \
    blink.fkb


# tools/mkfirmware.py --elf build/m0-fk/blink/blink-pic.elf --fkb blink.fkb --shim build/m0-fk/shim/libshim.so

arm-none-eabi-readelf --segments blink.fkb
arm-none-eabi-readelf --sections blink.fkb
