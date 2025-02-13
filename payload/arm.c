#include "include/arm.h"
#include "include/printf.h"

// ARM does not have a division instruction.
unsigned int __aeabi_uidiv(unsigned int n, unsigned int d) {
    unsigned int q = 0;
    while (n >= d) {
        n -= d;
        q++;
    }
    return q;
}

void patch_call(uint32_t patch_addr, void (*target_func)(void), jump_type_t type) {
    uint32_t current_address = patch_addr + 4;
    uint32_t target_address = (uint32_t)target_func & ~1;
    int32_t offset = target_address - current_address;

    uint16_t high_offset = (offset >> 12) & 0x7FF;
    uint16_t low_offset = (offset >> 1) & 0x7FF;

    uint16_t high_instruction = 0xF000 | high_offset;
    uint16_t low_instruction = (type == JUMP_BLX) ? (0xE800 | low_offset) : (0xF800 | low_offset);

    volatile uint16_t* ptr = (volatile uint16_t*)patch_addr;
    *ptr = high_instruction;
    *(ptr + 1) = low_instruction;
}

void arch_clean_invalidate_cache_range(uintptr_t start, uintptr_t size) {
    uintptr_t end = start + size;
    start &= ~(CACHE_LINE - 1);

    while (start < end) {
        __asm__ volatile(
            "mcr p15, 0, %0, c7, c14, 1\n" // invalidate cache by MVA
            "add %0, %0, %[clsize]\n"
            : "+r" (start)
            : [clsize] "I" (CACHE_LINE)
            : "memory"
        );
    }

    __asm__ volatile("mcr p15, 0, %0, c7, c10, 4\n" :: "r" (0) : "memory");
}