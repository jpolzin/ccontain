#include "ccontain-hash.h"

#include <stdint.h>

#include "ccontain-common.h"

static CCONTAIN_HASH_FUNC(simple_hash, bytes, n_bytes) {
    uint8_t *bytes8 = (uint8_t *)bytes;
    size_t i = 0;
    uint64_t sum = 0;

    // TODO: This can be faster if bytes is aligned
    for (; i < n_bytes; ++i) {
        sum += BYTE_SHIFT(bytes8[i], i % sizeof(hash_t));
    }
    return sum;
}

static inline uint32_t get_four_bytes(const void *ptr) {
    // TODO: I think this will behave differently on systems with different endianness
    uint32_t out = 0;
    if (PTR_MOD(ptr, 4)) {
        // Aligned
        return *(uint32_t *)ptr;
    } else if (PTR_MOD(ptr, 2)) {
        // Mostly aligned
        uint16_t *ptr2 = (uint16_t *)ptr;
        out = *ptr2++;
        out |= (*ptr2) << 16;
        return out;
    } else {
        // Unaligned
        uint8_t *ptr1 = (uint8_t *)ptr;
        out = *ptr1;
        out |= (*(uint16_t *)(ptr1 + 8)) << 8;
        out |= *(ptr1 + 24) << 24;
        return out;
    }
}

static CCONTAIN_HASH_FUNC(murmur_hash, bytes, n_bytes) {
    // Source: Wikipedia description of the Murmur3_32 algorithm: https://en.wikipedia.org/wiki/MurmurHash
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const uint32_t r1 = 15;
    const uint32_t r2 = 13;
    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;
    uint32_t k = 0;

    size_t n_bytes_remaining = n_bytes;
    uintptr_t bytes_remaining = (uintptr_t)bytes;

    while (bytes_remaining > 4) {
        k = get_four_bytes(bytes);
        // TODO: Complete this function
    }
}

ccontain_hash_func_t ccontain_get_hash(const ccontain_hash_t hash) {
    switch (hash) {
    case HASH_SIMPLE:
        return simple_hash;
    case HASH_MURMUR:

    default:
        return NULL;
    }
}
