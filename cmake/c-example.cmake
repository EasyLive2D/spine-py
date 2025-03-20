add_executable(glfw-example
    ${PROJECT_ROOT}/c-example/main.c
)

target_include_directories(glfw-example
    PRIVATE
    ${PROJECT_ROOT}/spine-c/spine-c/include
    ${PROJECT_ROOT}/spine-opengl/include
    ${PROJECT_ROOT}/Glad/include
    ${PROJECT_ROOT}/GLFW/include
)

target_link_options(glfw-example PRIVATE 
    $<IF:$<CONFIG:Release>,/NODEFAULTLIB:MSVCRTD,>
)

target_link_libraries(glfw-example 
    ${PROJECT_ROOT}/GLFW/lib/glfw3.lib
    spine-opengl
)

set_target_properties(glfw-example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT}/X64/Release
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT}/X64/Debug
)