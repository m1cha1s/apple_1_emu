#include "apple_1_emu.h"

#if _WIN32
#include <io.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <perfect6502.h>

// unsigned char A, X, Y, S, P;
// unsigned short PC;
// int N, Z, C;

const unsigned char wozmon[256] = {
    0xD8, 0x58, 0xA0, 0x7F, 0x8C, 0x12, 0xD0, 0xA9, 
    0xA7, 0x8D, 0x11, 0xD0, 0x8D, 0x13, 0xD0, 0xC9, 
    0xDF, 0xF0, 0x13, 0xC9, 0x9B, 0xF0, 0x03, 0xC8, 
    0x10, 0x0F, 0xA9, 0xDC, 0x20, 0xEF, 0xFF, 0xA9, 
    0x8D, 0x20, 0xEF, 0xFF, 0xA0, 0x01, 0x88, 0x30, 
    0xF6, 0xAD, 0x11, 0xD0, 0x10, 0xFB, 0xAD, 0x10, 
    0xD0, 0x99, 0x00, 0x02, 0x20, 0xEF, 0xFF, 0xC9, 
    0x8D, 0xD0, 0xD4, 0xA0, 0xFF, 0xA9, 0x00, 0xAA, 
    0x0A, 0x85, 0x2B, 0xC8, 0xB9, 0x00, 0x02, 0xC9, 
    0x8D, 0xF0, 0xD4, 0xC9, 0xAE, 0x90, 0xF4, 0xF0, 
    0xF0, 0xC9, 0xBA, 0xF0, 0xEB, 0xC9, 0xD2, 0xF0, 
    0x3B, 0x86, 0x28, 0x86, 0x29, 0x84, 0x2A, 0xB9, 
    0x00, 0x02, 0x49, 0xB0, 0xC9, 0x0A, 0x90, 0x06, 
    0x69, 0x88, 0xC9, 0xFA, 0x90, 0x11, 0x0A, 0x0A, 
    0x0A, 0x0A, 0xA2, 0x04, 0x0A, 0x26, 0x28, 0x26, 
    0x29, 0xCA, 0xD0, 0xF8, 0xC8, 0xD0, 0xE0, 0xC4, 
    0x2A, 0xF0, 0x97, 0x24, 0x2B, 0x50, 0x10, 0xA5, 
    0x28, 0x81, 0x26, 0xE6, 0x26, 0xD0, 0xB5, 0xE6, 
    0x27, 0x4C, 0x44, 0xFF, 0x6C, 0x24, 0x00, 0x30, 
    0x2B, 0xA2, 0x02, 0xB5, 0x27, 0x95, 0x25, 0x95, 
    0x23, 0xCA, 0xD0, 0xF7, 0xD0, 0x14, 0xA9, 0x8D, 
    0x20, 0xEF, 0xFF, 0xA5, 0x25, 0x20, 0xDC, 0xFF, 
    0xA5, 0x24, 0x20, 0xDC, 0xFF, 0xA9, 0xBA, 0x20, 
    0xEF, 0xFF, 0xA9, 0xA0, 0x20, 0xEF, 0xFF, 0xA1, 
    0x24, 0x20, 0xDC, 0xFF, 0x86, 0x2B, 0xA5, 0x24, 
    0xC5, 0x28, 0xA5, 0x25, 0xE5, 0x29, 0xB0, 0xC1, 
    0xE6, 0x24, 0xD0, 0x02, 0xE6, 0x25, 0xA5, 0x24, 
    0x29, 0x07, 0x10, 0xC8, 0x48, 0x4A, 0x4A, 0x4A, 
    0x4A, 0x20, 0xE5, 0xFF, 0x68, 0x29, 0x0F, 0x09, 
    0xB0, 0xC9, 0xBA, 0x90, 0x02, 0x69, 0x06, 0x2C, 
    0x12, 0xD0, 0x30, 0xFB, 0x8D, 0x12, 0xD0, 0x60, 
    0x00, 0x00, 0x00, 0x0F, 0x00, 0xFF, 0x00, 0x00, 
};

static void charout(state_t* state, char ch) {
	unsigned char S = readSP(state);
	unsigned short a = 1 + memory[0x0100+S+1] | memory[0x0100+((S+2) & 0xFF)] << 8;

	/*
	 * Apple I BASIC prints every character received
	 * from the terminal. UNIX terminals do this
	 * anyway, so we have to avoid printing every
	 * line again
	 */
	if (a==0xe2a6)	/* character echo */
		return;
	if (a==0xe2b6)	/* CR echo */
		return;

	/*
	 * Apple I BASIC prints a line break and 6 spaces
	 * after every 37 characters. UNIX terminals do
	 * line breaks themselves, so ignore these
	 * characters
	 */
	if (a==0xe025 && (ch==10 || ch==' '))
		return;

	/* INPUT */
	if (a==0xe182) {
#if _WIN32
		if (!_isatty(0))
			return;
#else
		struct stat st;
		fstat(0, &st);
		if (S_ISFIFO (st.st_mode))
			return;
#endif
	}

	putc(ch, stdout);
	fflush(stdout);
}

static void handle_monitor(state_t* state)
{
	if (readRW(state)) {
		unsigned short a = readAddressBus(state);
		if (a == 0xD010) {
			unsigned char c = getchar();
			if (c == 10)
				c = 13;
			c |= 0x80;
			writeDataBus(state, c);
		}
		if (a == 0xD011) {
            char b;
			if (fgets(&b, 1, stdin) != NULL)
				/* if the code is reading a character, we have one ready */
				writeDataBus(state, 0x80);
			else
				/* if the code checks for a STOP condition, nothing is pressed */
				writeDataBus(state, 0);
		}
		if (a == 0xD012) {
			/* 0x80 would mean we're not yet ready to receive a character */
			writeDataBus(state, 0);
		}
	} else {
		unsigned short a = readAddressBus(state);
		unsigned char d = readDataBus(state);
		if (a == 0xD012) {
			unsigned char temp8 = d & 0x7F;
			if (temp8 == 13)
				temp8 = 10;
			charout(state, temp8);
		}
	}
}

apple_1_state apple_1_init() {
    memcpy(memory+0xff00, wozmon, 256);
    return (apple_1_state){initAndResetChip(), 0};
}

void apple_1_step(apple_1_state* state) {
    step(state->state);
    state->clk = !state->clk;
    if (!state->clk)
        handle_monitor(state->state);
}
