#pragma once

#include "chip8_renderer.h"
#include "chip8_defs.h"

constexpr enum Chip8Key : unsigned char {
	KEY_0 = 0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F
};

// TODO: use scandcode instead of key value to support different keyboard layouts (e.g. QUERTY, QZERTY)
class Chip8Input
{
private:
	Chip8Renderer& renderer_;
	bool key_state_[CHIP8_KEY_COUNT];

public:
	Chip8Input(Chip8Renderer& renderer);

public:
	void Poll();
	bool IsKeyPressed(unsigned char key);
	bool GetKeyPressed(unsigned char& key);

private:
	int MapKey(unsigned char key);
};
