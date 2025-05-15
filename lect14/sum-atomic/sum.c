#include "thread.h"

#define N 100000000

long sum = 0;

void T_sum() {
    for (int i = 0; i < N; i++) {

        // "Lock" prefix is available for most arithmetic or
        // logical operations with a memory operand.
        //
        // asm volatile(
        //     "lock incq %0" : "+m"(sum)
        // );
#if defined(__x86_64__)
        asm volatile(
            "lock addq $1, %0" : "+m"(sum)
        );
    }
#elif defined(__aarch64__)
        asm volatile(
            "1: ldxr x1, [%[addr]]\n"
            "add x1, x1, #1\n"
            "stxr w2, x1, [%[addr]]\n"
            "cbnz w2, 1b\n"
            :
            : [addr] "r" (&sum)
            : "x1", "w2", "memory"
        );
#endif
    }
}

int main() {
    create(T_sum);
    create(T_sum);

    join();

    printf("sum = %ld\n", sum);
    printf("2*n = %ld\n", 2L * N);
}
