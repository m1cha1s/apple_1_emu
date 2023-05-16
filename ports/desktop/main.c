#include <stdio.h>
#include <apple_1_emu.h>

int main() {
    apple_1_init();

    for(;;)
        apple_1_step();

    return 0;
}