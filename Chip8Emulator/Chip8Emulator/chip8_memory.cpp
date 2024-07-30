#include "chip8_memory.h"

Chip8Memory::Chip8Memory()
	: main_{0}
	, stack_{0}
	, stack_pointer_(0)
	, framebuffer_{0}
{
}

unsigned char Chip8Memory::Read(unsigned short address)
{
	return this->main_[address];
}

void Chip8Memory::Write(unsigned short address, unsigned char value)
{
	this->main_[address] = value;
}

unsigned short Chip8Memory::Pop()
{
	// TODO: throw error if stack underflow
	if (this->stack_pointer_ >  0)
	{
		this->stack_pointer_--;
		return this->stack_[this->stack_pointer_];
	}
}

void Chip8Memory::Push(unsigned short value)
{
	// TODO: throw error if stack overflow
	if (this->stack_pointer_ < kStackSize - 1)
	{
		this->stack_[this->stack_pointer_] = value;
		this->stack_pointer_++;
	}
}

unsigned char* Chip8Memory::GetFramebuffer()
{
	return framebuffer_;
}
