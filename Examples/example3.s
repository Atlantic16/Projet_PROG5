.global main
.text
main:
    ldr r0, =limite
    mov r4, #1
    mov r5, #8
    ldr r4, [r0]
    str r5, [r0, #4]
    strh r4,[r0, #8]
    strb r4,[r0, #12]!
    str r4,[r0, #4]
    ldrb r1, [r0]
    ldrh r2, [r0]
    ldr r3, [r0]
    ldr r6, [r0, r5, LSR #1 ]
    swi 0x123456
.data
limite:
    .word 0x12345678

