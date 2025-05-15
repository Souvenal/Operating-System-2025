#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main() {
    pid_t x = fork();
    pid_t y = fork();
    printf("%d %d\n", x, y);
}
