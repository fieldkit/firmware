#include <atmel_start.h>

__int32_t main() {
    system_init();

    volatile uint32_t i = 0;
    while (1) {
        i++;
    }

    return 0;
}
