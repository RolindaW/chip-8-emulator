#pragma once

#define GLFW_INCLUDE_NONE

#include <GL/gl3w.h>  // Extension loading library
#include <GLFW/glfw3.h>  // Window/Context creation library
#include <stdio.h>

class Chip8Display
{
private:
	const int kOpenGLVersionMajor = 4;
	const int kOpenGLVersionMinor = 5;

private:
	GLFWwindow* window_;

	GLuint program_;
	GLuint vao_;
	GLuint texture2D_;

public:
	Chip8Display();
	virtual ~Chip8Display();

public:
	void Render();  // TODO: Accept pointer to display memory cache.

private:
	int Initialize();
	void InitializeStuff();
	void InitializeProgram();
	void InitializeTexture();
	int Terminate();
	void ReleaseStuff();
};

