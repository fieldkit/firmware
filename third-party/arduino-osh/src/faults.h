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
#ifndef OS_FAULTS_H
#define OS_FAULTS_H

#include <stdint.h>

#define SYSHND_CTRL (*(volatile unsigned int *)(0xE000ED24u))   // System Handler Control and State Register
#define NVIC_MFSR   (*(volatile unsigned char *)(0xE000ED28u))  // Memory Management Fault Status Register
#define NVIC_BFSR   (*(volatile unsigned char *)(0xE000ED29u))  // Bus Fault Status Register
#define NVIC_UFSR   (*(volatile unsigned short *)(0xE000ED2Au)) // Usage Fault Status Register
#define NVIC_HFSR   (*(volatile unsigned int *)(0xE000ED2Cu))   // Hard Fault Status Register
#define NVIC_DFSR   (*(volatile unsigned int *)(0xE000ED30u))   // Debug Fault Status Register
#define NVIC_BFAR   (*(volatile unsigned int *)(0xE000ED38u))   // Bus Fault Manage Address Register
#define NVIC_AFSR   (*(volatile unsigned int *)(0xE000ED3Cu))   // Auxiliary Fault Status Register

typedef struct cortex_hard_fault_t {
    struct {
        volatile void *R0;  // Register R0
        volatile void *R1;  // Register R1
        volatile void *R2;  // Register R2
        volatile void *R3;  // Register R3
        volatile void *R12; // Register R12
        volatile void *LR;  // Link register
        volatile void *PC;  // Program counter
        union {
            volatile uint32_t byte;
            struct {
                uint32_t IPSR : 8;  // Interrupt Program Status register (IPSR)
                uint32_t EPSR : 19; // Execution Program Status register (EPSR)
                uint32_t APSR : 5;  // Application Program Status register (APSR)
            } bits;
        } psr; // Program status register.
    } registers;

    union {
        volatile unsigned int byte;
        struct {
            unsigned int MEMFAULTACT : 1; // Read as 1 if memory management fault is active
            unsigned int BUSFAULTACT : 1; // Read as 1 if bus fault exception is active
            unsigned int UNUSED1 : 1;
            unsigned int USGFAULTACT : 1; // Read as 1 if usage fault exception is active
            unsigned int UNUSED2 : 3;
            unsigned int SVCALLACT : 1;  // Read as 1 if SVC exception is active
            unsigned int MONITORACT : 1; // Read as 1 if debug monitor exception is active
            unsigned int UNUSED3 : 1;
            unsigned int PENDSVACT : 1;      // Read as 1 if PendSV exception is active
            unsigned int SYSTICKACT : 1;     // Read as 1 if SYSTICK exception is active
            unsigned int USGFAULTPENDED : 1; // Usage fault pended; usage fault started but was replaced by a higher-priority exception
            unsigned int MEMFAULTPENDED : 1; // Memory management fault pended; memory management fault started but was replaced by a
                                             // higher-priority exception
            unsigned int
                BUSFAULTPENDED : 1; // Bus fault pended; bus fault handler was started but was replaced by a higher-priority exception
            unsigned int SVCALLPENDED : 1; // SVC pended; SVC was started but was replaced by a higher-priority exception
            unsigned int MEMFAULTENA : 1;  // Memory management fault handler enable
            unsigned int BUSFAULTENA : 1;  // Bus fault handler enable
            unsigned int USGFAULTENA : 1;  // Usage fault handler enable
        } bits;
    } syshndctrl; // System Handler Control and State Register (0xE000ED24)

    union {
        volatile unsigned char byte;
        struct {
            unsigned char IACCVIOL : 1; // Instruction access violation
            unsigned char DACCVIOL : 1; // Data access violation
            unsigned char UNUSED1 : 1;
            unsigned char MUNSTKERR : 1; // Unstacking error
            unsigned char MSTKERR : 1;   // Stacking error
            unsigned char UNUSED2 : 2;
            unsigned char MMARVALID : 1; // Indicates the MMAR is valid
        } bits;
    } mfsr; // Memory Management Fault Status Register (0xE000ED28)

    union {
        volatile unsigned int byte;
        struct {
            unsigned int IBUSERR : 1;    // Instruction access violation
            unsigned int PRECISERR : 1;  // Precise data access violation
            unsigned int IMPREISERR : 1; // Imprecise data access violation
            unsigned int UNSTKERR : 1;   // Unstacking error
            unsigned int STKERR : 1;     // Stacking error
            unsigned int UNUSED : 2;
            unsigned int BFARVALID : 1; // Indicates BFAR is valid
        } bits;
    } bfsr; // Bus Fault Status Register (0xE000ED29)

    volatile unsigned int bfar; // Bus Fault Manage Address Register (0xE000ED38)
    union {
        volatile unsigned short byte;
        struct {
            unsigned short UNDEFINSTR : 1; // Attempts to execute an undefined instruction
            unsigned short INVSTATE : 1;   // Attempts to switch to an invalid state (e.g., ARM)
            unsigned short INVPC : 1;      // Attempts to do an exception with a bad value in the EXC_RETURN number
            unsigned short NOCP : 1;       // Attempts to execute a coprocessor instruction
            unsigned short UNUSED : 4;
            unsigned short UNALIGNED : 1; // Indicates that an unaligned access fault has taken place
            unsigned short DIVBYZERO : 1; // Indicates a divide by zero has taken place (can be set only if DIV_0_TRP is set)
        } bits;
    } ufsr; // Usage Fault Status Register (0xE000ED2A)

    union {
        volatile unsigned int byte;
        struct {
            unsigned int UNUSED1 : 1;
            unsigned int VECTBL : 1; // Indicates hard fault is caused by failed vector fetch
            unsigned int UNUSED2 : 28;
            unsigned int FORCED : 1;   // Indicates hard fault is taken because of bus fault/memory management fault/usage fault
            unsigned int DEBUGEVT : 1; // Indicates hard fault is triggered by debug event
        } bits;
    } hfsr; // Hard Fault Status Register (0xE000ED2C)

    union {
        volatile unsigned int byte;
        struct {
            unsigned int HALTED : 1;   // Halt requested in NVIC
            unsigned int BKPT : 1;     // BKPT instruction executed
            unsigned int DWTTRAP : 1;  // DWT match occurred
            unsigned int VCATCH : 1;   // Vector fetch occurred
            unsigned int EXTERNAL : 1; // EDBGRQ signal asserted
        } bits;
    } dfsr;                     // Debug Fault Status Register (0xE000ED30)
    volatile unsigned int afsr; // Auxiliary Fault Status Register (0xE000ED3C) Vendor controlled (optional)
} cortex_hard_fault_t;

#endif // OS_FAULTS_H
