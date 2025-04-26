#pragma once

#include "chip8_renderer.h"

enum Key : unsigned char {
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

class Chip8Input
{
private:
	Chip8Renderer& renderer_;

public:
	Chip8Input(Chip8Renderer& renderer);

public:
	void HandleInput();
	bool IsKeyPressed(unsigned char key);
	bool GetKeyPressed(unsigned char& key);

private:
	int MapKeyToken(Key key);
};

