.global main
.text
main:
    mov r0, #120
    mov r1, #5
    add r2, r1, r0, lsl #8
    add r3, r1, r0
    add r4 , r1, r2
    subs r5, r1, r0, lsr #8
    mov r0,#5
    tsteq r0, r1
    cmpne r1, r2
    mvn r0, #5
    swi 0x123456
