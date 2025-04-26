#include "chip8_beep.h"

// TODO: Refactor this class: move sound file loading into a soundloader class (analog to rom and romloader classes)
Chip8Beep::Chip8Beep(char* filename)
    : error_ (true)
    , buffer_ (0)
    , isPlaying_(false)
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

    if (!this->isPlaying_)
    {
        // Play (async) sound (in loop) - Warning! Plays only .WAV sound files
        // It can be played by filename e.g. PlaySound(TEXT("C://workspace/chip-8-emulator/audio/Alarm01.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
        PlaySound((LPCWSTR)this->buffer_, NULL, SND_MEMORY | SND_LOOP | SND_ASYNC);
        this->isPlaying_ = true;
    }
}

void Chip8Beep::Stop()
{
    if (this->isPlaying_)
    {
        PlaySound(NULL, 0, 0);
        this->isPlaying_ = false;
    }
}

