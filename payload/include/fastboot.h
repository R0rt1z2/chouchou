#pragma once

#include <stdint.h>

struct fastboot_cmd {
    struct fastboot_cmd *next;
    const char *prefix;
    unsigned int prefix_len;
    int allowed_when_security_on;
    int forbidden_when_lock_on;
    void (*handle)(const char *arg, void *data, unsigned int sz);
};

void fastboot_info(const char *reason);
void fastboot_fail(const char *reason);
void fastboot_okay(const char *reason);
void fastboot_register(const char *prefix,
                       void (*handle)(const char *arg, void *data, unsigned sz),
                       unsigned char security_enabled);
struct fastboot_cmd *get_fastboot_cmd_list();

int fastboot_is_protected_partition(const char *partition);
