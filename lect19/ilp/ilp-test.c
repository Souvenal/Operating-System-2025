#include <stdio.h>

#define N 100000000

int x, y;

int main() {
#if defined(__x86_64__)
    register int i asm("edi");
    
    for (i = 0; i < N; i++) {
        // There're too many dependencies;
        // I didn't list them.

        asm volatile(
            // General-purpose registers
            "mov $0, %rax;"
            "mov $0, %rbx;"
            "mov $0, %rcx;"
            "mov $0, %rdx;"
            "mov $0, %rsi;"
            "mov $0, %r8;"
            "mov $0, %r9;"
            "mov $0, %r10;"
            "mov $0, %r11;"

            // SIMD registers
            "movaps %xmm0, %xmm1;"
            "movaps %xmm0, %xmm2;"
            "movaps %xmm0, %xmm3;"
            "movaps %xmm0, %xmm4;"
            "movaps %xmm0, %xmm5;"
            "movaps %xmm0, %xmm6;"
            "movaps %xmm0, %xmm7;"
            "movaps %xmm0, %xmm8;"
        );

    }
#elif defined(__aarch64__)
    int i;
    for (i = 0; i < N; i++) {
        asm volatile(
            // 通用寄存器清零
            "mov x0, #0\n\t"
            "mov x1, #0\n\t"
            "mov x2, #0\n\t"
            "mov x3, #0\n\t"
            "mov x4, #0\n\t"
            "mov x5, #0\n\t"
            "mov x6, #0\n\t"
            "mov x7, #0\n\t"
            "mov x8, #0\n\t"
            "mov x9, #0\n\t"
            "mov x10, #0\n\t"
            "mov x11, #0\n\t"

            // SIMD 寄存器复制
            "mov v1.16b, v0.16b\n\t"
            "mov v2.16b, v0.16b\n\t"
            "mov v3.16b, v0.16b\n\t"
            "mov v4.16b, v0.16b\n\t"
            "mov v5.16b, v0.16b\n\t"
            "mov v6.16b, v0.16b\n\t"
            "mov v7.16b, v0.16b\n\t"
            "mov v8.16b, v0.16b\n\t"
        );
    }
#endif
}
