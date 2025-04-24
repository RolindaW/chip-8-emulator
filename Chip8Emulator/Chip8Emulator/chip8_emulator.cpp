#include "chip8_emulator.h"

#include "chip8_rom.h"

Chip8Emulator::Chip8Emulator()
	//: beep_(0)
{
	// TODO: load font sprites in memory
	// TODO: initialize RNG seed
}

void Chip8Emulator::LoadRom(const Chip8Rom& rom)
{
	for (unsigned short i = 0; i < rom.size_; i++)
	{
		this->memory_.Write(kRomAddress + i, rom.content_[i]);
	}
}
