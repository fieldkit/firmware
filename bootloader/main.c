/**
 *
 *
 */
#include <hal_gpio.h>
#include <memory.h>

#include "bl.h"

#define FK_BUTTON_RIGHT            GPIO(GPIO_PORTA, 21)
#define FK_FAILSAFE_HOLD_TIME      1000 * 10

extern enum reset_reason _get_reset_reason(void);

extern uint32_t __cm_app_vectors_ptr;

uint32_t serial_number_get(uint32_t *sn) {
    sn[0] = __builtin_bswap32(*(uint32_t *)0x008061FC);
    sn[1] = __builtin_bswap32(*(uint32_t *)0x00806010);
    sn[2] = __builtin_bswap32(*(uint32_t *)0x00806014);
    sn[3] = __builtin_bswap32(*(uint32_t *)0x00806018);
    return 128;
}

uint32_t launch() {
    fkb_external_println("bl: looking for executable...");

    /* Look for FKB headers... */
    return fkb_find_and_launch((void *)&__cm_app_vectors_ptr);
}

int32_t bl_upgrade_firmware_necessary(fkb_header_t *running, fkb_header_t *upgrade, fkb_header_t *failsafe) {
    if (!fkb_has_valid_signature(upgrade)) {
        return 0;
    }

    if (fkb_has_valid_signature(running)) {
        bl_fkb_log_header(running);

        // If we're running the upgraded firmware then all is fine. This is the most common situation.
        if (running->firmware.timestamp == upgrade->firmware.timestamp) {
            fkb_external_println("bl: [0x%08" PRIx32 "] running same (%" PRIu32 " == %" PRIu32 ")",
                                 (uint32_t)upgrade, running->firmware.timestamp, upgrade->firmware.timestamp);
            return 0;
        }

        // If running firmware is older then ignore the upgraded firmware.
        if (running->firmware.timestamp > upgrade->firmware.timestamp) {
            fkb_external_println("bl: [0x%08" PRIx32 "] running newer (%" PRIu32 " >= %" PRIu32 ")",
                                 (uint32_t)upgrade, running->firmware.timestamp, upgrade->firmware.timestamp);
            return 0;
        }

        // Never automatically upgrade to previously running firmware.
        if (running->firmware.previous > 0 && running->firmware.previous != UINT32_MAX &&
            running->firmware.previous <= upgrade->firmware.timestamp) {
            fkb_external_println("bl: [0x%08" PRIx32 "] requiring newer (%" PRIu32 " > %" PRIu32 ")",
                                 (uint32_t)upgrade, running->firmware.previous, upgrade->firmware.timestamp);
            return 0;
        }
    }

    // We're going to upgrade!

    fkb_external_println("bl: upgrading");

    bl_fkb_log_header(upgrade);

    return 1;
}

int32_t bl_upgrade_firmware(fkb_header_t *running, fkb_header_t *fkbh, uint32_t address) {
    if (!fkb_has_valid_signature(fkbh)) {
        return -1;
    }

    fkb_external_println("bl: [0x%08" PRIx32 "] erasing %" PRIu32, address, fkbh->firmware.binary_size);

    if (bl_flash_erase(address, fkbh->firmware.binary_size) < 0) {
        return -1;
    }

    fkb_external_println("bl: [0x%08" PRIx32 "] copying %" PRIu32, address, fkbh->firmware.binary_size);

    uint8_t *source = (uint8_t *)fkbh;
    int32_t remaining = fkbh->firmware.binary_size;

    // Customize the header by including the firmware timestamp of the
    // firmware that performed the upgrade. This is very handy, and
    // the downside is that the hash is no longer valid.
    fkb_header_t header = *fkbh;

    header.firmware.previous = running->firmware.timestamp;

    if (bl_flash_write(address, (uint8_t *)&header, sizeof(fkb_header_t)) < 0) {
        return -1;
    }

    remaining -= sizeof(fkb_header_t);
    source += sizeof(fkb_header_t);
    address += sizeof(fkb_header_t);

    while (remaining > 0) {
        int32_t ncopy = bl_flash_page_size <= remaining ? bl_flash_page_size : remaining;
        if (bl_flash_write(address, source, ncopy) < 0) {
            return -1;
        }

        remaining -= ncopy;
        source += ncopy;
        address += ncopy;
    }

    fkb_external_println("bl: [0x%08" PRIx32 "] copied %" PRIu32, address, fkbh->firmware.binary_size);

    return 0;
}

int32_t bl_button_initialize() {
    gpio_set_pin_direction(FK_BUTTON_RIGHT, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(FK_BUTTON_RIGHT, GPIO_PULL_UP);

    return 0;
}

int32_t bl_button_pressed() {
    return gpio_get_pin_level(FK_BUTTON_RIGHT) == 0;
}

int32_t bl_firmware_upgrade_failed() {
    fkb_external_println("bl: upgrade failed!");

    // NOTE Really bad! Just really really bad. This likely
    // means there's no firmware ahead of us anymore.
    // TODO: Try again?
    // TODO: Swap banks and do so in a way that won't cause
    // the other bootloader to just have the same mistake? We
    // won't always be able to swap banks.
    delay(1000);
    NVIC_SystemReset();
    return 0;
}

int32_t main() {
    memory_initialize();

    SEGGER_RTT_Init();

    fkb_external_println(RTT_CTRL_RESET "");
    fkb_external_println("bl: starting!");

    board_initialize();

    SysTick_Config(F_CPU / 1000);

    uint32_t sn[4];
    serial_number_get(sn);
    fkb_external_println("bl: serial='%08" PRIx32 "-%08" PRIx32 "-%08" PRIx32 "-%08" PRIx32 "'",
                         (uint32_t)__builtin_bswap32(sn[0]), (uint32_t)__builtin_bswap32(sn[1]),
                         (uint32_t)__builtin_bswap32(sn[2]), (uint32_t)__builtin_bswap32(sn[3]));

    fkb_external_println("bl: board ready (%s) (bank = %d)", fk_get_reset_reason_string(), fk_nvm_get_active_bank());

    fkb_external_println("bl: flash...");

    bl_flash_initialize();

    fkb_external_println("bl: qspi...");

    bl_qspi_initialize();

    bl_button_initialize();

    int32_t failsafe = false;

    while (bl_button_pressed()) {
        if (millis() > FK_FAILSAFE_HOLD_TIME) {
            if (!failsafe) {
                fkb_external_println("bl: failsafe!");
            }
            failsafe = true;
        }
    }

    fkb_header_t *running = (fkb_header_t *)(uint32_t *)FK_MEMORY_FLASH_ADDRESS_RUNNING_CORE;
    fkb_header_t *upgrade_firmware = (fkb_header_t *)(uint32_t *)FK_MEMORY_QSPI_ADDRESS_UPGRADE_CORE;
    fkb_header_t *failsafe_firmware = (fkb_header_t *)(uint32_t *)FK_MEMORY_QSPI_ADDRESS_FAILSAFE_CORE;

    if (failsafe) {
        if (bl_upgrade_firmware(running, failsafe_firmware, FK_MEMORY_FLASH_ADDRESS_RUNNING_CORE) < 0) {
            bl_firmware_upgrade_failed();
        }
    }
    else {
        fkb_external_println("bl: normal");
    }

    if (bl_upgrade_firmware_necessary(running, upgrade_firmware, failsafe_firmware)) {
        if (bl_upgrade_firmware(running, upgrade_firmware, FK_MEMORY_FLASH_ADDRESS_RUNNING_CORE) < 0) {
            bl_firmware_upgrade_failed();
        }

        fkb_external_println("bl: upgraded!");

        bl_fkb_log_header(running);
    }

    launch();

    /* If we're here then no launching occurred! */
    fkb_external_println("bl: delay before trying again.");

    while (millis() < 60 * 1000 * 2) {
        delay(1000);
    }

    fkb_external_println("bl: hup!");

    delay(1000);

    NVIC_SystemReset();

    return 0;
}

void cm_dummy_handler() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

void cm_nmi() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

void cm_hard_fault() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

void cm_svc() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

void cm_pendsv() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

volatile uint32_t system_ticks = 0;

void cm_systick() {
    system_ticks++;
}

uint32_t millis() {
    return system_ticks;
}

void delay(uint32_t ms) {
    uint32_t end = system_ticks + ms;
    while (system_ticks < end) {
        /* yield */
    }
}

extern uint32_t __cm_stack_top;

// WARNING: DO NOT CHANGE THE SIZE OF THIS.
__attribute__((section(".isr_vector")))
const struct cm_vector_table_t vector_table = {
    .stack               = (void *)(&__cm_stack_top),
    .reset_handler       = (void *)main,
    .nmi_handler         = (void *)cm_nmi,
    .hard_fault_handler  = (void *)cm_hard_fault,
    .reserved_m12        = (void *)(0UL),
    .reserved_m11        = (void *)(0UL),
    .reserved_m10        = (void *)(0UL),
    .reserved_m9         = (void *)(0UL),
    .reserved_m8         = (void *)(0UL),
    .reserved_m7         = (void *)(0UL),
    .reserved_m6         = (void *)(0UL),
    .svc_handler         = (void *)cm_svc,
    .reserved_m4         = (void *)(0UL),
    .reserved_m3         = (void *)(0UL),
    .pendsv_handler      = (void *)cm_pendsv,
    .systick_handler     = (void *)cm_systick,
};

#define FK_XSTR(s) FK_STR(s)

#define FK_STR(s)  #s

__attribute__((section(".fkb.header")))
const struct fkb_header_t fkb_header = {
    .signature          = FKB_HEADER_SIGNATURE(),
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 0,
        .number         = 0,
        .reserved       = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
        .safe           = 0xff,
        .previous       = UINT32_MAX,
        .binary_size    = 0,
        .tables_offset  = 0,
        .data_size      = 0,
        .bss_size       = 0,
        .got_size       = 0,
        .vtor_offset    = 0,
        .got_offset     = 0,
        .version        = FK_XSTR(FK_VERSION),
        .hash_size      = 0,
        .hash           = { 0 }
    },
    .number_symbols     = 0,
    .number_relocations = 0
};
