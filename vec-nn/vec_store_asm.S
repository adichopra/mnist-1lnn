## Hwacha v4 STORE code

#include "vec-util.h"

.text
.align 2

.globl vec_store_asm
.type  vec_store_asm,@function

# assumes calling convention:
# a0 has int n
# a1 has float* x
# a2 has float* y
vec_store_asm:
    li a4, VCFG(0, 4, 0, 1)
    vsetcfg a4
stripmine:
    vsetvl t0, a0 #a0 is requested vec len, actual is placed in t0
    vmca va0, a1
    vmca va1, a2
    la t5, store
    vf 0(t5)
    slli t1, t0, 2
    add a1, a1, t1
    add a2, a2, t1
    sub a0, a0, t0
    bnez a0, stripmine
    fence
    ret

# vector thread asm
.align 3
store:
    vpset vp0
    vlw vv0, va0
    vlw vv1, va1
    vstop