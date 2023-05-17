#include "apple_1_emu.h"

#include "apple_1_wozmon.h"
#include "apple_1_basic.h"

#include <stdint.h>
#include <ctype.h>

void reset6502();
void exec6502(uint32_t tickcount);
void step6502();                
void irq6502();
void nmi6502();
void hookexternal(void *funcptr);

extern uint8_t readkey();
extern void writechar(uint8_t ch);
extern uint8_t keyavailable();

#define MEM_SIZE 0x8000

uint8_t memory[MEM_SIZE];

uint8_t read6502(uint16_t address) {
	if (address < MEM_SIZE)
		return memory[address]; // Ram
	if ((address & 0xFF1F) == 0xD010) {
		uint8_t c = toupper(readkey());
		if (c == 10)
			c = 13;
		c |= 0x80;
		return c; // Give the character
	}
	if ((address & 0xFF1F) == 0xD011) {
        if(keyavailable())
			return 0x80; // Characters available
		else
			return 0;    // No characters available
	}
	if ((address & 0xFF1F) == 0xD012)
		return 0; // We are ready to print the character
    if (address >= 0xE000 && address < 0xF000)
        return basic[address - 0xE000];
	if (address >= 0xFF00)
		return wozmon[address-0xFF00];

	return 0;
}

void write6502(uint16_t address, uint8_t value) {
	if (address < MEM_SIZE)
		memory[address] = value;
	if ((address & 0xFF1F) == 0xD012) {
		uint8_t temp8 = value & 0x7F;
		if (temp8 == 13)
			temp8 = 10;
		writechar(temp8);
	}
}

void apple_1_init() {
    reset6502();
}

void apple_1_step() {
	step6502();
}
