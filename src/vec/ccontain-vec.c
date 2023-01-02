#include "ccontain-vec.h"

#include <stdlib.h>
#include <string.h>

#include "ccontain-err.h"
#include "ccontain-common.h"

typedef struct vec_ctx_s {
    void *mem;
    size_t el_bytes;
    size_t el_bytes_step;
    size_t capacity;
    size_t size;
    void * (*allocate_func)(const size_t n_bytes);
    void (*free_func)(void *ptr);
} vec_ctx_t;

#define VEC_CTX(vec) ((vec_ctx_t *)((vec)->priv))
#define VEC_ALLOC(vec_ctx, sz) assert_alloc(sz, vec_ctx->allocate_func)
#define VEC_FREE(vec_ctx, ptr) (vec_ctx->free_func((ptr)))

#define VEC_SIZE_BYTES(vec_ctx) ((vec_ctx)->size * (vec_ctx)->el_bytes_step)
#define VEC_CAPACITY_BYTES(vec_ctx) ((vec_ctx)->capacity * (vec_ctx)->el_bytes_step)


#define STEP_SMALL 2
#define STEP_MED sizeof(int)
#define STEP_LONG sizeof(long long)

/**
 * @brief      Appends a value, resizing or reallocating if needed.
 *
 * @param      self    The object
 * @param[in]  el_ptr  Pointer to value to append
 *
 * @return     The ccontain error.
 */
static ccontain_err_t vec_append(vec_t *self, const vec_el_ptr_t el_ptr);

/**
 * @brief      Returns a pointer to the value at idx. Does not check the validity of input parameters.
 *
 * @param      self  The object
 * @param[in]  idx   Index to access
 *
 * @return     Pointer to the element in the vector.
 */
static vec_el_ptr_t vec_at(vec_t *self, const size_t idx);

/**
 * @brief      Inserts a value at idx, shifting subsequent elements up one.
 *
 * @param      self    The object
 * @param[in]  el_ptr  Pointer to element to insert
 * @param[in]  idx     Index to insert element at
 *
 * @return     The ccontain error.
 */
static ccontain_err_t vec_insert(vec_t *self, const vec_el_ptr_t el_ptr, const size_t idx);

/**
 * @brief      Removes the value at idx, shifting subsequent elements back one.
 *
 * @param      self  The object
 * @param[in]  idx   Index to remove element from
 *
 * @return     The ccontain error.
 */
static ccontain_err_t vec_remove(vec_t *self, const size_t idx);

static size_t vec_size(vec_t *self);

/**
 * @brief      Reserves memory for at least new_capacity elements. Size is unaffected.
 * @details    If current_capacity >= new_capacity, then the vector is unaffected.
 *
 * @param      self          The object
 * @param[in]  new_capacity  Number of elements to reserve memory for
 *
 * @return     The ccontain error.
 */
static ccontain_err_t vec_reserve(vec_t *self, const size_t new_capacity);

/**
 * @brief      Resizes the vector to contain new_size elements. If new_size is larger than the current size, the new values are not initialized. If it is smaller than the current size, the last elements are truncated.
 *
 * @param      self      The object
 * @param[in]  new_size  The new size
 *
 * @return     The ccontain error.
 */
static ccontain_err_t vec_resize(vec_t *self, const size_t new_size);

static size_t step_align(const size_t el_bytes) {
    if (el_bytes <= STEP_SMALL) {
        return el_bytes;
    } else if (el_bytes <= STEP_MED) {
        return STEP_MED;
    }

    // Operate on long-long alignment for any big element size
    return ALIGN_UP(el_bytes, STEP_LONG);
}

static inline ccontain_err_t _vec_reserve(vec_t *self, const size_t new_capacity) {
    void * tmp;
    vec_ctx_t *vec_ctx = VEC_CTX(self);

    if (vec_ctx->capacity < new_capacity) {
        if (vec_ctx->capacity == 0) {
            vec_ctx->capacity = 1;
        }

        while (vec_ctx->capacity < new_capacity) {
            vec_ctx->capacity *= 2;
        } 

        tmp = VEC_ALLOC(vec_ctx, VEC_CAPACITY_BYTES(vec_ctx));
        CCONTAIN_NULL(vec_ctx->mem, {
            return CCONTAIN_EALLOC;
        });

        if (vec_ctx->mem != NULL) {
            memcpy(tmp, vec_ctx->mem, VEC_SIZE_BYTES(vec_ctx));
            free(vec_ctx->mem);
        }
        vec_ctx->mem = tmp;
        vec_ctx->capacity = new_capacity;
    }
    
    return CCONTAIN_SUCCESS;
}

static inline ccontain_err_t _vec_resize(vec_t *self, const size_t new_size) {
    void * tmp = NULL;
    vec_ctx_t *vec_ctx = VEC_CTX(self);

    _vec_reserve(self, new_size);

    vec_ctx->size = new_size;

    return CCONTAIN_SUCCESS;
}

vec_t *vec_create(const vec_params_t *params) {
    vec_t *vec = NULL;
    vec_ctx_t *vec_ctx = NULL;

    vec = ALLOC(sizeof(vec_t));
    CCONTAIN_NULL_GOTO(vec_ctx, end);

    vec->append = vec_append;
    vec->at = vec_at;
    vec->insert = vec_insert;
    vec->remove = vec_remove;
    vec->size = vec_size;
    vec->reserve = vec_reserve;
    vec->resize = vec_resize;

    vec_ctx = ALLOC(sizeof(vec_ctx_t));
    CCONTAIN_NULL_GOTO(vec_ctx, cleanup_vec);

    vec_ctx->el_bytes = params->el_bytes;
    vec_ctx->el_bytes_step = step_align(params->el_bytes);
    vec_ctx->size = params->initial_size;
    vec_ctx->capacity = params->initial_size;
    vec_ctx->allocate_func = params->allocate_func;
    vec_ctx->free_func = params->free_func;
    vec_ctx->mem = NULL;

    vec->priv = vec_ctx;

    CCONTAIN_STATUS_GOTO(_vec_resize(vec, vec_ctx->size), cleanup_ctx);

    return (vec_t *)vec;

cleanup_ctx:
    free(vec_ctx);
cleanup_vec:
    free(vec);
end:
    return NULL;
}

void vec_destroy(vec_t *self) {
    vec_ctx_t *vec_ctx = VEC_CTX(self);
    VEC_FREE(vec_ctx, vec_ctx->mem);
    FREE(vec_ctx);
    FREE(self);
}

static inline vec_el_ptr_t _vec_at(vec_t *self, const size_t idx) {
    return (vec_el_ptr_t)((uintptr_t)VEC_CTX(self)->mem + idx * VEC_CTX(self)->el_bytes_step);
}

static ccontain_err_t vec_append(vec_t *self, const vec_el_ptr_t el_ptr) {
    vec_ctx_t *vec_ctx = VEC_CTX(self);
    CCONTAIN_STATUS_RETURN(_vec_resize(self, vec_ctx->size + 1), {});

    memcpy(_vec_at(self, vec_ctx->size - 1), el_ptr, vec_ctx->el_bytes);
    return CCONTAIN_SUCCESS;
}

vec_el_ptr_t vec_at(vec_t *self, const size_t idx) {
    return _vec_at(self, idx);
}

static ccontain_err_t vec_insert(vec_t *self, const vec_el_ptr_t el_ptr, const size_t idx) {
    vec_ctx_t *vec_ctx = VEC_CTX(self);
    size_t i = idx;
    CCONTAIN_STATUS_RETURN(_vec_resize(self, vec_ctx->size + 1), {});

    // TODO: Optimize
    for (; i < vec_ctx->size - 1; ++i) {
        memcpy(_vec_at(self, i + 1), _vec_at(self, i), vec_ctx->el_bytes);
    }

    memcpy(_vec_at(self, idx), el_ptr, vec_ctx->el_bytes);
    return CCONTAIN_SUCCESS;
}

static ccontain_err_t vec_remove(vec_t *self, const size_t idx) {
    vec_ctx_t *vec_ctx = VEC_CTX(self);
    size_t i = idx;

    // TODO: Optimize
    for (; i < vec_ctx->size - 1; ++i) {
        memcpy(_vec_at(self, i), _vec_at(self, i + 1), vec_ctx->el_bytes);
    }
    CCONTAIN_STATUS_RETURN(_vec_resize(self, vec_ctx->size), {});
    return CCONTAIN_SUCCESS;
}

static size_t vec_size(vec_t *self) {
    return VEC_CTX(self)->size;
}

static ccontain_err_t vec_reserve(vec_t *self, const size_t new_capacity) {
    return _vec_reserve(self, new_capacity);
}

static ccontain_err_t vec_resize(vec_t *self, const size_t new_size) {
    return _vec_resize(self, new_size);
}
