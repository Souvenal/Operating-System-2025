#include <stdarg.h>
#include <stddef.h>
#include <sys/syscall.h>

#ifdef __x86_64__
static inline long syscall(int num, ...) {
    va_list ap;
    va_start(ap, num);
    register long a0 asm("rax") = num;
    register long a1 asm("rdi") = va_arg(ap, long);
    register long a2 asm("rsi") = va_arg(ap, long);
    register long a3 asm("rdx") = va_arg(ap, long);
    register long a4 asm("r10") = va_arg(ap, long);
    va_end(ap);
    asm volatile("syscall"
                 : "+r"(a0) : "r"(a1), "r"(a2), "r"(a3), "r"(a4)
                 : "memory", "rcx", "r8", "r9", "r11");
    return a0;
}
#else
static inline long syscall(int num, ...) {
    va_list ap;
    va_start(ap, num);
    register long x8 asm("x8") = num;
    register long x0 asm("x0") = va_arg(ap, long);
    register long x1 asm("x1") = va_arg(ap, long);
    register long x2 asm("x2") = va_arg(ap, long);
    register long x3 asm("x3") = va_arg(ap, long);
    register long x4 asm("x4") = va_arg(ap, long);
    register long x5 asm("x5") = va_arg(ap, long);
    va_end(ap);
    asm volatile("svc 0"
                 : "+r"(x0)
                 : "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5), "r"(x8)
                 : "memory");
    return x0;
}
#endif

static inline size_t strlen(const char *s) {
    size_t len = 0;
    for (; *s; s++)
        len++;
    return len;
}

static inline char *strcpy(char *d, const char *s) {
    char *r = d;
    while (*s) {
        *d++ = *s++;
    }
    *d = '\0';
    return r;
}

static inline char *strchr(const char *s, int c) {
    for (; *s; s++) {
        if (*s == c)
            return (char *)s;
    }
    return NULL;
}

static inline void print(const char *s, ...) {
    va_list ap;
    va_start(ap, s);
    while (s) {
        syscall(SYS_write, 2, s, strlen(s));
        s = va_arg(ap, const char *);
    }
    va_end(ap);
}

#define assert(cond)                    \
    do {                                \
        if (!(cond))                    \
        {                               \
            print("Panicked.\n", NULL); \
            syscall(SYS_exit, 1);       \
        }                               \
    } while (0)

static char mem[4096], *freem = mem;

static inline void *zalloc(size_t sz) {
    // This is memory leak!
    assert(freem + sz < mem + sizeof(mem));
    void *ret = freem;
    freem += sz;
    return ret;
}
