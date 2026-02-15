__zero = 0
__r0 = 0
__r1 = 0
__r2 = 0
__r3 = 0
__r4 = 0
__r5 = 0
__r6 = 0
__r7 = 0
__r8 = 0
__r9 = 0
__r10 = 0
__r11 = 0
__r12 = 0
__r13 = 0
__r14 = 0
__r15 = 0
__r16 = 0
__r17 = 0
__r18 = 0
__r19 = 0
__r20 = 0
__r21 = 0
__r22 = 0
__r23 = 0
__r24 = 0
__r25 = 0
__r26 = 0
__r27 = 0
__r28 = 0
__r29 = 0
__r30 = 0
__r31 = 0
__sp = 0
__fp = 0
__ra = 0
n = 10
out = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
_start:
	set __r0 0
	store out __r0 __r0
	set __r1 out
	set __r2 4
	add __r1 __r1 __r2
	set __r2 1
	store_a __r1 __r0 __r2
	set __r1 2
	set __r2 8
	store_a __r2 __r0 __r1
	jmp LBB0_1
LBB0_1:
	set __r0 0
	set __r1 8
	load_a __r1 __r1 __r0
	load __r2 n __r0
	lt __r1 __r2 __r1
	lte __r0 __r1 __r0
	jmp0 __r0 LBB0_4
	jmp LBB0_2
LBB0_2:
	set __r0 0
	set __r1 8
	load_a __r1 __r1 __r0
	set __r2 -4
	set __r3 out
	set __r4 4
	mul __r1 __r1 __r4
	add __r1 __r3 __r1
	add __r2 __r1 __r2
	load_a __r2 __r2 __r0
	set __r3 -8
	add __r3 __r1 __r3
	load_a __r3 __r3 __r0
	add __r2 __r2 __r3
	store_a __r1 __r0 __r2
	jmp LBB0_3
LBB0_3:
	set __r0 0
	set __r1 8
	load_a __r2 __r1 __r0
	set __r3 1
	add __r2 __r2 __r3
	store_a __r1 __r0 __r2
	jmp LBB0_1
LBB0_4:
	set __r0 0
	load __r1 n __r0
	set __r2 4
	mul __r1 __r1 __r2
	set __r2 out
	add __r1 __r2 __r1
	load_a __r0 __r1 __r0
	halt
