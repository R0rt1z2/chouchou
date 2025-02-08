#include "include/arm.h"
#include "include/commands.h"
#include "include/common.h"

int app() {
    return ((int (*)(void))(0x4c42b790 | 1))();
}

void platform_init() {
    ((void (*)(void))(0x4c404864 | 1))();
}

void late_init() {
    register_commands();

    volatile uint16_t *x = (volatile uint16_t *)0x4c44f0c0; // orange_state_warning
    x[0] = 0x2000; // movs r0, #0
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)0x4c44f0c0, 2);

    x = (volatile uint16_t *)0x4c44eff0; // red_state_warning
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)0x4c44eff0, 2);
}

void early_init() {
    __asm__("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));
    ((volatile uint32_t *)0x4c5207c8)[0] = BOOT_STATE_GREEN;
    arch_clean_invalidate_cache_range(0x4c5207c8, 4);

    volatile uint16_t *x = (volatile uint16_t *)0x4c42c8e0; // register cmd_flash
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)0x4c42c8e0, 2);

    x = (volatile uint16_t *)0x4c42cb72; // register cmd_flashing_lock
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)0x4c42cb72, 2);
}