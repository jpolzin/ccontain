add_subdirectory(vec)

add_library(ccontain SHARED $<TARGET_OBJECTS:ccontain-vec>)

