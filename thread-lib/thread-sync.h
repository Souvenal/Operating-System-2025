#include <semaphore.h>
#include <pthread.h>

// Spinlock
typedef int spinlock_t;
#define SPIN_INIT() 0

#ifdef __x86_64__
static inline int atomic_xchg(volatile int *addr, int newval) {
    int result;
    asm volatile ("lock xchg %0, %1":
        "+m"(*addr), "=a"(result) : "1"(newval) : "memory");
    return result;
}
#elif defined(__aarch64__)
static inline int atomic_xchg(volatile int *addr, int newval) {
    int old;
    unsigned int tmp;
    asm volatile(
        "1:     ldaxr   %w0, [%2]\n"
        "       stlxr   %w1, %w3, [%2]\n"
        "       cbnz    %w1, 1b\n"
        : "=&r"(old), "=&r"(tmp)
        : "r"(addr), "r"(newval)
        : "memory");
    return old;
}
#endif

void spin_lock(spinlock_t *lk) {
    while (1) {
        int value = atomic_xchg(lk, 1);
        if (value == 0) {
            break;
        }
    }
}
void spin_unlock(spinlock_t *lk) {
    atomic_xchg(lk, 0);
}

// Mutex
typedef pthread_mutex_t mutex_t;
#define MUTEX_INIT() PTHREAD_MUTEX_INITIALIZER
#define mutex_init(mutex) pthread_mutex_init(mutex, NULL)
#define mutex_lock pthread_mutex_lock
#define mutex_unlock pthread_mutex_unlock

// Conditional Variable
typedef pthread_cond_t cond_t;
#define COND_INIT() PTHREAD_COND_INITIALIZER
#define cond_wait pthread_cond_wait
#define cond_broadcast pthread_cond_broadcast
#define cond_signal pthread_cond_signal

// Semaphore
#define P sem_wait
#define V sem_post
#define SEM_INIT(sem, val) sem_init(sem, 0, val)
