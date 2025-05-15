#include <thread.h>

extern long sum, N;

#define LOCKED   1
#define UNLOCKED 0

int status = UNLOCKED;

void lock() {
    int expected;
    do {
        // Try compare status with expected.
        // If the comparison succeeded, perform
        // an exchange.
        expected = UNLOCKED;
#if defined(__x86_64__)
        asm volatile (
            "lock cmpxchgl %2, %1"
            : "+a" (expected) // Value for comparison.
                              // x86 uses eax/rax.
            : "m" (status),   // Memory location.
              "r" (LOCKED)    // Value to be written if
                              // status == expected
            : "memory", "cc"
        );
#elif defined(__aarch64__)
        int result;
        asm volatile (
            "mov %w[result], #0\n"
            "1: ldaxr %w0, [%1]\n"
            "cmp %w0, %w2\n"
            "b.ne 2f\n"
            "stlxr %w[result], %w3, [%1]\n"
            "cbnz %w[result], 1b\n"
            "2:"
            : "+r"(expected), [result] "=&r"(result)
            : "r"(&status), "r"(UNLOCKED), "r"(LOCKED)
            : "memory"
        );
#endif
    } while (expected != UNLOCKED);
}

void unlock() {
    status = UNLOCKED;
#ifdef __x86_64__
    asm volatile("" ::: "memory");
#elif defined(__aarch64__)
    asm volatile("dmb ish" ::: "memory");
#endif
    __sync_synchronize();
}


void T_sum() {
    for (int i = 0; i < N; i++) {
        lock();
        sum++;
        unlock();
    }
}
