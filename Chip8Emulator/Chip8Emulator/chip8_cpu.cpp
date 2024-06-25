
#include <iostream>

#include "chip8_cpu.h"

Chip8Cpu::Chip8Cpu()
	: program_counter_(0)
	, index_register_(0)
	, stack_pointer_(0)
	, gp_register_{0}
	, delay_timer_(0)
	, sound_timer_(0)
	, memory_()
	, opcode_(0)
{
	LoadFont();
}

void Chip8Cpu::Start()
{
	this->program_counter_ = kRomAddress;

	while (true)
	{
		Cycle();
	}
}

void Chip8Cpu::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(kFont); i++)
	{
		this->memory_.Write(kFontAddress + i, kFont[i]);
	}
}

void Chip8Cpu::Cycle()
{
	Fetch();
	Process();
}

void Chip8Cpu::Fetch()
{
	unsigned char high_byte = this->memory_.Read(this->program_counter_);
	unsigned char low_byte = this->memory_.Read(this->program_counter_ + (unsigned short)1);

	this->opcode_ = high_byte << 8 | low_byte;

	this->program_counter_ += 2;
}

void Chip8Cpu::Process()
{
	switch (this->opcode_)
	{
	default:
		std::cout << "Not known opcode: ";
		std::cout << this->opcode_;
		std::cout << "\n";
		break;
	}
}
