#pragma once

#include <iostream>
#include <string>
#include <random>
#include <chrono>  // Basic cycle time implementation
#include <thread>  // Basic cycle time implementation

#include "chip8_defs.h"
#include "chip8_memory.h"
#include "chip8_display.h"
#include "chip8_input.h"

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
	IEX9E,
	IEXA1,
	IFX07,
	IFX0A,
	IFX15,
	IFX18,
	IFX1E,
	IFX29,
	IFX33,
	IFX55,
	IFX65
};

// Mersenne Twister random number generator
typedef std::mt19937 MersenneRNG;

class Chip8Cpu
{
private:
	unsigned short program_counter_;
	unsigned short index_register_;
	unsigned char stack_pointer_;
	unsigned char gp_register_[CHIP8_GP_REGISTER_COUNT];
	unsigned char delay_timer_;
	unsigned char sound_timer_;

	MersenneRNG rng_;

	unsigned short opcode_;
	unsigned char instruction_;

private:
	Chip8Memory& memory_;  // Memory is not owned by CPU; reference is a valid approach because not planing memory detaching or switching
	Chip8Display& display_;
	Chip8Input& input_;

public:
	Chip8Cpu(Chip8Memory& memory, Chip8Display& display, Chip8Input& input);

public:
	void Cycle();
	void HandleTimers();
	bool IsBeeping();

private:
	void Fetch();
	void NextInstruction();
	void PreviousInstruction();
	void Decode();
	void Execute();
	unsigned char DecodeX();
	unsigned char DecodeY();
	unsigned char DecodeN();
	unsigned char DecodeNN();
	unsigned short DecodeNNN();
	void LogFetchedOpcode();
	void LogDecodedInstruction(std::string instruction);
};

