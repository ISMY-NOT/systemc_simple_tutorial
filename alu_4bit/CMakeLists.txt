cmake_minimum_required(VERSION 3.16)
project(alu_4bit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall)

# Match the existing Makefile defaults.
set(SYSTEMC_ROOT "/usr/local/systemc" CACHE PATH "SystemC root path")
set(SYSTEMC_INCLUDE_DIR "${SYSTEMC_ROOT}/include")
set(SYSTEMC_LIBRARY_DIR "${SYSTEMC_ROOT}/lib")
set(VCD_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/../vcd" CACHE PATH "VCD output directory")

add_executable(alu_4bit_tb
    alu_4bit_tb.cpp
)

target_include_directories(alu_4bit_tb PRIVATE ${SYSTEMC_INCLUDE_DIR})
target_link_directories(alu_4bit_tb PRIVATE ${SYSTEMC_LIBRARY_DIR})
target_link_libraries(alu_4bit_tb systemc)

set_target_properties(alu_4bit_tb PROPERTIES
    BUILD_RPATH "${SYSTEMC_LIBRARY_DIR}"
    INSTALL_RPATH "${SYSTEMC_LIBRARY_DIR}"
)

add_custom_target(run
    COMMAND alu_4bit_tb
    DEPENDS alu_4bit_tb
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
)
