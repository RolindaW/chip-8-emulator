#pragma once

#include <string>

#include "chip8_rom.h"

class Chip8RomLoader
{
public:
	Chip8Rom LoadFromFile(const std::string& filename);
};

