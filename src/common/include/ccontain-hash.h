#include <stdint.h>
#include <stdlib.h>

typedef uint64_t (*ccontain_hash_func_t)(const void *, const size_t);

#define CCONTAIN_HASH_FUNC(__name, __bytes, __n_bytes) uint64_t __name(const void * __bytes, const size_t __n_bytes)

typedef enum ccontain_hash_e {
    HASH_MURMUR,
    HASH_SIMPLE,
    HASH_MAX
} ccontain_hash_t;


ccontain_hash_func_t ccontain_get_hash(const ccontain_hash_t hash);
