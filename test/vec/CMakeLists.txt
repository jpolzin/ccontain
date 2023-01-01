add_executable(ccontain-vec-test 
               vec-test.c
               $<TARGET_OBJECTS:ccontain-vec>
)

target_include_directories(ccontain-vec-test PRIVATE
    ${VEC_INCLUDE}
    ${COMMON_INCLUDE}  
)
