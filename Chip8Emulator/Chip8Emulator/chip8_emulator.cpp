#include <iostream>

#include "chip8_cpu.h"
#include "chip8_display.h"  // TODO: Remove when completing Display class dev and moving into CPU class.

int main()
{
    std::cout << "CHIP-8 Interpreter - Initialization!\n";

    // Test - Chip-8 Display
    Chip8Display display;

    unsigned char data[] =
    {
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
        0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00,
        0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0xFF, 0x00, 0x80, 0x00, 0x00, 0x00
    };

    while (true)
    {
        std::cout << "looping!";
        //display.Render();
        display.Render(data);
    }
    
    /*Chip8Cpu chip8_cpu;
    chip8_cpu.Start("C://workspace/chip-8-emulator/roms/test/IBM Logo.ch8");*/
}
