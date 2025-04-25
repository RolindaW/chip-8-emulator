#pragma once

#include "chip8_defs.h"

class Chip8Memory
{
private:
	unsigned char main_[CHIP8_RAM_SIZE];
	unsigned short stack_[CHIP8_STACK_SIZE];

public:
	Chip8Memory();

public:
	const unsigned char* GetPointer(unsigned short address);  // Warning! Use "const unsigned char*" to prevent writing into memory via return pointer
	unsigned char Read(unsigned short address);
	void Write(unsigned short address, unsigned char value);
	unsigned short Pop(unsigned char index);
	void Push(unsigned char index, unsigned short value);
};

