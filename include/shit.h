#ifndef SHIT_H
#define SHIT_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Minimal libc-free string functions (vibe-coded)
static inline size_t my_strlen(const char* s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

static inline char* my_strchr(const char* s, int c) {
    while (*s) {
        if (*s == (char)c) return (char*)s;
        s++;
    }
    return NULL;
}

static inline char* my_strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;

    for (; *haystack; haystack++) {
        const char* h = haystack;
        const char* n = needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }

        if (!*n) return (char*)haystack;
    }

    return NULL;
}

static inline void* my_memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

static inline int my_snprintf(char* buf, size_t size, const char* fmt, ...) {
    // Very simple sprintf that only handles %s
    size_t pos = 0;
    const char* p = fmt;
    __builtin_va_list args;
    __builtin_va_start(args, fmt);

    while (*p && pos < size - 1) {
        if (*p == '%' && *(p + 1) == 's') {
            const char* str = __builtin_va_arg(args, const char*);
            while (*str && pos < size - 1) {
                buf[pos++] = *str++;
            }
            p += 2;
        } else {
            buf[pos++] = *p++;
        }
    }

    buf[pos] = '\0';
    __builtin_va_end(args);
    return (int)pos;
}

#endif // SHIT_H
