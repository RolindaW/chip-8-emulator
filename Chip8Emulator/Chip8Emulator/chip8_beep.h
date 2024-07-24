#pragma once

#include <windows.h>
#include <mmsystem.h>

#include <fstream>

class Chip8Beep
{
public:
    Chip8Beep(char* filename);
    ~Chip8Beep();

public:
    void Play();
    void Stop();

private:
    char* buffer_;
    bool error_;
};

