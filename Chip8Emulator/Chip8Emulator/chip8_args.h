#pragma once

#include <string>

struct Chip8Arguments
{
	std::string rom_filename;
	int cpu_hz;
	bool show_help = false;
	bool parse_error = false;
};

// TODO: Make static? Because only used once in program execution beginnig and does not save any state
class Chip8ArgumentParser
{
public:
	Chip8Arguments Parse(int argc, char* argv[]);
	void PrintHelp();

private:
	int ParseCpuHz(const std::string& value);
};

