# CContain
CContain is a C library that aims to implement standard data structures with similar performance and flexibility to the C++ STL.

## Philosophies
1. Efficiency: CContain APIs should be as low overhead as possible, both in terms of memory and performance.
2. Simplicity: CContain will provide the bare minimum to implement the data structure. Any "helper" interfaces are the burden of the client, unless it is dramatically more efficient for CContain to implement them.
3. Flexibility: CContain should make few assumptions about the platform or application. Instead, the interfaces should enable developers to provide the optimal parameters for their own applications.

## Style
### File name format
All C source files should follow the format `ccontain-<file name>.<c or h>`. Use dashes, not underscores, to delimit words.

### Data structures
External data structure types should be typedef'ed and  follow `ccontain_<module>_<extra>_t`.
Example: `ccontain_vec_params_t`


### Interfaces
External function interfaces should follow the format `ccontain_<module>_<function>()`.
Example: `ccontain_vec_create()`

Primary data structures (e.g. module interfaces) must implement a struct with function pointers to any members and one opaque handle to a private structure. While this does prevent inlining, it allows CContain to choose different implementations for each member depending on the initial parameters.  
They should be named `ccontain_<module>_t`. Members do not need to be namespaced.
Example:
```
typedef struct ccontain_vec_s {
    ccontain_err_t (*append) (vec_t *self, const vec_el_ptr_t el);
    vec_el_ptr_t (*at) (vec_t *self, const size_t idx);
    ccontain_err_t (*insert) (vec_t *self, const vec_el_ptr_t el_ptr, const size_t idx);
    ccontain_err_t (*remove) (vec_t *self, const size_t idx);
    size_t (*size) (vec_t *self);
    ccontain_err_t (*reserve) (vec_t *self, const size_t new_capacity);
    ccontain_err_t (*resize) (vec_t *self, const size_t new_size);
    ccontain_vec_priv_t priv;
} ccontain_vec_t;
```

Use `const` whenever applicable in interfaces.

TODO: Land on a rule for interfaces that return an error code vs ones that don't

### Extern
Do not use extern variables unless they are `const`.

### Header guards
Header guards are the same as the filename but in all caps and with any periods or dashes replaced with underscores.

### C source
C code style is fairly flexible, but be sure to follow these rules.
1. Only use fixed-width integers and size_t unless required by a third-party interface.
2. Use descriptive variable names, and don't over or under-abbreviate.
3. Functions defined in the C file must be `static`.
4. Use `const` vigilantly.
5. Keep all functions to <=6 arguments. If there are more, use a `struct`.

## Questions
### Why not just link C code to C++ and use the C++ STL?
For the same reasons that some projects use C over C++. Including any C++ STL in your project requires that the platform support that library, which may not be possible. Embedded systems and kernel development are notable examples.

