#ifndef CCONTAIN_ERR_H
#define CCONTAIN_ERR_H

#ifdef DEBUG
#define CCONTAIN_COND(cond, code) \
    if ((cond)) {\
        { code }\
    }
#else
#define CCONTAIN_COND(cond, code) \
    (void)(cond)
#endif

#define CCONTAIN_STATUS(rc, code) \
    CCONTAIN_COND((rc) != CCONTAIN_SUCCESS, code)

#define CCONTAIN_STATUS_RETURN(rc, code) \
    CCONTAIN_STATUS(rc, {\
        { code }\
        return (rc);\
    })

#define CCONTAIN_STATUS_GOTO(rc, label) \
    CCONTAIN_STATUS(rc, { goto label; }) 

#define CCONTAIN_NULL(ptr, code) \
    CCONTAIN_COND((ptr) == NULL, code)

#define CCONTAIN_NULL_GOTO(ptr, label) \
    CCONTAIN_COND((ptr) == NULL, { goto label; })

typedef enum ccontain_err_s {
    CCONTAIN_SUCCESS,
    CCONTAIN_DUPLICATE,
    CCONTAIN_NOTFOUND,
    CCONTAIN_EALLOC,
    CCONTAIN_EINVAL,
    CCONTAIN_EFAILURE,
} ccontain_err_t;

#endif /* CCONTAIN_ERR_H */