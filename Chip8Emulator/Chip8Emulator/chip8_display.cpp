#include "chip8_display.h"

Chip8Display::Chip8Display()
	: framebuffer_{ 0 }
{
}

const unsigned char* Chip8Display::GetFramebuffer()
{
	return this->framebuffer_;
}

// TODO: get display number of pixels (i.e. value 2048 == 64*32) from display entity DisplayResolution constant
void Chip8Display::Clear()
{
	for (unsigned short i = 0; i < 64 * 32; i++)
	{
		this->framebuffer_[i] = 0x00;
	}
}

// TODO: implement variation (wrap sprite around display)
// TODO: get display bounds (i.e. values 32 and 64) from display entity DisplayResolution constant
bool Chip8Display::DrawSprite(unsigned char at_x, unsigned char at_y, const unsigned char* sprite, unsigned char sprite_height)
{
	bool collision = false;

	// Wrap sprite coordinate
	at_x %= 64;
	at_y %= 32;

	for (unsigned char i = 0; i < sprite_height; i++)  // "i": level of the sprite 
	{
		// Iterate by sprite level (i.e. byte) - from top to bottom
		// Warning! Kept for readability - actually using "sprite" pointer will suffice (update "sprite++;" and usage "...*sprite...")
		unsigned char sprite_level = *(sprite+i);  // "sprite++" will suffice

		unsigned char offset_at_y = at_y + i;
		if (!(offset_at_y < 32)) {
			// Stop iterating levels (i.e. end sprite drawing) when bottom limit is reached
			// TODO: implement variation (wrap sprite bottom-up): offset_at_y %= 64;
			break;
		}

		for (unsigned char j = 0; j < 8; j++)  // "j": bit of the level
		{
			// Iterate by level bit
			unsigned char offset_at_x = at_x + j;
			if (!(offset_at_x < 64)) {
				// Stop iterating bits (i.e. end level drawing) when right limit is reached
				// TODO: implement variation (wrap sprite right-left): offset_at_x %= 32;
				break;
			}

			if (!(sprite_level & (0b10000000 >> j)))
			{
				// End processing of the current bit if it is not active (no change would be made)
				continue;
			}

			// Calculate the index of the display buffer for the axis convention +X from left to right and +Y from top to bottom
			unsigned short framebuffer_index = 64 * (31 - offset_at_y) + offset_at_x;

			// At this point it is certain that the current bit will modify the display buffer; indicate collision if the corresponding pixel is active
			// Warning! Use OR assigment to prevent collision flag from being unset if already set
			collision |= this->framebuffer_[framebuffer_index];

			// Invert corresponding pixel status
			this->framebuffer_[framebuffer_index] = ~this->framebuffer_[framebuffer_index];
		}
	}

	return collision;
}
