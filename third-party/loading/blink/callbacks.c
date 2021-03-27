#include <SEGGER_RTT.h>

void do_something() {
    SEGGER_RTT_WriteString(0, "Something!\n");
}

void do_another() {
    SEGGER_RTT_WriteString(0, "Another!\n");
}

