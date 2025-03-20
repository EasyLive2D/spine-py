if(DEFINED MY_PYTHON)
    set(CMAKE_PREFIX_PATH ${MY_PYTHON})
else()
    set(CMAKE_PREFIX_PATH D:/Python/x64/3.12)
    message("MY_PYTHON: ${CMAKE_PREFIX_PATH}")
endif()

find_package(Python3 REQUIRED COMPONENTS Development.SABIModule)

add_library(spine-py SHARED 
    ${PROJECT_ROOT}/Wrapper/spine-py.cpp
)

target_include_directories(spine-py PRIVATE 
    ${PROJECT_ROOT}/spine-c/spine-c/include
    ${PROJECT_ROOT}/Glad/include
    ${PROJECT_ROOT}/spine-opengl/include
)

target_link_libraries(spine-py glad spine-c spine-opengl)