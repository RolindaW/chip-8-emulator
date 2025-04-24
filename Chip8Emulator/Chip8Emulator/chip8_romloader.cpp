#include <cstdio>
#include <stdexcept>

#include "chip8_romloader.h"
#include "chip8_rom.h"

Chip8Rom Chip8RomLoader::LoadFromFile(const std::string& filename)
{
    Chip8Rom rom(filename);

    // Open the file in binary mode
    FILE* file = nullptr;
    if (fopen_s(&file, filename.c_str(), "rb") != 0 || file == nullptr) {
        throw std::runtime_error("Failed to open ROM file: " + filename);
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size <= 0) {
        fclose(file);
        throw std::runtime_error("Invalid ROM file size: " + filename);
    }

    rom.size_ = static_cast<unsigned short>(file_size);
    rom.content_ = new unsigned char[rom.size_];

    // Read file contents
    size_t bytes_read = fread(rom.content_, sizeof(unsigned char), rom.size_, file);
    fclose(file);

    if (bytes_read != rom.size_) {
        delete[] rom.content_;
        rom.content_ = nullptr;
        throw std::runtime_error("Failed to read entire ROM file: " + filename);
    }

    return rom;
}
