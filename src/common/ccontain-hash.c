#include "ccontain-hash.h"

#include <stdint.h>

#include "ccontain-common.h"

static CCONTAIN_HASH_FUNC(simple_hash, bytes, n_bytes) {
    uint8_t *bytes8 = (uint8_t *)bytes;
    size_t i = 0;
    uint64_t sum = 0;

    // TODO: This can be faster if bytes is aligned
    for (; i < n_bytes; ++i) {
        sum += BYTE_SHIFT(bytes8[i], i % 8);
    }

    return sum;
}

ccontain_hash_func_t ccontain_get_hash(const ccontain_hash_t hash) {
    switch (hash) {
    case HASH_SIMPLE:
        return simple_hash;
    default:
        return NULL;
    }
}