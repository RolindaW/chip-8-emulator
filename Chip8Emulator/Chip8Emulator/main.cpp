#include <string>
#include <iostream>
#include <chrono>  // Clock
//#include <thread>  // Sleep

#include "chip8_defs.h"
#include "chip8_args.h"
#include "chip8_romloader.h"
#include "chip8_rom.h"
#include "chip8_emulator.h"

void LogRealHz(std::string msg, int hz)
{
	std::cout << msg << ": ";
	std::cout << std::dec << hz;
	std::cout << "\n";
}

int main(int argc, char* argv[])
{
	// Warning! Mock using project DEBUG command line arguments
	Chip8ArgumentParser parser;
	Chip8Arguments args = parser.Parse(argc, argv);

	if (args.parse_error || args.show_help)
	{
		parser.PrintHelp();
		return 0;
	}

	Chip8RomLoader rom_loader;
	Chip8Rom rom = rom_loader.LoadFromFile(args.rom_filename);

	Chip8Emulator emulator;
	emulator.LoadRom(rom);

	// Timing - CPU, timers (and sound) and rendering (and input)
	// Warning! Basic timing technique not appropriate for more complex emulators
	using clock = std::chrono::steady_clock;

	auto last_cpu = clock::now();
	auto last_timer = last_cpu;
	auto last_frame = last_cpu;

	const auto cpu_interval = std::chrono::microseconds(1000000 / args.cpu_hz);
	const auto timer_interval = std::chrono::microseconds(1000000 / CHIP8_TIMER_HZ);
	const auto frame_interval = std::chrono::microseconds(1000000 / CHIP8_FRAME_HZ);

	// Profiler
	auto last_second = last_cpu;
	const auto second_interval = std::chrono::microseconds(1000000);
	int cpu_cycle_count = 0;
	int timer_count = 0;
	int frame_count = 0;

	while (true)  // TODO: exit on console or window close event
	{
		auto now = clock::now();

		// CPU
		if (now - last_cpu >= cpu_interval) {
			emulator.StepCPU();
			cpu_cycle_count++;
			last_cpu += cpu_interval;  // Warning! Ensure accurate number of cycles even if system is temporally halted/delayed
		}

		// Timers and sound
		if (now - last_timer >= timer_interval) {
			emulator.HandleTimers();
			emulator.HandleSound();
			timer_count++;
			last_timer += timer_interval;
		}

		// Rendering and input
		if (now - last_frame >= frame_interval) {
			emulator.Render();
			emulator.HandleInput();
			frame_count++;
			last_frame += frame_interval;
		}

		// Relieve CPU
		// Warning! Windows OS timer resolution is by default ~15.6 ms - it can be changed by affects the whole system
		// Emulator becomes very slow (50-75 Hz) whenever sleep is performed
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// Profiler
		if (now - last_second >= second_interval) {
			LogRealHz("CPU Hz", cpu_cycle_count);
			LogRealHz("Timers Hz", timer_count);
			LogRealHz("FPS", frame_count);
			cpu_cycle_count = 0;
			timer_count = 0;
			frame_count = 0;
			last_second += second_interval;
		}
	}

	return 0;
}