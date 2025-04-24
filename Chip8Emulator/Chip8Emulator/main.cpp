#include <string>

#include "chip8_romloader.h"
#include "chip8_rom.h"
#include "chip8_emulator.h"

int main()
{
	std::string rom_filename = "C://workspace/chip-8-emulator/roms/test/IBM Logo.ch8";  // TODO: get from program args; make relative - environment

	Chip8RomLoader rom_loader;
	Chip8Rom rom = rom_loader.LoadFromFile(rom_filename);

	Chip8Emulator emulator;
	emulator.LoadRom(rom);

	while (true) {  // TODO: exit on console or window close event

		// TODO: execute cycle
		// TODO: update timers
		// TODO: render (conditional, on 60 Hz)
	}

	return 0;
}