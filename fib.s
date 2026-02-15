__r0 = 0
__r1 = 0
__r2 = 0
__r3 = 0
__r4 = 0
__sp = 0
_start:
	set __r0 10
	set __r1 51
	set __r2 __sp
	store_a __r2 __r1 __r0
	set __r0 0
	set __r1 1
	store_a __r2 __r1 __r0
	set __r0 1
	add __r1 __r1 __r0
	store_a __r2 __r1 __r0
	set __r0 2
	set __r1 52
	store_a __r2 __r1 __r0
	jmp LBB0_1
LBB0_1:
	set __r0 52
	set __r1 __sp
	load_a __r0 __r1 __r0
	set __r2 51
	load_a __r1 __r1 __r2
	lt __r0 __r1 __r0
	set __r1 0
	lte __r0 __r0 __r1
	jmp0 __r0 LBB0_4
	jmp LBB0_2
LBB0_2:
	set __r0 52
	set __r1 __sp
	load_a __r0 __r1 __r0
	set __r2 -1
	add __r2 __r0 __r2
	set __r3 1
	add __r2 __r3 __r2
	load_a __r2 __r1 __r2
	set __r4 -2
	add __r4 __r0 __r4
	add __r4 __r3 __r4
	load_a __r4 __r1 __r4
	add __r2 __r2 __r4
	add __r0 __r3 __r0
	store_a __r1 __r0 __r2
	jmp LBB0_3
LBB0_3:
	set __r0 52
	set __r1 __sp
	load_a __r2 __r1 __r0
	set __r3 1
	add __r2 __r2 __r3
	store_a __r1 __r0 __r2
	jmp LBB0_1
LBB0_4:
	set __r0 51
	set __r1 __sp
	load_a __r0 __r1 __r0
	set __r2 1
	add __r0 __r2 __r0
	load_a __r0 __r1 __r0
	halt
