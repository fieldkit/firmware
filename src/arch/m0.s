
        .thumb_func
        .type   invoke_pic, %function
        .global invoke_pic
invoke_pic:
        .fnstart
        .cantunwind

        mov     r9, r1
        bx      r0

        .fnend
        .size   invoke_pic, .-invoke_pic
