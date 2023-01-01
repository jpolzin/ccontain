#include <assert.h>
#include <stdlib.h>

#define ASSERT(cond) assert

#define ASSERT_ALLOC(sz, allocator) \
    {\
        void * __ptr = allocator((sz));\
        ASSERT(__ptr != NULL);\
        __ptr;\
    } 

#ifdef DEBUG
#define ALLOC(sz) \
    ASSERT_ALLOC(sz, malloc)
#else
#define ALLOC(sz) malloc((sz))
#endif

#define ALIGN_UP(val, alignment) ((((val) / (alignment)) + ((val) % (alignment) == 0 ? 0 : 1)) * (alignment))

#define FREE(ptr) free(ptr)