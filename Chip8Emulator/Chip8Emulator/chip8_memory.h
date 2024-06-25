#pragma once

class Chip8Memory
{
private:
	unsigned char main_memory_[4096];
	unsigned short stack_[16];
	unsigned char display_memory_[64 * 32];

public:
	Chip8Memory();

public:
	unsigned char Read(unsigned short address);
	void Write(unsigned short address, unsigned char value);
};

