.global main
.text
main:
	mov r0, #0x15
	mov r1, #0x5
	
	add r2, r0, r1
	add r3, r0, #20

	adc r4, r0, r1
	adc r5, r0, r1

	mov r0, #0xFFFFFFF0
	adcs r1, r0, #0x10
   	adds r1, r0, #0xF

   swi 0x123456
