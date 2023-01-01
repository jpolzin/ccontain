#include "common.h"
#include "vec.h"

#include <stdlib.h>

#define INIT_PARAMS_DEFAULT(params_ptr) {\
    (params_ptr)->initial_size = 0;\
    (params_ptr)->allocate_func = malloc;\
    (params_ptr)->free_func = free;\
    (params_ptr)->el_bytes = 1;\
}

static void create_destroy() {
    vec_params_t params = {0};
    INIT_PARAMS_DEFAULT(&params);

    vec_t * vec = vec_create(&params);
    ASSERT(vec != NULL);
    vec_destroy(vec);
}

int main() {
    create_destroy();
    return 0;
}
