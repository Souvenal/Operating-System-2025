extern long sum, N;

void T_sum() {
    for (int i = 0; i < N; i++) {
#if defined(__x86_64__)
        asm volatile(
            "lock addq $1, %0" : "+m"(sum)
        );
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
