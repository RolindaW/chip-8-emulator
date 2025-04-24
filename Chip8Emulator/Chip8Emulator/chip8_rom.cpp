#include <string>

#include "chip8_rom.h"

Chip8Rom::Chip8Rom(const std::string& filename)
	: filename_(filename)
	, content_{0}
	, size_(0)
{
}

Chip8Rom::~Chip8Rom()
{
	delete[] this->content_;
	this->content_ = nullptr;
}
