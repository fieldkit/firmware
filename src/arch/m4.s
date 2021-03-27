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

        tst    lr, #4
        ite    EQ
        mrseq  r0, msp
        mrsne  r0, psp
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

        /* Check to see if we're switching to the same task. */
        ldr      r3, =osg
        ldr      r1, [r3, #OSG_RUNNING]
        ldr      r2, [r3, #OSG_SCHEDULED]
        cmp      r1, r2
        beq      pendsv_done
        mov      r0, #0
        cmp      r2, r0
        beq      pendsv_done

        /* Disable interrupts: */
        cpsid	i

        mrs      r12, psp
        tst      lr, #0x10                        /* Is it extended frame? */
        itte     eq
        vstmdbeq r12!, {s16 - s31}
        moveq    r0, #0x01
        movne    r0, #0x00
        strb     r0, [r1, #OS_TASK_STACK_KIND]    /* osg.running->stack_kind = val */
        stmdb    r12!, {r4 - r11}
        str      r12, [r1, #OS_TASK_SP]           /* Update osg.running->sp */

        push     {r2, r3}
        bl       osi_stack_check
        pop      {r2, r3}

        /* running_task = scheduled_task; */
        ldr      r3, =osg
        ldr      r2, [r3, #OSG_SCHEDULED]
        str      r2, [r3, #OSG_RUNNING]

        /* scheduled_task = 0; */
        movs     r2, #0
        str      r2, [r3, #OSG_SCHEDULED]

        ldr      r3, [r3, #OSG_RUNNING]
        ldr      r12, [r3, #OS_TASK_SP]           /* Assign osg.running->sp */
        ldmia    r12!, {r4 - r11}
        ldrb     r0, [r3, #OS_TASK_STACK_KIND]
        cmp      r0, #0                           /* Basic/Extended Stack Frame */
        itee     eq
        mvneq    lr, #~0xFFFFFFFD                 /* EXC_RETURN value */
        mvnne    lr, #~0xFFFFFFED
        vldmiane r12!, {s16 - s31}
        msr      psp, r12                         /* Write PSP */

        /* Enable interrupts: */
        cpsie	i

pendsv_done:
        bx       lr                               /* Return to Thread Mode */

        .fnend
        .size   PendSV_Handler, .-PendSV_Handler

/*-------------------------- SVC_Handler ---------------------------------*/

        .thumb_func
        .type   SVC_Handler, %function
        .global SVC_Handler
SVC_Handler:
        .fnstart
        .cantunwind

        mrs       r0, psp                         /* read psp */
        ldr       r1, [r0, #24]                   /* read saved pc from stack */

        // No user defined SVC's for now.
        // ldrb      r1, [r1, #-2]                /* load svc number */
        // cbnz      r1, svc_user

        ldm       r0, {r0 - r3, r12}
        push      {r4, lr}                        /* save EXC_RETURN */
        blx       r12                             /* call svc function */
        pop       {r4, lr}                        /* restore EXC_RETURN */

        mrs       r12, psp                        /* read psp */
        stm       r12, {r0 - r2}                  /* store return values */

        ldr       r3, =osg                        /* osg.running, osg.scheduled */
        ldr       r1, [r3, #OSG_RUNNING]
        ldr       r2, [r3, #OSG_SCHEDULED]

        cmp       r2, #0                          /* scheduled == NULL? */
        it        eq
        bxeq      lr                              /* no task switch */

        cmp       r1, r2                          /* running == scheduled? */
        it        eq
        bxeq      lr                              /* no task switch */

        cbnz      r1, svc_ctx_save                /* task not deleted? */

        tst       lr, #0x10                       /* extended frame? */
        bne       svc_ctx_restore
        ldr       r1, =0xe000ef34
        ldr       r0, [r1]                        /* load FPCCR */
        bic       r0, #1                          /* clear LSPACT (lazy state) */
        str       r0, [r1]                        /* store FPCCR */
        b         svc_ctx_restore

svc_ctx_save:
        tst       lr, #0x10                       /* check stack frame kind? */
        itte      eq
        vstmdbeq  r12!, {s16 - s31}               /* yes, stack also vfp hi-regs */
        moveq     r0, #0x01
        movne     r0, #0x00
        strb      r0, [r1, #OS_TASK_STACK_KIND]   /* osg.running->stack_kind = val */
        stmdb     r12!, {r4 - r11}                /* save old  */
        str       r12, [r1, #OS_TASK_SP]          /* update osg.running->sp_stack */

        push      {r2, r3}
        bl        osi_stack_check
        pop       {r2, r3}

svc_ctx_restore:
        str       r2, [r3]                        /* osg.running = osg.scheduled */
        movs      r0, #0
        str       r0, [r3, #OSG_SCHEDULED]        /* osg.scheduled = NULL */

        ldr       r12, [r2, #OS_TASK_SP]          /* osg.scheduled->sp */
        ldmia     r12!, {r4 - r11}
        ldrb      r0, [r2, #OS_TASK_STACK_KIND]
        cmp       r0, #0                          /* check if stack frame kind */
        itee      eq
        mvneq     lr, #~0xFFFFFFFD                /* EXC_RETURN value */
        mvnne     lr, #~0xFFFFFFED
        vldmiane  r12!, {s16-s31}                 /* restore vfp hi-registers */
        msr       psp, r12                        /* write psp */

svc_done:
        bx        lr

        .fnend
        .size   SVC_Handler, .-SVC_Handler
