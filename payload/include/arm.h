#pragma once

#include <stdint.h>

#define CACHE_LINE 32

#define READ_VBAR() ({ \
    uint32_t vbar; \
    asm volatile("MRC p15, 0, %0, c12, c0, 0" : "=r" (vbar)); \
    (uint32_t)vbar; \
})

unsigned int __aeabi_uidiv(unsigned int n, unsigned int d);
void patch_call(uint32_t patch_addr, void (*target_func)(void));
void arch_clean_invalidate_cache_range(uintptr_t start, uintptr_t size);
void parse_vector_table();