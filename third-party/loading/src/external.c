#include <SEGGER_RTT.h>
#include <string.h>

#include <loading.h>

#if defined(__SAMD51__)

uint32_t fkb_external_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
    va_end(args);
    return r;
}

uint32_t fkb_external_println(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
    SEGGER_RTT_WriteString(0, "\n");
    va_end(args);
    return r;
}

uint32_t fkb_external_vprintf(const char *str, va_list args) {
    return (uint32_t)SEGGER_RTT_vprintf(0, str, (va_list *)&args);
}

#else

#include <stdio.h>

uint32_t fkb_external_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)vprintf(str, args);
    va_end(args);
    return r;
}

uint32_t fkb_external_println(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)vprintf(str, args);
    printf("\n");
    va_end(args);
    return r;
}

uint32_t fkb_external_vprintf(const char *str, va_list args) {
    return (uint32_t)vprintf(str, args);
}

#endif

int32_t fkb_same_header(fkb_header_t const *a, fkb_header_t const *b) {
    if (a == NULL || b == NULL) return 0;
    if (a->firmware.hash_size != b->firmware.hash_size) return 0;
    return memcmp(a->firmware.hash, b->firmware.hash, b->firmware.hash_size) == 0;
}

int32_t fkb_bytes_to_hex(char *buffer, size_t buffer_length, uint8_t const *ptr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        buffer[i * 2    ] = "0123456789abcdef"[ptr[i] >> 4];
        buffer[i * 2 + 1] = "0123456789abcdef"[ptr[i] & 0x0F];
    }

    buffer[size * 2] = 0;

    return 0;
}

int32_t fkb_has_valid_signature(void const *ptr) {
    fkb_header_t const *fkbh = (fkb_header_t const *)ptr;
    return strcmp(fkbh->signature, "FKB") == 0;
}

uint32_t fkb_aligned_on(uint32_t value, uint32_t on) {
    return ((value % on != 0) ? (value + (on - (value % on))) : value);
}
