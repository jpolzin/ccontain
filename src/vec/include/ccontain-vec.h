

#ifndef CCONTAIN_VEC_H
#define CCONTAIN_VEC_H

#include <stdint.h>
#include <stdlib.h>

#include "ccontain-err.h"

typedef void *vec_priv_t;

typedef void * vec_el_ptr_t;

typedef struct vec_params_s {
    size_t initial_size; // Initial number of elements
    size_t el_bytes; // Number of bytes per element
    void * (*allocate_func)(const size_t n_bytes); // Memory allocator
    void (*free_func)(void *ptr); // Memory free-er
} vec_params_t;

typedef struct vec_s vec_t;

typedef struct vec_s {
    ccontain_err_t (*append) (vec_t *self, const vec_el_ptr_t el);
    vec_el_ptr_t (*at) (vec_t *self, const size_t idx);
    ccontain_err_t (*insert) (vec_t *self, const vec_el_ptr_t el_ptr, const size_t idx);
    ccontain_err_t (*remove) (vec_t *self, const size_t idx);
    size_t (*size) (vec_t *self);
    ccontain_err_t (*reserve) (vec_t *self, const size_t new_capacity);
    ccontain_err_t (*resize) (vec_t *self, const size_t new_size);
    vec_priv_t priv;
} vec_t;

vec_t *vec_create(const vec_params_t *params);
void vec_destroy(vec_t *self);

#endif /* CCONTAIN_VEC_H */
