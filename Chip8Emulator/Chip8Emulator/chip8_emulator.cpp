#include "chip8_emulator.h"
#include "chip8_defs.h"
#include "chip8_rom.h"
#include "chip8_beep.h"

Chip8Emulator::Chip8Emulator()
	: memory_()
	, display_()
	, cpu_(memory_, display_)
	, beep_(kBeepFilename)
{
	LoadFont();
}

void Chip8Emulator::LoadRom(const Chip8Rom& rom)
{
	for (unsigned short i = 0; i < rom.size_; i++)
	{
		this->memory_.Write(CHIP8_ROM_ADDRESS + i, rom.content_[i]);
	}
}

void Chip8Emulator::HandleTimers()
{
	this->cpu_.HandleTimers();
}

void Chip8Emulator::HandleSound()
{
	if (this->cpu_.IsBeeping())
	{
		this->beep_.Play();
	}
	else
	{
		this->beep_.Stop();
	}
}

// TODO: Move rendering to a seprate class from display to decouple rendering from display/video device emulation
// This way rendering could be performed outside the emulator in a different way (console, web - canvas, webgl, sdl, etc.)
// In this case display framebuffer should be made available for reading from outside the emulator
// TODO: Draw only if required - check pixel blitting flag
void Chip8Emulator::Render()
{
	//if (display.needsRedraw()) {
	//	const uint8_t* framebuffer = display_.getFramebuffer();
	//	// TODO: render framebuffer to screen (SDL, OpenGL, etc.)
	//}
	this->display_.Render();
}

void Chip8Emulator::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(CHIP8_FONTSET); i++)
	{
		this->memory_.Write(CHIP8_FONT_ADDRESS + i, CHIP8_FONTSET[i]);
	}
}
