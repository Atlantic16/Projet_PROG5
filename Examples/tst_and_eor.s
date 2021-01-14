.global main
.text
main:
	mov r0, #0xAA
	mov r1, #0x55
    
   	and r2, r0, r1
   	and r3, r1, #0x55
    
   	ands r4, r0, r1
    
   	eor r5, r0, r1
   	eors r6, r1, #0x55
   
   	swi 0x123456