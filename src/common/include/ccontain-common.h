#ifndef CCONTAIN_COMMON_H
#define CCONTAIN_COMMON_H

#include "ccontain-defines.h"

#define ASSERT(cond)                                                                                                   \
    if (!(cond)) {                                                                                                     \
        printf("Assertion failed in %s on line %zu. Condition: %s\n", __FILE__, (size_t)__LINE__, #cond);              \
        exit(1);                                                                                                       \
    }

static inline void *assert_alloc(const size_t sz, void *(*allocator)(size_t sz)) {
    void *__ptr = allocator((sz));
    ASSERT(__ptr != NULL);
    return __ptr;
}

#ifdef DEBUG
#define ALLOC(sz) assert_alloc(sz, malloc)
#else
#define ALLOC(sz) malloc((sz))
#endif

#define FREE(ptr) free(ptr)

/* Bit/byte math helper functions */
#define ALIGN_UP(val, alignment) ((((val) / (alignment)) + ((val) % (alignment) == 0 ? 0 : 1)) * (alignment))

#define BYTE_SHIFT(val, byteshift) (((uintptr_t)(val)) << ((byteshift)*8))

#define PTR_MOD(ptr, mod) (((uint64_t)(ptr)) % (mod))

static inline uint32_t ROL_32(const uint32_t val, const uint8_t n) {
    // Source: This blog post explains the safety/performance tradeoff of rol32 and describes the best option
    // https://blog.regehr.org/archives/1063
    ASSERT(0 < n && n < 32);
    return (val << n) | (val >> (-n & 31));
}

#endif /* CCONTAIN_COMMON_H */
