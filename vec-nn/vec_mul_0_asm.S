## Hwacha v4 MUL0 ASM code

#include "vec-util.h"

.text
.align 2

.globl vec_mul_0_asm
.type  vec_mul_0_asm,@function

# assumes calling convention:
# a0 has int n
# a1 has float* result  <---
# a2 has float* x
vec_mul_0_asm:
stripmine:
    vsetvl t0, a0 #a0 is requested vec len, actual is placed in t0
    vmca va2, a1
    vmca va0, a2
    la t5, mul_v_0
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
mul_v_0:
    vpset vp0
    vlw vv2, va0
    vfmul.s vv3, vv0, vv2
    vsw vv3, va2
    vstop