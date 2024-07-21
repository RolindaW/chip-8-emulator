#include <iostream>

#include "chip8_cpu.h"

int main()
{
    std::cout << "CHIP-8 Interpreter - Initialization!\n";

    Chip8Cpu chip8_cpu;
    //chip8_cpu.Start("C://workspace/chip-8-emulator/roms/test/IBM Logo.ch8");
    chip8_cpu.Start("C://workspace/chip-8-emulator/roms/test/test_opcode_mod.ch8");

    std::cout << "CHIP-8 Interpreter - Finish!\n";
}
