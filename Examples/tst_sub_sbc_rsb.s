.global main
.text
main:

	mov r0, #0x55

    sub r1, r0, #0x22
    subs r2, r0, r1
    subs r3, r1, r0

    sbc r4, r0, r1
    sbcs r4, r0, r1

    rsb r5, r1, #0xaf
    rsbs r5, r1, r0
         
    swi 0x123456