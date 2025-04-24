#pragma once

#include <string>

#include "chip8_defs.h"
//#include "chip8_cpu.h"
#include "chip8_memory.h"
//#include "chip8_display.h"
//#include "chip8_beep.h"
#include "chip8_rom.h"

class Chip8Emulator
{
private:
	//Chip8Cpu cpu_;
	Chip8Memory memory_;
	//Chip8Display display_;
	//Chip8Beep beep_;

public:
	Chip8Emulator();

public:
	void LoadRom(const Chip8Rom& rom);

private:
	void LoadFont();
};