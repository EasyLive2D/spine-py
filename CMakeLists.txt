cmake_minimum_required(VERSION 3.16)

project(spine-wrapper)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/glad.cmake)
include(cmake/spine-c.cmake)
include(cmake/spine-opengl.cmake)