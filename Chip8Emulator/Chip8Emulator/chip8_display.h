#pragma once

class Chip8Display
{
private:
	// TODO: add a flag (public access) to indicate whether collision occur during sprite drawing or not
	// cummulative - not unset on start drawsrpite execution

	// TODO: get display number of pixels (i.e. value 2048 == 64*32) from display entity DisplayResolution constant
	unsigned char framebuffer_[64 * 32];  // Warning! Different from video memory (i.e. vram) - which would be owned by memory

public:
	Chip8Display();

public:
	const unsigned char* GetFramebuffer();
	void Clear();
	bool DrawSprite(unsigned char at_x, unsigned char at_y, const unsigned char* sprite_level, unsigned char sprite_height);
};

