#include <iostream>
#include <string>

#include "chip8_cpu.h"
#include "chip8_rom.h"

Chip8Cpu::Chip8Cpu()
	: program_counter_(0)
	, index_register_(0)
	, stack_pointer_(0)
	, gp_register_{0}
	, delay_timer_(0)
	, sound_timer_(0)
	, memory_()
	, opcode_(0)
{
	LoadFont();
}

void Chip8Cpu::Start(std::string filename)
{
	LoadRom(filename);
	this->program_counter_ = kRomAddress;

	while (true)
	{
		Cycle();
	}
}

void Chip8Cpu::LoadRom(std::string filename)
{
	Chip8Rom rom_(filename);
	for (unsigned short i = 0; i < rom_.size_; i++)
	{
		this->memory_.Write(kRomAddress + i, rom_.content_[i]);
	}
}

void Chip8Cpu::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(kFont); i++)
	{
		this->memory_.Write(kFontAddress + i, kFont[i]);
	}
}

void Chip8Cpu::Cycle()
{
	Fetch();
	Process();
}

void Chip8Cpu::Fetch()
{
	unsigned char high_byte = this->memory_.Read(this->program_counter_);
	unsigned char low_byte = this->memory_.Read(this->program_counter_ + (unsigned short)1);

	this->opcode_ = high_byte << 8 | low_byte;

	this->program_counter_ += 2;
}

void Chip8Cpu::Process()
{
	LogFetchedOpcode();

	switch (this->opcode_ & 0xF000)
	{
		case 0x1000: // 1NNN
		{
			LogDecodedInstruction("1NNN");
			unsigned short address = DecodeNNN();
			this->program_counter_ = address;
			break;
		}
		case 0x6000: // 6XNN
		{
			LogDecodedInstruction("6XNN");
			unsigned char gp_register_index = DecodeX();
			unsigned char value = DecodeNN();
			this->gp_register_[gp_register_index] = value;
			break;
		}
		case 0x7000: // 7XNN
		{
			LogDecodedInstruction("7XNN");
			unsigned char gp_register_index = DecodeX();
			unsigned char value = DecodeNN();
			this->gp_register_[gp_register_index] += value;
			break;
		}
		case 0xA000: // ANNN
		{
			LogDecodedInstruction("ANNN");
			unsigned short address = DecodeNNN();
			this->index_register_ = address;
			break;
		}
		default:
			std::cout << "Warning! Unknown instruction.";
			std::cout << "\n";
			break;
	}
}

unsigned char Chip8Cpu::DecodeX()
{
	return (this->opcode_ & (unsigned short)0x0F00) >> 8;
}

unsigned char Chip8Cpu::DecodeY()
{
	return (this->opcode_ & (unsigned short)0x00F0) >> 4;
}

unsigned char Chip8Cpu::DecodeN()
{
	return this->opcode_ & (unsigned short)0x000F;
}

unsigned char Chip8Cpu::DecodeNN()
{
	return this->opcode_ & (unsigned short)0x00FF;
}

unsigned short Chip8Cpu::DecodeNNN()
{
	return this->opcode_ & (unsigned short)0x0FFF;
}

void Chip8Cpu::LogFetchedOpcode()
{
	std::cout << "Opcode: ";
	std::cout << std::hex << this->opcode_;
	std::cout << " - ";
}

void Chip8Cpu::LogDecodedInstruction(std::string instruction)
{
	std::cout << "Instruction: " + instruction;
	std::cout << "\n";
}
