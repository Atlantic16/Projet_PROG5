.global main
.text
main:

    ldr r1, =donnee
    ldr r2, =fin
    ldr r3, =limite
    ldr r0, =addr
    ldmda r3, {r4, r5, r6}
    stmia r0, {r4, r5, r6}
    swi 0x123456
.data
donnee:
    .word 0x11223344
fin:
    .word 0x55667788
limite:
    .word 0x12345678
addr: 
    .word 0x280c
    


    
