#pragma once

#include <string>
#include <cstdio>

class Chip8Rom
{
public:
	std::string filename_;
	unsigned short size_;
	unsigned char* content_;

public:
	Chip8Rom(std::string filename);
	virtual ~Chip8Rom();

private:
	void Load();
};

