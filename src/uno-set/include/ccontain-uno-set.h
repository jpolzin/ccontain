#include "ccontain-common.h"

#include <stdint.h>

#define DEFINE_UNO_SET_RETURN(rettype)\
    typedef struct uno_set_return_##rettype_s {\
        ccontain_err_t err;\
        rettype_t retval;\
    } uno_set_return_##rettype_t;


typedef void *uno_set_priv_t;

/*
Possible params:
- Initial capacity
- Allocator
- Hashing algorithm
*/
uno_set_t * create_uno_set(parameters);

ccontain_err_t destroy_uno_set(uno_set_t *self);

// Copyable
typedef struct uno_set_key_s {
    void *key_ptr;
    size_t keylen;
} uno_set_key_t;


typedef struct uno_set_it_s uno_set_it_t;

// Copyable
typedef struct uno_set_it_s {
    uno_set_key_t (*deref)(uno_set_it_t *self); // Returns key value
    void advance(uno_set_it_t *self); // Advances iterator
    bool end(uno_set_it_t *self); // True if iterator is at end
    uno_set_it_priv_t priv; // Pointer to separately allocated priv
} uno_set_it_t;

// Copyable
typedef struct uno_set_s {
    ccontain_err_t (*find)(const uno_set_t *self, const uno_set_key_t key,
        uno_set_it_t *iterator /* out */);
    ccontain_err_t (*exists)(const uno_set_t *self, bool *exists /* out */);
    ccontain_err_t (*insert)(uno_set_t *self, const uno_set_key_t key,
        uno_set_it_t *iterator /* out */);
    ccontain_err_t (*insert_if_new)(uno_set_t *self, const uno_set_key_t key,
        uno_set_it_t *iterator /* out */);
    ccontain_err_t (*size)(const uno_set_t *self, size_t *size /* out */)
    ccontain_err_t (*capacity)(const uno_set_t *self, size_t *capacity /* out */);
    ccontain_err_t (*reserve)(uno_set_t *self, size_t capacity);
    uno_set_priv_t priv; // Pointer to separately allocated priv
} uno_set_t;


