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
#include "sysexample.h"
#include "syscall_plumbing.h"

uint32_t svc_example(void) {
    osi_printf("svc_example\n");
    return 0;
}

SVC_0_1(svc_example, uint32_t, RET_uint32_t);

uint32_t os_example() {
    if (osi_in_task()) {
        return __svc_example();
    }
    else {
        return svc_example();
    }
}
