/**
 *
 *
 */
#include <string.h>
#include <loading.h>
#include <cortex.h>

extern void memory_initialize(void);

extern void board_minimal_initialize(void);

extern void board_initialize();

void delay(uint32_t ms);

extern uint32_t __cm_app_vectors_ptr;

extern void invoke_pic(void *entry);

void delay(uint32_t ms);

typedef struct fkb_found_t {
    void *ptr;
} fkb_found_t;

/*
static uint32_t aligned_on(uint32_t value, uint32_t on) {
    return ((value % on != 0) ? (value + (on - (value % on))) : value);
}
*/

static uint32_t bytes_to_hex(char *buffer, size_t buffer_length, uint8_t *ptr, size_t size) {
    // ASSERT(buffer_length > (size * 2));

    for (size_t i = 0; i < size; ++i) {
        buffer[i * 2    ] = "0123456789abcdef"[ptr[i] >> 4];
        buffer[i * 2 + 1] = "0123456789abcdef"[ptr[i] & 0x0F];
    }

    buffer[size * 2] = 0;

    return 0;
}

uint32_t try_launch(uint32_t *base) {
    /* Make sure vector table address of app is aligned. */
    if (((uint32_t)(base) & ~SCB_VTOR_TBLOFF_Msk) != 0x00) {
        debug_println("bl: [0x%08x] no vector table", base);
        return 0;
    }

    /* Do nothing if SP is invalid. */
    if (*base <= 0x20000000) {
        debug_println("bl: [0x%08x] invalid SP value (0x%08x)", base, *base);
        return 0;
    }

    /* Do nothing if vector pointer is erased. */
    if (*base == 0xFFFFFFFF) {
        debug_println("bl: [0x%08x] erased cell", base);
        return 0;
    }

    /* Get entry address, skip over initial SP. */
    uint32_t *entry_function = (uint32_t *)base + 1;

    if (0) {
        debug_println("bl: [0x%08x] execution disabled (entry=0x%p)", base, entry_function);
        return 1;
    }

    /* Ok, so we're doing this! */
    debug_println("bl: [0x%08x] executing (entry=0x%p)", base, entry_function);

    delay(500);

    __set_MSP((uint32_t)(*base));

    SCB->VTOR = ((uint32_t)(base) & SCB_VTOR_TBLOFF_Msk);

    invoke_pic((void *)*entry_function);

    return 0;
}

uint32_t fkb_check_find(void *ptr, fkb_found_t *fkbf) {
    fkb_header_t *fkbh = (fkb_header_t *)ptr;

    fkbf->ptr = NULL;

    debug_println("bl: [0x%08p] checking for fkb", ptr);

    if (strcmp((const char *)fkbh->signature, "FKB") != 0) {
        return 0;
    }

    debug_println("bl: [0x%08p] found ('%s') flags=0x%x size=%lu vtor=0x%x", ptr,
                  fkbh->firmware.name, fkbh->firmware.flags, fkbh->firmware.binary_size,
                  fkbh->firmware.vtor_offset);

    char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
    bytes_to_hex(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

    debug_println("bl: [0x%08p] hash='%s' timestamp=%lu", ptr,
                  hex_hash, fkbh->firmware.timestamp);

    /* This will need some future customization. I'm considering also placing
     * the header after the vector table, which is more efficient. */
    fkbf->ptr = (void *)((uint8_t *)ptr) + fkbh->firmware.vtor_offset;

    return try_launch((uint32_t *)fkbf->ptr);
}

uint32_t launch() {
    debug_println("bl: [0x%08x] looking for executable", &__cm_app_vectors_ptr);

    /* Look for FKB headers... */
    fkb_found_t fkbf;

    if (fkb_check_find((void *)&__cm_app_vectors_ptr, &fkbf)) {
        return 0;
    }

    /* Fall back on a regular old firmware launch */
    return try_launch(&__cm_app_vectors_ptr);
}

int32_t main() {
    memory_initialize();

    SEGGER_RTT_Init();

    debug_println("");
    debug_println("bl: starting!");

    // SysTick_Config(F_CPU / 1000);

    board_minimal_initialize();

    debug_println("bl: board ready");

    launch();

    /* If we're here then no launching occurred! */

    debug_println("bl: delay before trying again.");

    while (1) {
        delay(100);
    }

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

void delay(uint32_t ms) {
    uint32_t end = system_ticks + ms;
    while (system_ticks < end) {
        /* yield */
    }
}

extern uint32_t __cm_stack_top;

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
