#include "chip8_cpu.h"

Chip8Cpu::Chip8Cpu()
	: program_counter_(0)
	, index_register_(0)
	, stack_pointer_(0)
	, gp_register_{0}
	, delay_timer_(0)
	, sound_timer_(0)
	, memory_()
{}

void Chip8Cpu::Start()
{
	while (true)
	{
		Cycle();
	}
}

void Chip8Cpu::Cycle()
{
	// TODO: Fetch-Decode-Execute instruction cycle
}