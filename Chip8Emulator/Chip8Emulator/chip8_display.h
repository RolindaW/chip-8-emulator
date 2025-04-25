#pragma once

#define GLFW_INCLUDE_NONE

#include <GL/gl3w.h>  // Extension loading library
#include <GLFW/glfw3.h>  // Window/Context creation library
#include <stdio.h>

struct DisplayResolution
{
	int width;
	int height;
	int total = width * height;
};

class Chip8Display
{
private:
	const int kOpenGLVersionMajor = 4;
	const int kOpenGLVersionMinor = 5;
	const DisplayResolution kDisplayResolution = { 64, 32 };

private:
	// TODO: get display number of pixels (i.e. value 2048 == 64*32) from display entity DisplayResolution constant
	unsigned char framebuffer_[64 * 32];  // Warning! Different from video memory (i.e. vram) - which would be owned by memory

	GLFWwindow* window_;

	GLuint program_;
	GLuint vao_;
	GLuint texture2D_;

public:
	Chip8Display();
	virtual ~Chip8Display();

public:
	unsigned char* GetFramebuffer();
	void Clear();
	bool DrawSprite(unsigned char at_x, unsigned char at_y, const unsigned char* sprite_level, unsigned char sprite_height);

	void Render();  // TODO: move rendering into a renderer class
	void Render(unsigned char* cache);  // TODO: move rendering into a renderer class
	bool IsKeyPressed(unsigned char keyHex);  // TODO: move to corresponding entity
	bool GetKeyPressed(unsigned char* keyHex);  // TODO: move to corresponding entity

private:
	int InitializeGL();  // TODO: move rendering into a renderer class
	void TerminateGL();
	void Initialize();
	void InitializeProgram();
	void InitializeTexture();
	void SetTextureData(unsigned char* cache);
	void SetFilteredTextureData(unsigned char* cache);
	void FilterTextureData(unsigned char* rawData, unsigned char** filteredData);
	void Terminate();
	int MapKeyToken(unsigned char keyHex);  // TODO: move to corresponding entity
};

