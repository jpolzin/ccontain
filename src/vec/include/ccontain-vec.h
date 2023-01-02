

#ifndef CCONTAIN_VEC_H
#define CCONTAIN_VEC_H

#include <stdint.h>
#include <stdlib.h>

#include "ccontain-err.h"

typedef void *ccontain_vec_priv_t;

typedef void * ccontain_vec_el_ptr_t;

typedef struct ccontain_vec_params_s {
    size_t initial_size; // Initial number of elements
    size_t el_bytes; // Number of bytes per element
    void * (*allocate_func)(const size_t n_bytes); // Memory allocator
    void (*free_func)(void *ptr); // Memory free-er
} ccontain_vec_params_t;

typedef struct ccontain_vec_s ccontain_vec_t;

typedef struct ccontain_vec_s {
    ccontain_err_t (*append) (ccontain_vec_t *self, const ccontain_vec_el_ptr_t el);
    ccontain_vec_el_ptr_t (*at) (ccontain_vec_t *self, const size_t idx);
    ccontain_err_t (*insert) (ccontain_vec_t *self, const ccontain_vec_el_ptr_t el_ptr, const size_t idx);
    ccontain_err_t (*remove) (ccontain_vec_t *self, const size_t idx);
    size_t (*size) (ccontain_vec_t *self);
    ccontain_err_t (*reserve) (ccontain_vec_t *self, const size_t new_capacity);
    ccontain_err_t (*resize) (ccontain_vec_t *self, const size_t new_size);
    ccontain_vec_priv_t priv;
} ccontain_vec_t;

ccontain_vec_t *ccontain_vec_create(const ccontain_vec_params_t *params);
void ccontain_vec_destroy(ccontain_vec_t *self);

#endif /* CCONTAIN_VEC_H */
