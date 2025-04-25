#include "chip8_display.h"

Chip8Display::Chip8Display()
	: framebuffer_{ 0 }
{
	InitializeGL();
    Initialize();
}

Chip8Display::~Chip8Display()
{
	Terminate();
	TerminateGL();
}

// TODO: use when rendering is move outside display class into own renderer class
unsigned char* Chip8Display::GetFramebuffer()
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

void Chip8Display::Render(unsigned char* data)
{
	SetFilteredTextureData(data);
	Render();
}

void Chip8Display::Render()
{
	static const GLfloat color[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	glUseProgram(this->program_);

	glBindVertexArray(this->vao_);

	// Bind it to the context using the GL_TEXTURE_2D binding point
	glBindTexture(GL_TEXTURE_2D, this->texture2D_);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glfwSwapBuffers(this->window_);

	// Warning! Event polling is required to update state, but we are not actually using it for the moment
	glfwPollEvents();
}

// TODO: use scandcode instead of key to support different keyboard layouts (e.g. QUERTY, QZERTY)
bool Chip8Display::IsKeyPressed(unsigned char keyHex)
{
	bool keyPressed = false;

	int keyToken = MapKeyToken(keyHex);
	if (keyToken != GLFW_KEY_UNKNOWN)
	{
		glfwPollEvents();
		int keyState = glfwGetKey(this->window_, keyToken);
		keyPressed = (keyState == GLFW_PRESS);
	}
	
	return keyPressed;
}

bool Chip8Display::GetKeyPressed(unsigned char* keyHex)
{
	const unsigned char keyHexs[16] = { 0x1, 0x2, 0x3, 0xC, 0x4, 0x5, 0x6, 0xD, 0x7, 0x8, 0x9, 0xE, 0xA, 0x0, 0xB, 0xF };
	bool keyPressed = false;

	for (unsigned char i = 0; i < 16; i++)
	{
		if (IsKeyPressed(keyHexs[i]))
		{
			*keyHex = keyHexs[i];
			keyPressed = true;
			break;
		}
	}

	return keyPressed;
}

int Chip8Display::MapKeyToken(unsigned char keyHex)
{
	switch (keyHex)
	{
		case 0x1:
			return GLFW_KEY_1;
		case 0x2:
			return GLFW_KEY_2;
		case 0x3:
			return GLFW_KEY_3;
		case 0xC:
			return GLFW_KEY_4;
		case 0x4:
			return GLFW_KEY_Q;
		case 0x5:
			return GLFW_KEY_W;
		case 0x6:
			return GLFW_KEY_E;
		case 0xD:
			return GLFW_KEY_R;
		case 0x7:
			return GLFW_KEY_A;
		case 0x8:
			return GLFW_KEY_S;
		case 0x9:
			return GLFW_KEY_D;
		case 0xE:
			return GLFW_KEY_F;
		case 0xA:
			return GLFW_KEY_Z;
		case 0x0:
			return GLFW_KEY_X;
		case 0xB:
			return GLFW_KEY_C;
		case 0xF:
			return GLFW_KEY_V;
		default:
			return GLFW_KEY_UNKNOWN;
	}
}

// TODO: initialize GLFW safely (e.g. glfwSetErrorCallback)
// TODO: set callback on resize for updating the viewport (e.g. glfwSetFramebufferSizeCallback)
int Chip8Display::InitializeGL()
{
    if (!glfwInit())
        return 0;  //exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kOpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kOpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window_ = glfwCreateWindow(640, 480, "CHIP-8 Display", NULL, NULL);
    if (!this->window_)
    {
        glfwTerminate();
        return 0;  //exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->window_);

	// Warning! GL3W initialization returns 0 if success
    if (gl3wInit()) {
		// TODO: make console logging consistent across the projec
        fprintf(stderr, "failed to initialize OpenGL\n");
        return 0;
    }

    if (!gl3wIsSupported(kOpenGLVersionMajor, kOpenGLVersionMinor)) {
		// TODO: make console logging consistent across the projec
        fprintf(stderr, "OpenGL version %i.%i not supported\n", kOpenGLVersionMajor, kOpenGLVersionMinor);
        return 0;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));

    return 1;
}

void Chip8Display::TerminateGL()
{
    glfwDestroyWindow(this->window_);
    glfwTerminate();
}

void Chip8Display::Initialize()
{
    InitializeProgram();
	InitializeTexture();
}

void Chip8Display::InitializeProgram()
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

void Chip8Display::InitializeTexture()
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
	GLubyte data_bck[] =
	{
		0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
		0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78,
		0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8,
		0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8
	};

	GLubyte* data = new GLubyte[kDisplayResolution.total];
	for (int i = 0; i < kDisplayResolution.total; i++)
	{
		data[i] = unsigned char(0xFF * i / kDisplayResolution.total); //(i % 2 == 0) ? 0xFF : 0x00;
	}
	SetTextureData(data);
	delete[] data;

	// Wrapping
	glTextureParameteri(this->texture2D_, GL_TEXTURE_WRAP_S, GL_REPEAT);  // GL_REPEAT - GL_MIRRORED_REPEAT - GL_CLAMP_TO_EDGE - GL_CLAMP_TO_BORDER
	glTextureParameteri(this->texture2D_, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Border color
	const GLfloat borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTextureParameterfv(this->texture2D_, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Filtering
	glTextureParameteri(this->texture2D_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // GL_NEAREST - GL_LINEAR
	glTextureParameteri(this->texture2D_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Swizzle
	glTextureParameteri(this->texture2D_, GL_TEXTURE_SWIZZLE_G, GL_RED);
	glTextureParameteri(this->texture2D_, GL_TEXTURE_SWIZZLE_B, GL_RED);
}

void Chip8Display::SetFilteredTextureData(unsigned char* data)
{
	unsigned char* filteredData;
	FilterTextureData(data, &filteredData);
	SetTextureData(filteredData);
	delete[] filteredData;
}

void Chip8Display::SetTextureData(unsigned char* data)
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
void Chip8Display::FilterTextureData(unsigned char* rawData, unsigned char** filteredDataAdd)
{
	unsigned char* filter = new unsigned char[kDisplayResolution.total];
	for (int i = 0; i < kDisplayResolution.total; i++)
	{
		filter[i] = rawData[i] ? 0xFF : 0x00;
	}

	*filteredDataAdd = filter;
}

void Chip8Display::Terminate()
{
    glDeleteTextures(1, &texture2D_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteProgram(program_);
}
