#include "chip8_beep.h"

Chip8Beep::Chip8Beep(char* filename)
    : error_ (true)
    , buffer_ (0)
{
    std::ifstream infile(filename, std::ios::binary);

    if (!infile)
    {
        return;
    }

    infile.seekg(0, std::ios::end);  // Get file length
    int length = infile.tellg();
    this->buffer_ = new char[length];  // Allocate memory
    infile.seekg(0, std::ios::beg);  // File start position
    infile.read(this->buffer_, length);  // Read entire file

    infile.close();
    error_ = false;
}

Chip8Beep::~Chip8Beep()
{
    Stop();
    delete[] buffer_;
}

void Chip8Beep::Play()
{
    if (this->error_)
    {
        return;
    }

    // Warning! From filename (.WAV!) - PlaySound(TEXT("C://workspace/chip-8-emulator/audio/Alarm01.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    PlaySound((LPCWSTR)this->buffer_, NULL, SND_MEMORY | SND_LOOP | SND_ASYNC);
}

void Chip8Beep::Stop()
{
    PlaySound(NULL, 0, 0);
}

