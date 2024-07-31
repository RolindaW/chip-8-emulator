#pragma once

class Chip8Memory
{
private:
	static const unsigned char kStackSize = 16;

private:
	unsigned char main_[4096];
	unsigned short stack_[kStackSize];
	unsigned char stack_pointer_;
	unsigned char framebuffer_[64*32];  // TODO: get display number of pixels (i.e. value 2048 == 64*32) from display entity DisplayResolution constant

public:
	Chip8Memory();

public:
	unsigned char Read(unsigned short address);
	void Write(unsigned short address, unsigned char value);
	unsigned short Pop();
	void Push(unsigned short value);
	unsigned char* GetFramebuffer();
};

