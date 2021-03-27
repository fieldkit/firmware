
        .thumb_func
        .type   invoke_pic, %function
        .global invoke_pic
invoke_pic:
        .fnstart
        .cantunwind

        msr     MSP, r0
        mov     r9, r2

        /* Push GOT value onto stack, the application can then use this to check for corruption. */
        push    {r9}
        bx      r1

        .fnend
        .size   invoke_pic, .-invoke_pic
