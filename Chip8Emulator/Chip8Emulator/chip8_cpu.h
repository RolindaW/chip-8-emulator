#pragma once

#include <iostream>
#include <string>
#include <chrono> // TODO: Remove when removing sleep.
#include <thread> // TODO: Remove when removing sleep.
#include <random>

#include "chip8_memory.h"
#include "chip8_rom.h"
#include "chip8_display.h"
#include "chip8_beep.h"

enum Instruction : unsigned char {
	I0NNN = 1,
	I00E0,
	I00EE,
	I1NNN,
	I2NNN,
	I3XNN,
	I4XNN,
	I5XY0,
	I6XNN,
	I7XNN,
	I8XY0,
	I8XY1,
	I8XY2,
	I8XY3,
	I8XY4,
	I8XY5,
	I8XY6,
	I8XY7,
	I8XYE,
	I9XY0,
	IANNN,
	IBNNN,
	ICXNN,
	IDXYN,
	IFX07,
	IFX15,
	IFX18,
	IFX1E,
	IFX29,
	IFX33,
	IFX55,
	IFX65
};

typedef std::mt19937 MyRNG;


class Chip8Cpu
{
private:
	const unsigned short kFontAddress = 0x50;
	const unsigned char kFont[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	const unsigned short kRomAddress = 0x200;

	uint32_t kSeed = 0x31;  // TODO: RElocate about rndom nuber genration

	char kBeepFilename[80] = "C://workspace/chip-8-emulator/audio/censor-beep-1s.wav";  // TODO: Move this to another location

private:
	unsigned short program_counter_;
	unsigned short index_register_;
	unsigned char gp_register_[16];

	unsigned char delay_timer_;
	unsigned char sound_timer_;

	Chip8Memory memory_;
	Chip8Display display_;

	MyRNG rng_;  // TODO: RElocate about rndom nuber genration

	Chip8Beep beep_;

private:
	unsigned short opcode_;
	unsigned char instruction_;

public:
	Chip8Cpu();

public:
	void Start(std::string filename);

private:
	void LoadFont();
	void LoadRom(std::string filename);
	void Cycle();
	void Fetch();
	void NextInstruction();
	void Decode();
	void Execute();
	unsigned char DecodeX();
	unsigned char DecodeY();
	unsigned char DecodeN();
	unsigned char DecodeNN();
	unsigned short DecodeNNN();
	void ClearDisplay();
	void DrawSprite(unsigned char at_x, unsigned char at_y, unsigned char sprite_height);
	void HandleTimers();
	void LogFetchedOpcode();
	void LogDecodedInstruction(std::string instruction);
};

