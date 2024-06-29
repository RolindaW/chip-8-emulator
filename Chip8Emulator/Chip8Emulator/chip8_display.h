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
	const DisplayResolution kDisplayResolution = { 8, 4 };//{ 64, 32 };

private:
	GLFWwindow* window_;

	GLuint program_;
	GLuint vao_;
	GLuint texture2D_;

public:
	Chip8Display();
	virtual ~Chip8Display();

public:
	void Render();
	void Render(unsigned char* cache);

private:
	int Initialize();
	void InitializeStuff();
	void InitializeProgram();
	void InitializeTexture();
	void SetTextureData(unsigned char* cache);
	void SetFilteredTextureData(unsigned char* cache);
	void FilterTextureData(unsigned char* rawData, unsigned char** filteredData);
	int Terminate();
	void ReleaseStuff();
};

