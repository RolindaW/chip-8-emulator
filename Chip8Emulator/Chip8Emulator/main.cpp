#include <string>
#include <chrono>  // Timer
#include <thread>  // Sleep

#include "chip8_defs.h"
#include "chip8_romloader.h"
#include "chip8_rom.h"
#include "chip8_emulator.h"

int main()
{
	std::string rom_filename = "C://workspace/chip-8-emulator/roms/test/IBM Logo.ch8";  // TODO: Get from program args; make relative - environment

	Chip8RomLoader rom_loader;
	Chip8Rom rom = rom_loader.LoadFromFile(rom_filename);

	Chip8Emulator emulator;
	emulator.LoadRom(rom);

	using clock = std::chrono::steady_clock;

	auto last_cpu = clock::now();
	auto last_timer = last_cpu;
	auto last_frame = last_cpu;

	const auto cpu_interval = std::chrono::microseconds(1000000 / CHIP8_CPU_HZ);  // TODO: Get from program args
	const auto timer_interval = std::chrono::microseconds(1000000 / CHIP8_TIMER_HZ);
	const auto frame_interval = std::chrono::microseconds(1000000 / CHIP8_FRAME_HZ);

	while (true)  // TODO: exit on console or window close event
	{
		auto now = clock::now();

		// CPU
		if (now - last_cpu >= cpu_interval) {
			// TODO: Execute CPU
			// emulator.stepCPU();
			last_cpu = now;
		}

		// Timers and sound
		if (now - last_timer >= timer_interval) {
			emulator.HandleTimers();
			emulator.HandleSound();
			last_timer = now;
		}

		// Rendering
		if (now - last_frame >= frame_interval) {
			//if (emulator.needsRedraw()) {
			//	const uint8_t* framebuffer = emulator.getFramebuffer();
			//	// TODO: render framebuffer to screen (SDL, OpenGL, etc.)
			//}
			last_frame = now;
		}		

		// TODO: Handle input

		// Relieve CPU
		// TODO: Make deterministic
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}