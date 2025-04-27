#include "chip8_input.h"
#include "chip8_renderer.h"
#include "chip8_defs.h"

Chip8Input::Chip8Input(Chip8Renderer& renderer)
	: renderer_(renderer)
	, key_state_{false}
{
}

void Chip8Input::Poll()
{
	this->renderer_.PollEvents();

	for (unsigned char i = 0; i < CHIP8_KEY_COUNT; i++)
	{
		int glfwKey = MapKey(i);
		this->key_state_[i] = this->renderer_.IsKeyPressed(glfwKey);
	}
}

bool Chip8Input::IsKeyPressed(unsigned char key)
{
	return this->key_state_[key];
}

bool Chip8Input::GetKeyPressed(unsigned char &key)
{
	for (unsigned char i = 0; i < CHIP8_KEY_COUNT; i++)
	{
		if (IsKeyPressed(i))
		{
			key = i;
			return true;
		}
	}

	return false;
}

int Chip8Input::MapKey(unsigned char key)
{
	switch (key)
	{
	case Chip8Key::KEY_1:
		return GLFW_KEY_1;
	case Chip8Key::KEY_2:
		return GLFW_KEY_2;
	case Chip8Key::KEY_3:
		return GLFW_KEY_3;
	case Chip8Key::KEY_C:
		return GLFW_KEY_4;
	case Chip8Key::KEY_4:
		return GLFW_KEY_Q;
	case Chip8Key::KEY_5:
		return GLFW_KEY_W;
	case Chip8Key::KEY_6:
		return GLFW_KEY_E;
	case Chip8Key::KEY_D:
		return GLFW_KEY_R;
	case Chip8Key::KEY_7:
		return GLFW_KEY_A;
	case Chip8Key::KEY_8:
		return GLFW_KEY_S;
	case Chip8Key::KEY_9:
		return GLFW_KEY_D;
	case Chip8Key::KEY_E:
		return GLFW_KEY_F;
	case Chip8Key::KEY_A:
		return GLFW_KEY_Z;
	case Chip8Key::KEY_0:
		return GLFW_KEY_X;
	case Chip8Key::KEY_B:
		return GLFW_KEY_C;
	case Chip8Key::KEY_F:
		return GLFW_KEY_V;
	default:
		return GLFW_KEY_UNKNOWN;  // TODO: Handle error
	}
}
