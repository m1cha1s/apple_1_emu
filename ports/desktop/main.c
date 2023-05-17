#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <apple_1_emu.h>

// FIXME: This only works on windows for now.

#if _WIN32
#include <conio.h>
#endif

int cursor = 0;

uint8_t readkey() {
    #if _WIN32
    return _getch();
    #endif
}

void writechar(uint8_t ch) {
    cursor++;
    if (cursor >= 40 || ch == '\n') {
        cursor = 0; 
        if (cursor >= 40)
            printf("\n");
    }
    printf("%c", ch);
}

uint8_t keyavailable() {
    #if _WIN32
    return _kbhit();
    #endif
}

int main() {


    apple_1_init();

    for(;;)
        apple_1_step();

    return 0;
}