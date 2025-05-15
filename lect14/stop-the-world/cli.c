int main() {
#ifdef __x86_64__
        // 64-bit x86
        asm volatile("cli");
    // Clear FL_IF in the CPU.
    // Interrupt disabled.
    asm volatile("cli");

    // Set FL_IF in the CPU.
    // Interrupt enabled.
    asm volatile("sti");
#elif defined(__aarch64__)
        // 关闭IRQ中断
    asm volatile("msr daifset, #2");

    // 开启IRQ中断
    asm volatile("msr daifclr, #2");
#endif

    while (1);
}
