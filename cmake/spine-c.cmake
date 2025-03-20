aux_source_directory(
    ${PROJECT_ROOT}/spine-c/spine-c/src/spine

    SPINE_C_SRC
)

add_library(spine-c STATIC
    ${SPINE_C_SRC}
)

target_include_directories(spine-c
    PRIVATE
    ${PROJECT_ROOT}/spine-c/spine-c/include
)