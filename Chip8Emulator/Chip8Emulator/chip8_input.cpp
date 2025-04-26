#include "chip8_input.h"
#include "chip8_renderer.h"
#include "chip8_defs.h"

Chip8Input::Chip8Input(Chip8Renderer& renderer)
	: renderer_(renderer)
{
}

void Chip8Input::HandleInput()
{
	this->renderer_.PollEvents();
}

// TODO: use scandcode instead of key to support different keyboard layouts (e.g. QUERTY, QZERTY)
bool Chip8Input::IsKeyPressed(unsigned char key)
{
	int keyToken = MapKeyToken((Key)key);
	return this->renderer_.IsKeyPressed(keyToken);
}

bool Chip8Input::GetKeyPressed(unsigned char& key)
{
	bool keyPressed = false;

	for (unsigned char i = 0; i < CHIP8_KEY_COUNT; i++)
	{
		if (IsKeyPressed(i))
		{
			key = i;
			keyPressed = true;
			break;
		}
	}

	return keyPressed;
}

int Chip8Input::MapKeyToken(Key key)
{
	switch (key)
	{
	case Key::KEY_1:
		return GLFW_KEY_1;
	case Key::KEY_2:
		return GLFW_KEY_2;
	case Key::KEY_3:
		return GLFW_KEY_3;
	case Key::KEY_C:
		return GLFW_KEY_4;
	case Key::KEY_4:
		return GLFW_KEY_Q;
	case Key::KEY_5:
		return GLFW_KEY_W;
	case Key::KEY_6:
		return GLFW_KEY_E;
	case Key::KEY_D:
		return GLFW_KEY_R;
	case Key::KEY_7:
		return GLFW_KEY_A;
	case Key::KEY_8:
		return GLFW_KEY_S;
	case Key::KEY_9:
		return GLFW_KEY_D;
	case Key::KEY_E:
		return GLFW_KEY_F;
	case Key::KEY_A:
		return GLFW_KEY_Z;
	case Key::KEY_0:
		return GLFW_KEY_X;
	case Key::KEY_B:
		return GLFW_KEY_C;
	case Key::KEY_F:
		return GLFW_KEY_V;
	default:
		return GLFW_KEY_UNKNOWN;
	}
}
