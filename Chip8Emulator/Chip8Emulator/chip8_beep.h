#pragma once

#include <windows.h>
#include <mmsystem.h>

#include <fstream>

class Chip8Beep
{
private:
    char* buffer_;
    bool error_;
    bool isPlaying_;

public:
    Chip8Beep(char* filename);
    ~Chip8Beep();

public:
    void Play();
    void Stop();
};

