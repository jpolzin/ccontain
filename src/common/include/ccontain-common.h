#ifndef CCONTAIN_COMMON_H
#define CCONTAIN_COMMON_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define ASSERT(cond) if (!(cond)) {\
    printf("Assertion failed in %s on line %zu. Condition: %s\n", __FILE__, (size_t)__LINE__, #cond);\
    exit (1);\
}

static inline void * assert_alloc(const size_t sz, void * (*allocator)(size_t sz)) {
    void * __ptr = allocator((sz));
    ASSERT(__ptr != NULL);
    return __ptr;
}

#ifdef DEBUG
#define ALLOC(sz) assert_alloc(sz, malloc)
#else
#define ALLOC(sz) malloc((sz))
#endif

#define ALIGN_UP(val, alignment) ((((val) / (alignment)) + ((val) % (alignment) == 0 ? 0 : 1)) * (alignment))

#define FREE(ptr) free(ptr)

#endif /* CCONTAIN_COMMON_H */
