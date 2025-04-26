#include "chip8_memory.h"
#include "chip8_defs.h"

Chip8Memory::Chip8Memory()
	: main_{0}
	, stack_{0}
{
}

const unsigned char* Chip8Memory::GetPointer(unsigned short address)
{
	// TODO: throw error if out of range
	return &this->main_[address];
}

unsigned char Chip8Memory::Read(unsigned short address)
{
	// TODO: throw error if out of range
	return this->main_[address];
}

void Chip8Memory::Write(unsigned short address, unsigned char value)
{
	// TODO: throw error if out of range
	this->main_[address] = value;
}

unsigned short Chip8Memory::Pop(unsigned char index)
{
	// TODO: throw error if stack underflow
	if (index >= 0)
	{
		return this->stack_[index];
	}
}

void Chip8Memory::Push(unsigned char index, unsigned short value)
{
	// TODO: throw error if stack overflow
	if (index < CHIP8_STACK_SIZE)
	{
		this->stack_[index] = value;
	}
}
