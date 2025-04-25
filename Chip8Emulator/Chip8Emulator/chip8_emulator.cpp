#include "chip8_emulator.h"
#include "chip8_defs.h"
#include "chip8_rom.h"
#include "chip8_beep.h"

Chip8Emulator::Chip8Emulator()
	: memory_()
	, cpu_(memory_)
	, beep_(kBeepFilename)
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

void Chip8Emulator::HandleTimers()
{
	this->cpu_.HandleTimers();
}

void Chip8Emulator::HandleSound()
{
	if (this->cpu_.IsBeeping())
	{
		this->beep_.Play();
	}
	else
	{
		this->beep_.Stop();
	}
}

void Chip8Emulator::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(CHIP8_FONTSET); i++)
	{
		this->memory_.Write(CHIP8_FONT_ADDRESS + i, CHIP8_FONTSET[i]);
	}
}
