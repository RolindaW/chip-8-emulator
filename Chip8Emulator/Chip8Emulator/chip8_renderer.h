#pragma once

#define GLFW_INCLUDE_NONE

#include <GL/gl3w.h>  // Extension loading library
#include <GLFW/glfw3.h>  // Window/Context creation library

struct DisplayResolution
{
	int width;
	int height;
	int total = width * height;
};

class Chip8Renderer
{
private:
	const int kOpenGLVersionMajor = 4;
	const int kOpenGLVersionMinor = 5;
	const DisplayResolution kDisplayResolution = { 64, 32 };

private:
	GLFWwindow* window_;

	GLuint program_;
	GLuint vao_;
	GLuint texture2D_;

public:
	Chip8Renderer();
	virtual ~Chip8Renderer();

public:
	void Render(const unsigned char* cache);
	bool WindowShouldClose();
	void PollEvents();
	bool IsKeyPressed(int key);

private:
	int InitializeGL();
	void TerminateGL();
	void InitializeObject();
	void InitializeProgram();
	void InitializeTexture();
	void TerminateObject();
	void SetTextureData(unsigned char* cache);
	void SetFilteredTextureData(const unsigned char* cache);
	void FilterTextureData(const unsigned char* rawData, unsigned char** filteredData);
	void Render();
};
