#ifndef _APPLE_1_EMU
#define _APPLE_1_EMU

#include <perfect6502.h>

typedef struct {
    state_t* state;
    int clk;
} apple_1_state;

apple_1_state apple_1_init();
void apple_1_step(apple_1_state* state);

#endif