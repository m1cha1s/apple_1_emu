project(apple_1_emu)

add_library(apple_1_emu
    apple_1_emu.c
    fake6502.c
)

target_include_directories(apple_1_emu PUBLIC ${CMAKE_CURRENT_LIST_DIR})