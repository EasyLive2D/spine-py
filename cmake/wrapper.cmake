if(DEFINED MY_PYTHON)
    set(CMAKE_PREFIX_PATH ${MY_PYTHON})
else()
    set(CMAKE_PREFIX_PATH D:/Python/x64/3.12)
    message("MY_PYTHON: ${CMAKE_PREFIX_PATH}")
endif()

find_package(Python3 REQUIRED COMPONENTS Development.SABIModule)

aux_source_directory(${PROJECT_ROOT}/wrapper/src WRAPPER_SRC)
add_library(spine-py SHARED 
    ${WRAPPER_SRC}
)

target_include_directories(spine-py PRIVATE 
    ${PROJECT_ROOT}/spine-c/spine-c/include
    ${PROJECT_ROOT}/glad/include
    ${PROJECT_ROOT}/spine-opengl/include
    ${PROJECT_ROOT}/wrapper/include
)

target_link_libraries(spine-py glad spine-c spine-opengl Python3::SABIModule)