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

/*-------------------------- HardFault_Handler ---------------------------------*/

        .thumb_func
        .type   HardFault_Handler, %function
        .global HardFault_Handler
HardFault_Handler:
        .fnstart
        .cantunwind

        movs   r2, #4
        mov    r1, lr
        tst    r0, r2
        bne    hf_uses_psp
        mrs    r0, msp
        b      hf_pass_stack_ptr
        hf_uses_psp:
        mrs    r0, psp
        hf_pass_stack_ptr:
        mov    r1, lr
        b      osi_hard_fault_handler

        .fnend
        .size   HardFault_Handler, .-HardFault_Handler

/*-------------------------- PendSV_Handler ---------------------------------*/

        .thumb_func
        .type   PendSV_Handler, %function
        .global PendSV_Handler
PendSV_Handler:
        .fnstart
        .cantunwind

        /* Disable interrupts: */
        cpsid	i

        /* Check to see if we're switching to the same task. */
        ldr     r3, =osg
        ldr     r1, [r3, #OSG_RUNNING]
        ldr     r2, [r3, #OSG_SCHEDULED]
        cmp     r1, r2
        beq     pendsv_done
        movs    r0, #0
        cmp     r2, r0
        beq     pendsv_done

        /*
          Exception frame saved by the NVIC hardware onto stack:
          +------+
          |      | <- SP before interrupt (orig. SP)
          | xPSR |
          |  PC  |
          |  LR  |
          |  R12 |
          |  R3  |
          |  R2  |
          |  R1  |
          |  R0  | <- SP after entering interrupt (orig. SP + 32 bytes)
          +------+

          Registers saved by the software (PendSV_Handler):
          +------+
          |  R7  |
          |  R6  |
          |  R5  |
          |  R4  |
          |  R11 |
          |  R10 |
          |  R9  |
          |  R8  | <- Saved SP (orig. SP + 64 bytes)
          +------+
        */

        /* Save registers R4-R11 (32 bytes) onto current PSP (process stack
           pointer) and make the PSP point to the last stacked register (R8):
           - The MRS/MSR instruction is for loading/saving a special registers.
           - The STMIA inscruction can only save low registers (R0-R7), it is
           therefore necesary to copy registers R8-R11 into R4-R7 and call
           STMIA twice. */
        mrs	    r0, psp
        subs  	r0, #16
        stmia	  r0!, {r4 - r7}
        mov	    r4, r8
        mov	    r5, r9
        mov	    r6, r10
        mov	    r7, r11
        subs	  r0, #32
        stmia	  r0!, {r4 - r7}
        subs	  r0, #16

        /* Save current task's SP: */
        ldr     r3, =osg
        ldr     r2, [r3, #OSG_RUNNING]
        str	    r0, [r2, #OS_TASK_SP]

        /* check for stack overflow */
        push    {r2, r3}
        bl      osi_stack_check
        pop     {r2, r3}

        /* running_task = scheduled_task; */
        ldr     r3, =osg
        ldr     r2, [r3, #OSG_SCHEDULED]
        str     r2, [r3, #OSG_RUNNING]

        /* check for stack overflow */
        push    {r2, r3}
        bl      osi_stack_check
        pop     {r2, r3}

        /* Load next task's SP: */
        ldr     r3, =osg
        ldr     r2, [r3, #OSG_SCHEDULED]
        ldr	    r0, [r2, #OS_TASK_SP]
        /* scheduled_task = 0; */
        movs    r2, #0
        str     r2, [r3, #OSG_SCHEDULED]

        /* Load registers R4-R11 (32 bytes) from the new PSP and make the PSP
           point to the end of the exception stack frame. The NVIC hardware
           will restore remaining registers after returning from exception): */
        ldmia	  r0!, {r4 - r7}
        mov	    r8, r4
        mov	    r9, r5
        mov	    r10, r6
        mov  	  r11, r7
        ldmia	  r0!, {r4 - r7}
        msr	    psp, r0

        pendsv_done:
        /* EXC_RETURN - Thread mode with PSP: */
        ldr r0, =0xFFFFFFFD

        /* Enable interrupts: */
        cpsie	i

        bx	r0

        .fnend
        .size   PendSV_Handler, .-PendSV_Handler

/*-------------------------- SVC_Handler ------------------------------------*/

        .thumb_func
        .type   SVC_Handler, %function
        .global SVC_Handler
SVC_Handler:
        .fnstart
        .cantunwind

        mrs       r0, psp                         /* read psp */
        ldr       r1, [r0, #24]                   /* read saved pc from stack */
        subs      r1, r1, #2                      /* point to svc instruction */

        // No user defined SVC's for now.
        // ldrb    r1, [r1]                       /* load svc number */
        // cmp     r1, #0
        // bne     svc_user                       /* user svc number > 0 */

        mov       lr, r4
        ldmia     r0, {r0 - r3, r4}               /* read r0 - r3, r12 from stack */
        mov       r12, r4
        mov       r4, lr
        blx       r12                             /* call svc function */

        mrs       r3, psp                         /* read psp */
        stmia     r3!, {r0 - r2}                  /* store return values */

        ldr       r0, =osg                        /* osg.running, osg.scheduled */
        ldr       r1, [r0, #OSG_RUNNING]
        ldr       r2, [r0, #OSG_SCHEDULED]
        cmp       r2, #0                          /* scheduled == NULL? */
        beq       svc_done                        /* no task switch */
        cmp       r1, r2
        beq       svc_done                        /* no task switch */

        subs      r3, #8
        cmp       r1, #0                          /* task deleted? */
        beq       svc_switch

        mrs       r0, psp                         /* read psp */
        subs      r0, r0, #16                     /* adjust start address */
        stmia     r0!, {r4 - r7}                  /* save (r4 - r7) */
        mov       r4, r8
        mov       r5, r9
        mov       r6, r10
        mov       r7, r11
        subs      r0, r0, #32                     /* adjust start address */
        stmia     r0!, {r4 - r7}                  /* save (r8 - r11) */
        subs      r0, r0, #16                     /* adjust start address */
        str       r0, [r1, #OS_TASK_SP]           /* update osg.running->sp */

        push      {r2, r3}
        bl        osi_stack_check
        pop       {r2, r3}

svc_switch:
        ldr       r0, =osg
        str       r2, [r0, #OSG_RUNNING]          /* osg.running = osg.scheduled */
        movs      r1, #0
        str       r1, [r0, #OSG_SCHEDULED]        /* osg.scheduled = NULL */

        ldr       r0, [r2, #OS_TASK_SP]           /* osg.scheduled->sp */
        ldmia     r0!, {r4 - r7}                  /* restore (r8-r11) */
        mov       r8, r4
        mov       r9, r5
        mov       r10, r6
        mov       r11, r7
        ldmia     r0!, {r4-r7}                    /* restore r4-r7 */
        msr       psp, r0                         /* write psp */

svc_done:
        movs      r0, #~0XFFFFFFFD                /* EXC_RETURN value */
        mvns      r0, r0
        bx        r0                              /* return to thread mode, use psp */

        pop       {r4, pc}

        .fnend
        .size   SVC_Handler, .-SVC_Handler
