#include <iostream>

#include "chip8_cpu.h"
#include "chip8_display.h"  // TODO: Remove when completing Display class dev and moving into CPU class.

int main()
{
    std::cout << "CHIP-8 Interpreter - Initialization!\n";

    // Test - Chip-8 Display
    Chip8Display display;

    while (true)
    {
        std::cout << "looping!";
        display.Render();
    }
    
    /*Chip8Cpu chip8_cpu;
    chip8_cpu.Start("C://workspace/chip-8-emulator/roms/test/IBM Logo.ch8");*/
}
