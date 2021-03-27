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
#ifndef LDING_CORTEX_H
#define LDING_CORTEX_H

#include <sam.h>

typedef struct cm_vector_table_t {
    void *stack;
    void *reset_handler;
    void *nmi_handler;
    void *hard_fault_handler;
    void *reserved_m12;
    void *reserved_m11;
    void *reserved_m10;
    void *reserved_m9;
    void *reserved_m8;
    void *reserved_m7;
    void *reserved_m6;
    void *svc_handler;
    void *reserved_m4;
    void *reserved_m3;
    void *pendsv_handler;
    void *systick_handler;
} cm_vector_table_t;

#endif // LDING_CORTEX_H
