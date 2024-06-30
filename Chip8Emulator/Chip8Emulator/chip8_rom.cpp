#include "chip8_rom.h"

Chip8Rom::Chip8Rom(std::string filename)
	: size_(0)
	, content_{0}
{
	this->filename_ = filename;
	this->Load();
}

Chip8Rom::~Chip8Rom()
{
	delete[] this->content_;
}

void Chip8Rom::Load()
{
	// Open the file
	FILE* file;
	fopen_s(&file, this->filename_.c_str(), "rb");
	if (file == NULL) return;

	// Read size of file
	fseek(file, 0, SEEK_END);
	this->size_ = ftell(file);
	fclose(file);

	// Read data to array of unsigned chars
	fopen_s(&file, this->filename_.c_str(), "rb");
	this->content_ = new unsigned char [this->size_];
	int bytes_read = fread(this->content_, sizeof(unsigned char), this->size_, file);
	fclose(file);
}


