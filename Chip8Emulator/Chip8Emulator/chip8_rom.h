#pragma once

#include <string>

class Chip8Rom
{
public:
	std::string filename_;
	unsigned char* content_;
	unsigned short size_;

public:
	Chip8Rom(const std::string& filename);
	virtual ~Chip8Rom();
};

