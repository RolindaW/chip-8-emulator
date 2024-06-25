#include "chip8_memory.h"

Chip8Memory::Chip8Memory()
	: main_memory_{0}
	, stack_{0}
	, display_memory_{0}
{}

unsigned char Chip8Memory::Read(unsigned short address)
{
	return this->main_memory_[address];
}

void Chip8Memory::Write(unsigned short address, unsigned char value)
{
	this->main_memory_[address] = value;
}
