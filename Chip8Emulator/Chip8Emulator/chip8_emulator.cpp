#include "chip8_emulator.h"
#include "chip8_defs.h"
#include "chip8_rom.h"
#include "chip8_beep.h"

Chip8Emulator::Chip8Emulator()
	: memory_()
	, display_()
	, renderer_()
	, input_(renderer_)
	, cpu_(memory_, display_, input_)
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

void Chip8Emulator::StepCPU()
{
	this->cpu_.Cycle();
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

void Chip8Emulator::HandleInput()
{
	this->input_.HandleInput();
}

// Warning! Result (display framebuffer) of draw instructions issued between frames are missed (i.e. only result of last issued draw instruction is rendered)
void Chip8Emulator::Render()
{
	// TODO: Draw only if required (check display pixel blitting flag) - Warning! Implementation may be required
	const uint8_t* framebuffer = this->display_.GetFramebuffer();
	this->renderer_.Render(framebuffer);
}

void Chip8Emulator::LoadFont()
{
	for (unsigned char i = 0; i < sizeof(CHIP8_FONTSET); i++)
	{
		this->memory_.Write(CHIP8_FONT_ADDRESS + i, CHIP8_FONTSET[i]);
	}
}
