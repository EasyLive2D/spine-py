add_executable(spine-opengl 
    ${PROJECT_ROOT}/spine-opengl/main.c
    ${PROJECT_ROOT}/spine-opengl/spine-opengl.c
)

target_include_directories(spine-opengl
    PRIVATE
    ${PROJECT_ROOT}/spine-c/spine-c/include
    ${PROJECT_ROOT}/Glad/include
    ${PROJECT_ROOT}/GLFW/include
)

target_link_libraries(spine-opengl PRIVATE
    glad
    ${PROJECT_ROOT}/GLFW/lib/glfw3.lib
    spine-c
)

set_target_properties(spine-opengl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT}/X64/Release
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT}/X64/Debug
)
