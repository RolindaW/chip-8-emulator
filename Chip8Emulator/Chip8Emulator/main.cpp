#include <string>
#include <chrono>  // Timer
//#include <thread>  // Sleep
#include <iostream>

#include "chip8_defs.h"
#include "chip8_romloader.h"
#include "chip8_rom.h"
#include "chip8_emulator.h"

void LogRealCpuHz(int cpuHz)
{
	std::cout << "Real CPU Hz: ";
	std::cout << std::dec << cpuHz;
	std::cout << "\n";
}

int main()
{
	std::string rom_filename = "C://workspace/chip-8-emulator/roms/test/test_opcode_mod.ch8";  // TODO: Get from program args; make relative - environment
	rom_filename = "C://workspace/chip-8-emulator/roms/invaders.rom";

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

	// Profile real CPU Hz
	int cpu_cycle_count = 0;
	auto last_second = last_cpu;
	const auto second_interval = std::chrono::microseconds(1000000);

	while (true)  // TODO: exit on console or window close event
	{
		auto now = clock::now();

		// CPU
		if (now - last_cpu >= cpu_interval) {
			emulator.StepCPU();
			cpu_cycle_count++;
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
			emulator.Render();
			last_frame = now;
		}		

		// TODO: Handle input
		// Warning! Input is - wrongly - being handled in renderer class for the moment

		// Relieve CPU
		// Warning! Emulator becomes very slow (50-75 Hz) whenever performing sleep (with almost any value - up to one nanosecond)
		// TODO: Make deterministic
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Profiler
		if (now - last_second >= second_interval) {
			LogRealCpuHz(cpu_cycle_count);
			cpu_cycle_count = 0;
			last_second = now;
		}
	}

	return 0;
}