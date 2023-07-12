
        .thumb_func
        .type   invoke_pic, %function
        .global invoke_pic
invoke_pic:
        .fnstart
        .cantunwind

        /* Push GOT value onto stack, the application can then use this to check for corruption. */
        /*
        Disabled for now:
        1) We aren't using this functionality.
        2) If we do need this, then this push should be 8 bytes aligned to
        ensure that the SP remains aligned as such.

        mov     r9, r2
        push    {r9}
        */
        
        msr     MSP, r0
        bx      r1

        .fnend
        .size   invoke_pic, .-invoke_pic
