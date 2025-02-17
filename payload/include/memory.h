#pragma once

#define FIND_PATTERN(start, end, pattern)                                                   \
    ({                                                                                      \
        uint8_t* result = NULL;                                                             \
        size_t pattern_size = sizeof(pattern);                                              \
        for (uint8_t* ptr = (uint8_t*)(start); ptr < (uint8_t*)(end)-pattern_size; ptr++) { \
            if (memcmp(ptr, pattern, pattern_size) == 0) {                                  \
                result = ptr;                                                               \
                break;                                                                      \
            }                                                                               \
        }                                                                                   \
        result;                                                                             \
    })

void *malloc(size_t size);
void free(void *ptr);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t count);
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *cs, const void *ct, size_t count);
void *memmove(void *dest, void const *src, size_t count);
void *memscan(void *addr, int c, size_t size);