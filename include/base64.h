#ifndef BASE64_H
#define BASE64_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Base64 encoding/decoding without libc dependencies (vibe-coded)

static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Calculate the size needed for base64 encoded output
static inline size_t base64_encoded_size(size_t input_len) {
    return ((input_len + 2) / 3) * 4;
}

// Calculate the maximum size needed for base64 decoded output
static inline size_t base64_decoded_size(size_t input_len) {
    return (input_len / 4) * 3;
}

// Base64 encode
static inline size_t base64_encode(const uint8_t* input, size_t input_len, char* output, size_t output_size) {
    if (output_size < base64_encoded_size(input_len)) {
        return 0;
    }

    size_t i = 0, j = 0;

    while (i < input_len) {
        uint32_t octet_a = i < input_len ? input[i++] : 0;
        uint32_t octet_b = i < input_len ? input[i++] : 0;
        uint32_t octet_c = i < input_len ? input[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        output[j++] = base64_chars[(triple >> 18) & 0x3F];
        output[j++] = base64_chars[(triple >> 12) & 0x3F];
        output[j++] = base64_chars[(triple >> 6) & 0x3F];
        output[j++] = base64_chars[triple & 0x3F];
    }

    // Add padding
    size_t padding = (3 - (input_len % 3)) % 3;
    for (size_t p = 0; p < padding; p++) {
        output[j - 1 - p] = '=';
    }

    return j;
}

// Helper to get the value of a base64 character
static inline int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '=') return -1; // padding
    return -2; // invalid
}

// Base64 decode
static inline size_t base64_decode(const char* input, size_t input_len, uint8_t* output, size_t output_size) {
    if (input_len == 0 || input_len % 4 != 0) {
        return 0;
    }

    size_t max_output = base64_decoded_size(input_len);
    if (output_size < max_output) {
        return 0;
    }

    size_t i = 0, j = 0;

    while (i < input_len) {
        int val0 = base64_char_value(input[i++]);
        int val1 = base64_char_value(input[i++]);
        int val2 = base64_char_value(input[i++]);
        int val3 = base64_char_value(input[i++]);

        if (val0 < 0 || val1 < 0) {
            return 0; // invalid
        }

        uint32_t triple = (val0 << 18) + (val1 << 12);
        output[j++] = (triple >> 16) & 0xFF;

        if (val2 >= 0) {
            triple += (val2 << 6);
            output[j++] = (triple >> 8) & 0xFF;

            if (val3 >= 0) {
                triple += val3;
                output[j++] = triple & 0xFF;
            }
        }
    }

    return j;
}

#endif // BASE64_H
