#pragma once

#include <iostream>
#include <string>

#include "chip8_memory.h"
#include "chip8_rom.h"
#include "chip8_display.h"

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

private:
	unsigned short program_counter_;
	unsigned short index_register_;
	unsigned char stack_pointer_;
	unsigned char gp_register_[16];

	unsigned char delay_timer_;
	unsigned char sound_timer_;

	Chip8Memory memory_;
	Chip8Display display_;

private:
	unsigned short opcode_;

public:
	Chip8Cpu();

public:
	void Start(std::string filename);

private:
	void LoadFont();
	void LoadRom(std::string filename);
	void Cycle();
	void Fetch();
	void Process();
	unsigned char DecodeX();
	unsigned char DecodeY();
	unsigned char DecodeN();
	unsigned char DecodeNN();
	unsigned short DecodeNNN();
	void ClearDisplay();
	void DrawSprite(unsigned char at_x, unsigned char at_y, unsigned char sprite_height);
	void LogFetchedOpcode();
	void LogDecodedInstruction(std::string instruction);
};

