#include "include/fastboot.h"

void fastboot_info(const char *reason) {
    ((void (*)(const char *reason))(0x4c42c178 | 1))(reason);
}

void fastboot_fail(const char *reason) {
    ((void (*)(const char *reason))(0x4c42c1f8 | 1))(reason);
}

void fastboot_okay(const char *reason) {
    ((void (*)(const char *reason))(0x4c42c3c4 | 1))(reason);
}

void fastboot_register(const char *prefix,
                       void (*handle)(const char *arg, void *data, unsigned sz),
                       unsigned char security_enabled) {
    ((void (*)(const char *prefix, void (*handle)(const char *arg, void *data, unsigned sz), unsigned char security_enabled))(0x4c42bdc8 | 1))(prefix, handle, security_enabled);
}

struct fastboot_cmd *get_fastboot_cmd_list() {
    return (struct fastboot_cmd *)0x4c5b3144;
}