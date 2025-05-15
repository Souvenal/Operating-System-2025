#include <thread.h>
#include <stdatomic.h>

int x = 0, y = 0;
atomic_int flag;

#define F1  1
#define F2  2

#define FLAG \
    atomic_load(&flag)
#define FLAG_XOR(val) \
    atomic_fetch_xor(&flag, val)
#define WAIT_FOR(cond) \
    while (!(cond))    \
        ;

#if defined(__x86_64__)
__attribute__((noinline))
void write_x_read_y() {
    int y_;

    asm volatile(
        "movl $1, %0;" // x = 1
        "movl %2, %1;" // y_ = y
        : "=m"(x), "=r"(y_)
        : "m"(y)
    );

    printf("%d ", y_);
}

__attribute__((noinline))
void write_y_read_x() {
    int x_;

    asm volatile(
        "movl $1, %0;" // y = 1
        "movl %2, %1;" // x_ = x
        : "=m"(y), "=r"(x_)
        : "m"(x)
    );

    printf("%d ", x_);
}
#elif defined(__aarch64__)
__attribute__((noinline))
void write_x_read_y() {
    int y_;
    asm volatile(
        "mov w0, #1\n\t"      // w0 = 1
        "str w0, %0\n\t"      // x = 1
        "ldr w1, %2\n\t"      // w1 = y
        "mov %w1, w1\n\t"     // y_ = w1
        : "=m"(x), "=r"(y_)
        : "m"(y)
        : "w0", "w1", "memory"
    );
    printf("%d ", y_);
}

__attribute__((noinline))
void write_y_read_x() {
    int x_;
    asm volatile(
        "mov w0, #1\n\t"      // w0 = 1
        "str w0, %0\n\t"      // y = 1
        "ldr w1, %2\n\t"      // w1 = x
        "mov %w1, w1\n\t"     // x_ = w1
        : "=m"(y), "=r"(x_)
        : "m"(x)
        : "w0", "w1", "memory"
    );
    printf("%d ", x_);
}
#endif

void T_1(int id) {
    while (1) {
        // Wait until F1 is raised.
        WAIT_FOR((FLAG & F1));

        write_x_read_y();

        // Put F1 down.
        FLAG_XOR(F1);
    }
}

void T_2() {
    while (1) {
        // Wait until F2 is raised.
        WAIT_FOR((FLAG & F2));

        write_y_read_x();

        // Put F2 down.
        FLAG_XOR(F2);
    }
}

void T_flag() {
    while (1) {
        x = 0;
        y = 0;
        __sync_synchronize(); // full barrier
        usleep(1);            // + delay

        // Now, x = 0, y = 0, and flag = 0.
        // Both T_1 and T_2 are waiting for their signals.
        assert(FLAG == 0);

        // flag = 3; Both flags are raised.
        FLAG_XOR(F1 | F2);

        // T1 and T2 are ready to go...
        // They will eventually put F1 and F2 down.
        WAIT_FOR(FLAG == 0);

        printf("\n");
        fflush(stdout);
    }
}

int main() {
    // All infinite loops
    spawn(T_1);
    spawn(T_2);
    spawn(T_flag);
}
