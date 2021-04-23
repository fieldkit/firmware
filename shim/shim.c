/**
 *
 *
 *
 */
#include <loading.h>
#include <cortex.h>

__attribute__((section(".shim_text")))
uint32_t shim_main() {
    fkb_header_t *fkb = NULL;

    volatile uint32_t i = 0;
    while (1) {
        i++;
    }

    // __set_MSP( (uint32_t)(__sketch_vectors_ptr) );
    // SCB->VTOR = ((uint32_t)(&__sketch_vectors_ptr) & SCB_VTOR_TBLOFF_Msk);
    // asm("bx %0"::"r"(*pulSketch_Start_Address));

    return 0;
}

__attribute__((section(".shim_text")))
void cm_dummy_handler() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

__attribute__((section(".shim_text")))
void cm_shim_nmi() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

__attribute__((section(".shim_text")))
void cm_shim_hard_fault() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

__attribute__((section(".shim_text")))
void cm_shim_svc() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

__attribute__((section(".shim_text")))
void cm_shim_pendsv() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
}

__attribute__((section(".shim_text")))
void cm_shim_systick() {
}

extern uint32_t __cm_shim_stack_top;

__attribute__((section(".shim_isr_vector")))
const struct cm_vector_table_t vector_table = {
  .stack               = (void *)(&__cm_shim_stack_top),
  .reset_handler       = (void *)shim_main,
  .nmi_handler         = (void *)cm_shim_nmi,
  .hard_fault_handler  = (void *)cm_shim_hard_fault,
  .reserved_m12        = (void *)(0UL),
  .reserved_m11        = (void *)(0UL),
  .reserved_m10        = (void *)(0UL),
  .reserved_m9         = (void *)(0UL),
  .reserved_m8         = (void *)(0UL),
  .reserved_m7         = (void *)(0UL),
  .reserved_m6         = (void *)(0UL),
  .svc_handler         = (void *)cm_shim_svc,
  .reserved_m4         = (void *)(0UL),
  .reserved_m3         = (void *)(0UL),
  .pendsv_handler      = (void *)cm_shim_pendsv,
  .systick_handler     = (void *)cm_shim_systick,
};

uint32_t main() {
    volatile uint32_t i = 0;
    while (1) {
        i++;
    }
    return 0;
}
