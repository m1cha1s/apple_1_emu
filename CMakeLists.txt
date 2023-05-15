cmake_minimum_required(VERSION 3.16)

project(perfect6502)

add_library(perfect6502
    ${CMAKE_CURRENT_LIST_DIR}/perfect6502/netlist_sim.h
    ${CMAKE_CURRENT_LIST_DIR}/perfect6502/netlist_sim.c
    ${CMAKE_CURRENT_LIST_DIR}/perfect6502/perfect6502.h
    ${CMAKE_CURRENT_LIST_DIR}/perfect6502/perfect6502.c
)

target_include_directories(perfect6502 PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/perfect6502/
)

add_subdirectory(apple_1_emu)
add_subdirectory(ports)
