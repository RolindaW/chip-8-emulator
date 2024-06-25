#pragma once

#include "chip8_memory.h"

class Chip8Cpu
{
private:
	unsigned char kFont[80] = {};

private:
	unsigned short program_counter_;
	unsigned short index_register_;
	unsigned char stack_pointer_;
	unsigned char gp_register_[16];

	unsigned char delay_timer_;
	unsigned char sound_timer_;

	Chip8Memory memory_;

private:
	unsigned short opcode_;

public:
	Chip8Cpu();

public:
	void Start();

private:
	void Cycle();
	void Fetch();
	void Process();
};

