#pragma once

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define VERSION "0.1"

/* verified boot state */
#define BOOT_STATE_GREEN   0x0
#define BOOT_STATE_YELLOW  0x1
#define BOOT_STATE_ORANGE  0x2
#define BOOT_STATE_RED     0x3

int app();
void platform_init();
void early_init();
void late_init();