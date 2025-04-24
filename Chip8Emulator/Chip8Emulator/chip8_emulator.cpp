#include "chip8_emulator.h"
#include "chip8_defs.h"
#include "chip8_rom.h"

Chip8Emulator::Chip8Emulator()
	//: beep_(0)
{
	LoadFont();
}

void Chip8Emulator::LoadRom(const Chip8Rom& rom)
{
	for (unsigned short i = 0; i < rom.size_; i++)
	{
		this->memory_.Write(CHIP8_ROM_ADDRESS + i, rom.content_[i]);
	}
}

void Chip8Emulator::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(CHIP8_FONTSET); i++)
	{
		this->memory_.Write(CHIP8_FONT_ADDRESS + i, CHIP8_FONTSET[i]);
	}
}
