#pragma once

#include <string>

#include "chip8_defs.h"
#include "chip8_memory.h"
#include "chip8_display.h"
#include "chip8_renderer.h"
#include "chip8_input.h"
#include "chip8_cpu.h"
#include "chip8_beep.h"
#include "chip8_rom.h"

class Chip8Emulator
{
private:
	// TODO: maybe is better using default sound and not allowing configuration. (or at least doing different way - insnide a sound loader class) think about this
	// TODO: Move into soundloader class like a default sound or similar
	char kBeepFilename[80] = "C://workspace/chip-8-emulator/audio/censor-beep-1s.wav";

private:
	Chip8Memory memory_;
	Chip8Display display_;
	Chip8Renderer renderer_;
	Chip8Input input_;
	Chip8Cpu cpu_;
	Chip8Beep beep_;

public:
	Chip8Emulator();

public:
	void LoadRom(const Chip8Rom& rom);
	void StepCPU();
	void HandleTimers();
	void HandleSound();
	void PollInput();
	void Render();
	bool WindowShouldClose();

private:
	void LoadFont();
};