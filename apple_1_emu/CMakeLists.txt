project(apple_1_emu)

add_library(apple_1_emu
    apple_1_emu.c
)

target_include_directories(apple_1_emu PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(apple_1_emu PUBLIC perfect6502)