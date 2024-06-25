#include <iostream>

#include "chip8_cpu.h"

int main()
{
    std::cout << "CHIP-8 Interpreter - Initialization!\n";

    Chip8Cpu chip8_cpu;
    chip8_cpu.Start();
}
