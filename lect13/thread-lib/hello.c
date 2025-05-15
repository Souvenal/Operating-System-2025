#include <thread.h>

void T_a() {
    while (1) {
        printf("a");
    }
}

void T_b() {
    while (1) {
        printf("b");
    }
}

int main() {
    spawn(T_a);
    spawn(T_b);
}
