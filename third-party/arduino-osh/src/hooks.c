/**
 * This software is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this source code. If not, see <http://www.gnu.org/licenses/>.
 */
#include "os.h"
#include "internal.h"
#include "utilities.h"
#include "hooks.h"

#if !defined(OS_CONFIG_HOOKS_CUSTOM)

#if defined(__SAMD21__) || defined(__SAMD51__)

extern void NVIC_SystemReset(void);

void osi_assert(const char *assertion, const char *file, int line) {
    osi_printf("\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    osi_panic(OS_PANIC_ASSERTION);
}

#endif // defined(__SAMD21__) || defined(__SAMD51__)

void osi_debug_dump(os_panic_kind_t code) {
}

void osi_panic(os_panic_kind_t code) {
    osi_printf("\n\npanic! (%s)\n", os_panic_kind_str(code));

    osi_printf("\nrunning:\n");
    for (os_task_t *iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
        osi_printf("  '%s' status(%s) (0x%x)\n", iter->name, os_task_status_str(iter->status), iter->priority);
    }

    osi_printf("\nwaiting:\n");
    for (os_task_t *iter = osg.waitqueue; iter != NULL; iter = iter->nrp) {
        osi_printf("  '%s' status(%s) (0x%x)\n", iter->name, os_task_status_str(iter->status), iter->priority);
    }

#if defined(__SAMD21__) || defined(__SAMD51__)
    NVIC_SystemReset();
#endif // defined(__SAMD21__) || defined(__SAMD51__)

    if (osg.running != NULL) {
        osi_task_status_set((os_task_t *)osg.running, OS_TASK_STATUS_PANIC);
    }
}

void osi_hard_fault_report(uintptr_t *stack, uint32_t lr, cortex_hard_fault_t *hfr) {
#if defined(__SAMD21__) || defined(__SAMD51__)
    NVIC_SystemReset();
#endif // defined(__SAMD21__) || defined(__SAMD51__)
}

#endif // !defined(OS_CONFIG_HOOKS_CUSTOM)
