#include "include/printf.h"
#include "include/arm.h"
#include "include/common.h"

void chouchou() {
    late_init();
    int ret = app();

    // app() is expected to never return
    LOGE("app() returned %d\n", ret);
    while (1) {
        // Wait for WDT to reset the SoC
        __asm__("wfi");
    }
}

__attribute__((section(".text.start"))) int main(void) {
    dprintf("This is chouchou payload by R0rt1z2. Copyright 2025\n");

    patch_call(0x4c42794c, (void *)chouchou, JUMP_BL);
    early_init();
    platform_init();

    return 0;
}