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
out = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
main:
	set __r0 0
	set __r1 0
	store_a __r1 __r0 __r0
	store out __r0 __r0
	set __r1 4
	add __r1 __r0 __r1
	set __r2 1
	store_a __r1 __r0 __r2
	set __r1 2
	set __r2 4
	store_a __r2 __r0 __r1
LBB0_1:
	set __r0 0
	set __r1 4
	load_a __r1 __r1 __r0
	set __r1 -4
	add __r2 __r0 __r0
	add __r1 __r2 __r1
	load_a __r1 __r1 __r0
	set __r3 -8
	add __r3 __r2 __r3
	load_a __r3 __r3 __r0
	add __r1 __r1 __r3
	store_a __r2 __r0 __r1
	set __r0 0
	set __r1 4
	load_a __r2 __r1 __r0
	set __r3 1
	add __r2 __r2 __r3
	store_a __r1 __r0 __r2
LBB0_4:
	set __r0 0
	halt
