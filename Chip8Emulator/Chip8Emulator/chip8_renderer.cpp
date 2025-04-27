#include "chip8_renderer.h"

#include <iostream>

Chip8Renderer::Chip8Renderer()
{
	InitializeGL();
	InitializeObject();
}

Chip8Renderer::~Chip8Renderer()
{
	TerminateObject();
	TerminateGL();
}

void Chip8Renderer::Render(const unsigned char* data)
{
	SetFilteredTextureData(data);
	Render();
}

bool Chip8Renderer::WindowShouldClose()
{
	return glfwWindowShouldClose(this->window_);
}

void Chip8Renderer::PollEvents()
{
	glfwPollEvents();
}

bool Chip8Renderer::IsKeyPressed(int key)
{
	bool keyPressed = false;

	if (key != GLFW_KEY_UNKNOWN)
	{
		int keyState = glfwGetKey(this->window_, key);
		keyPressed = (keyState == GLFW_PRESS);
	}

	return keyPressed;
}

void GlfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW error: " << description << std::endl;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int Chip8Renderer::InitializeGL()
{
	// Warning! Set GLFW error callback before initialization (get errors notified both during and after initialization)
	glfwSetErrorCallback(GlfwErrorCallback);

	// Initialize GLFW library
	if (!glfwInit())
		return 0;  //exit(EXIT_FAILURE);

	// Create a window and a rendering context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kOpenGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kOpenGLVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window_ = glfwCreateWindow(640, 480, "CHIP-8 Display", NULL, NULL);
	if (!this->window_)
	{
		std::cerr << "Failed to create a window or its rendering context" << std::endl;
		glfwTerminate();
		return 0;  //exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(this->window_);

	// Set window framebuffer resize callback - Warning! Use to update viewport
	glfwSetFramebufferSizeCallback(this->window_, FramebufferSizeCallback);

	// Initialize GL3W library - Warning! GL3W initialization returns 0 if success
	// Load OpenGL extensions
	if (gl3wInit()) {
		std::cerr << "Failed to initialize OpenGL" << std::endl;
		return 0;
	}

	if (!gl3wIsSupported(kOpenGLVersionMajor, kOpenGLVersionMinor)) {
		std::cerr << "OpenGL version not supported" << std::endl;
		return 0;
	}

	std::cout << "OpenGL version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << std::endl;
	std::cout << "GLSL version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)) << std::endl;

	return 1;
}

void Chip8Renderer::TerminateGL()
{
	glfwDestroyWindow(this->window_);
	glfwTerminate();
}

void Chip8Renderer::InitializeObject()
{
	InitializeProgram();
	glUseProgram(this->program_);

	InitializeTexture();
	glBindVertexArray(this->vao_);
	glBindTexture(GL_TEXTURE_2D, this->texture2D_);
}

void Chip8Renderer::InitializeProgram()
{
	// Vertex shader
	const GLchar* vertexShaderSource[] = {
		"#version 450 core													\n"
		"																	\n"
		"out vec2 uv;														\n"
		"																	\n"
		"void main(void)													\n"
		"{																	\n"
		"	const vec4 vertices[4] = vec4[4](vec4(-0.75, -0.75, 0.5, 1.0),	\n"
		"									 vec4( 0.75, -0.75, 0.5, 1.0),	\n"
		"									 vec4(-0.75,  0.75, 0.5, 1.0),	\n"
		"									 vec4( 0.75,  0.75, 0.5, 1.0)); \n"
		"																	\n"
		"	const vec2 uvs[4] = vec2[4](vec2( 0.0, 0.0),					\n"
		"								vec2( 1.0, 0.0),					\n"
		"								vec2( 0.0, 1.0),					\n"
		"								vec2( 1.0, 1.0));					\n"
		"																	\n"
		"	gl_Position = vertices[gl_VertexID];							\n"
		"	uv = uvs[gl_VertexID];											\n"
		"}																	\n"
	};

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Fragment shader
	const GLchar* fragmentShaderSource[] = {
		"#version 450 core													\n"
		"																	\n"
		"uniform sampler2D s;												\n"
		"																	\n"
		"in vec2 uv;														\n"
		"																	\n"
		"out vec4 color;													\n"
		"																	\n"
		"void main(void)													\n"
		"{																	\n"
		"	color = texture(s, uv);											\n"
		"}																	\n"
	};

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Program
	this->program_ = glCreateProgram();
	glAttachShader(this->program_, vertexShader);
	glAttachShader(this->program_, fragmentShader);
	glLinkProgram(this->program_);

	// VAO
	glCreateVertexArrays(1, &this->vao_);

	// Free resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Chip8Renderer::InitializeTexture()
{
	// Create a new 2D texture object
	glCreateTextures(GL_TEXTURE_2D, 1, &this->texture2D_);

	// Specify the amount of storage we want to use for the texture
	glTextureStorage2D(this->texture2D_,
		1,
		GL_R8,
		kDisplayResolution.width, kDisplayResolution.height);

	// Define some data to upload into the texture
	// Note: Data is laid out (this setup can be changed in OpenGL with a parameter) left to right, top to bottom.
	// Ordering: first array element corresponds to lower row, first element; after a row is completed, jump to next row.
	//GLubyte data_bck[] =
	//{
	//	0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
	//	0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78,
	//	0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8,
	//	0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8
	//};

	GLubyte* data = new GLubyte[kDisplayResolution.total];
	for (int i = 0; i < kDisplayResolution.total; i++)
	{
		data[i] = unsigned char(0xFF * i / kDisplayResolution.total);  // Alternative: (i % 2 == 0) ? 0xFF : 0x00;
	}
	SetTextureData(data);
	delete[] data;

	// Wrapping
	glTextureParameteri(this->texture2D_, GL_TEXTURE_WRAP_S, GL_REPEAT);  // GL_REPEAT - GL_MIRRORED_REPEAT - GL_CLAMP_TO_EDGE - GL_CLAMP_TO_BORDER
	glTextureParameteri(this->texture2D_, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTextureParameteri(this->texture2D_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // GL_NEAREST - GL_LINEAR
	glTextureParameteri(this->texture2D_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Swizzle
	glTextureParameteri(this->texture2D_, GL_TEXTURE_SWIZZLE_G, GL_RED);
	glTextureParameteri(this->texture2D_, GL_TEXTURE_SWIZZLE_B, GL_RED);
}

void Chip8Renderer::TerminateObject()
{
	glDeleteTextures(1, &texture2D_);
	glDeleteVertexArrays(1, &vao_);
	glDeleteProgram(program_);
}

void Chip8Renderer::SetFilteredTextureData(const unsigned char* data)
{
	unsigned char* filteredData;
	FilterTextureData(data, &filteredData);
	SetTextureData(filteredData);
	delete[] filteredData;
}

void Chip8Renderer::SetTextureData(unsigned char* data)
{
	// Pixel store
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Fill texture with data
	glTextureSubImage2D(this->texture2D_,
		0,
		0, 0,
		kDisplayResolution.width, kDisplayResolution.height,
		GL_RED,
		GL_UNSIGNED_BYTE,
		data);
}

// Force monochrome texture data - Lock grayscale values
void Chip8Renderer::FilterTextureData(const unsigned char* rawData, unsigned char** filteredDataAdd)
{
	unsigned char* filter = new unsigned char[kDisplayResolution.total];
	for (int i = 0; i < kDisplayResolution.total; i++)
	{
		filter[i] = rawData[i] ? 0xFF : 0x00;
	}

	*filteredDataAdd = filter;
}

void Chip8Renderer::Render()
{
	static const GLfloat color[] = { 0.0f, 1.0f, 0.7f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glfwSwapBuffers(this->window_);
}
