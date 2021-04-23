/**
 *
 *
 */
#include <loading.h>
#include <string.h>
#include <sam.h>
#include <SEGGER_RTT.h>

extern void invoke_pic(uint32_t sp, void *entry, uint32_t got);

extern void delay(uint32_t ms);

extern uint32_t __cm_ram_origin;
extern uint32_t __heap_top;
extern uint32_t __data_start__;

fkb_header_t const *fkb_try_header(void const *ptr) {
    if (!fkb_has_valid_signature(ptr)) {
        return NULL;
    }

    return (fkb_header_t const *)ptr;
}

uint32_t fkb_verify_exec_state(fkb_header_t const *fkbh, fkb_exec_state_t *fkes) {
    uint32_t *vtor = (uint32_t *)((uint8_t *)fkbh + fkbh->firmware.vtor_offset);

    fkes->vtor = vtor;
    fkes->sp = (uint32_t *)*vtor;
    fkes->entry = (uint32_t *)*(vtor + 1);
    fkes->got = (uint32_t *)((uint8_t *)&__cm_ram_origin + fkbh->firmware.got_offset);

    /* Make sure vector table address of app is aligned. */
    if (((uint32_t)(vtor) & ~SCB_VTOR_TBLOFF_Msk) != 0x00) {
        fkb_external_println("bl: [0x%08x] no vector table", fkbh);
        return FKB_EXEC_STATE_BAD_VTOR;
    }

    /* Do nothing if vector pointer is erased. */
    if ((uint32_t)fkes->sp == 0xFFFFFFFF) {
        fkb_external_println("bl: [0x%08x] erased cell", fkbh);
        return FKB_EXEC_STATE_BAD_SP;
    }

    /* Do nothing if SP is invalid. */
    if ((uint32_t)fkes->sp <= (uint32_t)&__cm_ram_origin) {
        fkb_external_println("bl: [0x%08x] invalid SP value (0x%08x)", fkbh, fkes->sp);
        return FKB_EXEC_STATE_BAD_SP;
    }

    return FKB_EXEC_STATE_OK;
}

uint32_t fkb_try_launch(fkb_header_t const *fkbh) {
    fkb_exec_state_t fkes;

    if (fkb_verify_exec_state(fkbh, &fkes) != FKB_EXEC_STATE_OK) {
        return 0;
    }

    /* Ok, so we're doing this! */
    fkb_external_println("bl: [0x%08x] executing (sp=0x%p) (entry=0x%p) (got=0x%x)", fkbh, fkes.sp, fkes.entry, fkes.got);

    SCB->VTOR = ((uint32_t)fkes.vtor & SCB_VTOR_TBLOFF_Msk);

    invoke_pic((uint32_t)fkes.sp, (void *)fkes.entry, (uint32_t)fkes.got);

    return 0;
}

uint32_t fkb_find_and_launch(void const *ptr) {
    fkb_header_t const *selected = NULL;

    while (1) {
        fkb_external_println("bl: [0x%08p] checking for header", ptr);

        fkb_header_t const *fkbh = fkb_try_header(ptr);
        if (fkbh == NULL) {
            break;
        }

        selected = fkbh;

        fkb_external_println("bl: [0x%08p] found '%s' (#%lu) flags=0x%x timestamp=%lu previous=%lu binary-size=%lu dyntables=+%lu data=%lu bss=%lu got=%lu vtor=0x%x", ptr,
                             fkbh->firmware.version, fkbh->firmware.number, fkbh->firmware.flags,
                             fkbh->firmware.timestamp, fkbh->firmware.previous,
                             fkbh->firmware.binary_size, fkbh->firmware.tables_offset,
                             fkbh->firmware.data_size, fkbh->firmware.bss_size, fkbh->firmware.got_size,
                             fkbh->firmware.vtor_offset);

        char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
        fkb_bytes_to_hex(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

        fkb_external_println("bl: [0x%08p] hash='%s' timestamp=%lu", ptr,
                             hex_hash, fkbh->firmware.timestamp);

        ptr += fkb_aligned_on(fkbh->firmware.binary_size, 0x1000);

        break;
    }

    if (selected == NULL) {
        return 0;
    }

    return fkb_try_launch(selected);
}
