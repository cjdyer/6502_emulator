#include <cstring>
#include <iostream>
#include "byte_code_memory.h"
#include "logging.h"

ByteCodeMemory::ByteCodeMemory()
{
    // Initialize memory as required
    memset(data, 0, sizeof(data));
}

ByteCodeMemory::~ByteCodeMemory() {}

uint8_t ByteCodeMemory::read(uint16_t address)
{
    // Wrap around if the address is out of bounds.
    address &= 0xFFFF;
    return data[address];
}

void ByteCodeMemory::write(uint16_t address, uint8_t value)
{
    // Wrap around if the address is out of bounds.
    address &= 0xFFFF;
    data[address] = value;

    if (address == 0xFF00)
    {
        LOG_INFO(static_cast<char>(data[address]));
    }
}