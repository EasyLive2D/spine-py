add_library(spine-opengl STATIC
    ${PROJECT_ROOT}/spine-opengl/src/spine-opengl.c
)

target_include_directories(spine-opengl
    PRIVATE
    ${PROJECT_ROOT}/spine-c/spine-c/include
    ${PROJECT_ROOT}/spine-opengl/include
    ${PROJECT_ROOT}/glad/include
)

target_link_libraries(spine-opengl PRIVATE
    glad
    spine-c
)


