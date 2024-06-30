#include "chip8_memory.h"

Chip8Memory::Chip8Memory()
	: main_{0}
	, stack_{0}
	, framebuffer_{0}
{
	// TODO: test - remove on finish
	for (int i = 0; i < 64 * 32; i++)
	{
		//framebuffer_[i] = i % 2 == 0 ? 0x00 : 0xFF;
		framebuffer_[i] = 0x00;
	}
}

unsigned char Chip8Memory::Read(unsigned short address)
{
	return this->main_[address];
}

void Chip8Memory::Write(unsigned short address, unsigned char value)
{
	this->main_[address] = value;
}

unsigned char* Chip8Memory::GetFramebuffer()
{
	return framebuffer_;
}
