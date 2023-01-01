add_library(ccontain-vec OBJECT vec.c)

target_include_directories(ccontain-vec PUBLIC 
                           include
                           ${SRC_DIR}/common/include)
