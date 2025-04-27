#include <algorithm>  // std::clamp
#include <cstdlib>    // std::stoi
#include <iostream>

#include "chip8_args.h"
#include "chip8_defs.h"

Chip8Arguments Chip8ArgumentParser::Parse(int argc, char* argv[])
{
    Chip8Arguments args;
    args.cpu_hz = CHIP8_CPU_HZ_DEFAULT;  // Optional argument

    // Warning! Ignore first argument (program name)
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--rom" || arg == "-r")
        {
            if (i + 1 < argc)
            {
                args.rom_filename = argv[++i];  // Warning! (Pre-)Increment counter to avoid reprocessing argument value
            }
            else
            {
                // Error! Missing "--rom" argument value
                args.parse_error = true;
                break;
            }
        }
        else if (arg == "--cpu" || arg == "-c")
        {
            if (i + 1 < argc)
            {
                args.cpu_hz = ParseCpuHz(argv[++i]);
            }
            else
            {
                // Error! Missing "--cpu" argument value
                args.parse_error = true;
                break;
            }
        }
        else if (arg == "--help" || arg == "-h")
        {
            args.show_help = true;
            break;  // Warning! Exit parsing on help request
        }
        else
        {
            // Error! Unknown argument
            args.parse_error = true;
            break;
        }
    }

    // Validate if ROM filename provided when no help requested
    args.parse_error = (!args.show_help && args.rom_filename.empty());

    return args;
}

void Chip8ArgumentParser::PrintHelp()
{
    std::cout << "Usage:\n";
    std::cout << "  chip8_emulator [options]\n\n";

    std::cout << "Options:\n";
    std::cout << "  --rom, -r <value>\n";
    std::cout << "    Path to ROM file (required)\n";
    std::cout << "  --cpu, -c <value>\n";
    std::cout << "    CPU frequency (default " + std::to_string(CHIP8_CPU_HZ_DEFAULT) + " Hz)\n";
    std::cout << "  --help, -h\n";
    std::cout << "    Show this help message\n";

    std::cout << std::endl;
}

int Chip8ArgumentParser::ParseCpuHz(const std::string& value)
{
    int cpu_hz;
    try
    {
        cpu_hz = std::stoi(value);
    }
    catch (...)
    {
        // TODO: Handle error properly - notify the user
        cpu_hz = CHIP8_CPU_HZ_DEFAULT;
    }
    return std::clamp(cpu_hz, (int)CHIP8_CPU_HZ_MIN, (int)CHIP8_CPU_HZ_MAX);
}