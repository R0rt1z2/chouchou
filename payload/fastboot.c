#include "include/arm.h"
#include "include/fastboot.h"
#include "string.h"
#include "include/memory.h"

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

void fastboot_publish(const char *name, const char *value) {
    ((void (*)(const char *name, const char *value))(0x4c42be04 | 1))(name, value);
}

int fastboot_is_protected_partition(const char *partition) {
    // TODO: Handle protected partitions properly
    return (strcmp(partition, "boot0") == 0 || 
        strcmp(partition, "boot1") == 0 || 
        strcmp(partition, "boot2") == 0 || 
        strcmp(partition, "preloader") == 0 ||
        strcmp(partition, "preloader_a") == 0 ||
        strcmp(partition, "preloader_b") == 0);
}

struct fastboot_cmd *get_fastboot_cmd_list() {
    uint32_t vbar = READ_VBAR();
    uint8_t *start = (uint8_t *)(vbar & ~0xFFF);
    uint8_t *end = start + 0x200000;

    const uint8_t pattern[] = {0x4C, 0x1B, 0x5F, 0x4C, 0x00};
    uint8_t *found = FIND_PATTERN(start, end, pattern);
    return found ? (struct fastboot_cmd *)found : NULL;
}
