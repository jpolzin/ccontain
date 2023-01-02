#include "ccontain-common.h"
#include "ccontain-vec.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PASSED() printf("Passed %s\n", __func__);

#define INIT_PARAMS_DEFAULT(params_ptr) {\
    (params_ptr)->initial_size = 0;\
    (params_ptr)->allocate_func = malloc;\
    (params_ptr)->free_func = free;\
    (params_ptr)->el_bytes = 1;\
}

static void create_destroy() {
    ccontain_vec_params_t params = {0};
    INIT_PARAMS_DEFAULT(&params);

    ccontain_vec_t * vec = ccontain_vec_create(&params);
    ASSERT(vec != NULL);
    ccontain_vec_destroy(vec);
    PASSED();
}



static void size_sweep() {
    const size_t max_el_size = 128;
    const size_t max_len = 50;
    ccontain_vec_params_t params = {0};
    size_t i = 0;
    size_t el_size = 1;
    ccontain_vec_t * vec = NULL;

    uint8_t *tmp, *tmp_end;
    uint8_t *buf;

    buf = ALLOC(max_el_size);

    for (; el_size < max_el_size; ++el_size) {
        INIT_PARAMS_DEFAULT(&params);
        params.el_bytes = el_size;
        vec = ccontain_vec_create(&params);
        ASSERT(vec != NULL);

        // Set each byte in the buf equal to i
        for (i = 0; i < max_len; ++i) {
            memset(buf, i, el_size);
            ASSERT(vec->append(vec, buf) == CCONTAIN_SUCCESS);
            ASSERT(vec->size(vec) == i + 1);
        }

        // Look at each byte to make sure it matches i
        for (i = 0; i < max_len; ++i) {
            tmp = vec->at(vec, i);
            tmp_end = tmp + el_size;
            for (; tmp < tmp_end; ++tmp) {
                ASSERT(*tmp == i)
            }
        }

        ccontain_vec_destroy(vec);
    }

    FREE(buf);
    PASSED();
}

int main() {
    create_destroy();
    size_sweep();
    return 0;
}
