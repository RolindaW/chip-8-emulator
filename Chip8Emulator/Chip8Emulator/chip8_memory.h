#pragma once

class Chip8Memory
{
private:
	static const unsigned char kStackSize = 16;

private:
	unsigned char main_[4096];
	unsigned short stack_[kStackSize];
	unsigned char stack_pointer_;  // TODO: verify whether sp should be stored in memory or in CPU

public:
	Chip8Memory();

public:
	const unsigned char* GetPointer(unsigned short address);  // Warning! Use "const unsigned char*" to prevent writing into memory via return pointer
	unsigned char Read(unsigned short address);
	void Write(unsigned short address, unsigned char value);
	unsigned short Pop();
	void Push(unsigned short value);
};

