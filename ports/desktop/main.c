#include <stdio.h>
#include <stdint.h>
#include <apple_1_emu.h>

uint8_t readkey() {
    return getchar();
}

void writechar(uint8_t ch) {
    printf("%c", ch);
}

uint8_t keyavailable() {
    char b;
    return fgets(&b, 1, stdin) != NULL;
}

int main() {
    apple_1_init();

    for(;;)
        apple_1_step();

    return 0;
}