cmake_minimum_required(VERSION 3.26)

project(spine-wrapper)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/glad.cmake)
include(cmake/spine-c.cmake)
include(cmake/spine-opengl.cmake)
include(cmake/wrapper.cmake)
include(cmake/c-example.cmake)

if(WIN32)
    add_custom_command(
        TARGET spine-py
        POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E copy $<TARGET_FILE:spine-py> ${PROJECT_ROOT}/package/spine/spine.pyd   
    )
endif()

