cmake_minimum_required(VERSION 3.16)

project(alu_8bit_exam LANGUAGES CXX)

# Match Makefile: -std=c++17 -Wall
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall)

# Match Makefile defaults:
#   -I/usr/local/systemc/include
#   -L/usr/local/systemc/lib -lsystemc -pthread -Wl,-rpath,/usr/local/systemc/lib
set(SYSTEMC_ROOT "/usr/local/systemc" CACHE PATH "SystemC root path")

find_path(SYSTEMC_INCLUDE_DIR
    NAMES systemc.h
    PATHS
        "${SYSTEMC_ROOT}/include"
    NO_DEFAULT_PATH
)

find_library(SYSTEMC_LIBRARY
    NAMES systemc
    PATHS
        "${SYSTEMC_ROOT}/lib"
        "${SYSTEMC_ROOT}/lib64"
    NO_DEFAULT_PATH
)

if(NOT SYSTEMC_INCLUDE_DIR OR NOT SYSTEMC_LIBRARY)
    message(FATAL_ERROR
        "Cannot find SystemC under SYSTEMC_ROOT=${SYSTEMC_ROOT}.\n"
        "Please configure with -DSYSTEMC_ROOT=/path/to/systemc"
    )
endif()

add_executable(alu_8bit_exam_tb
    alu_8bit_exam.cpp
    alu_8bit_exam_tb.cpp
)

target_include_directories(alu_8bit_exam_tb PRIVATE ${SYSTEMC_INCLUDE_DIR})
target_link_libraries(alu_8bit_exam_tb PRIVATE ${SYSTEMC_LIBRARY} pthread)

# Equivalent to Makefile rpath behavior
set_target_properties(alu_8bit_exam_tb PROPERTIES
    BUILD_RPATH "${SYSTEMC_ROOT}/lib"
    INSTALL_RPATH "${SYSTEMC_ROOT}/lib"
)

# Optional convenience target: cmake --build <build_dir> --target run
add_custom_target(run
    COMMAND alu_8bit_exam_tb
    DEPENDS alu_8bit_exam_tb
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
)
