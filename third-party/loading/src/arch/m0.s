
        .thumb_func
        .type   invoke_pic, %function
        .global invoke_pic
invoke_pic:
        .fnstart
        .cantunwind

        /*
        Disabled for now:
        1) We aren't using this functionality.
        mov     r9, r1
        */

        bx      r0

        .fnend
        .size   invoke_pic, .-invoke_pic
