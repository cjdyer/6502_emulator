#ifndef __BTYE_CODE_MEMORY_H__
#define __BTYE_CODE_MEMORY_H__

#include <cstdint>

// 64KB of memory
static constexpr uint32_t MEMORY_SIZE = 1024 * 64;

class ByteCodeMemory
{
public:
    ByteCodeMemory();
    ~ByteCodeMemory();

    virtual uint8_t read(uint16_t address);
    virtual void write(uint16_t address, uint8_t value);

private:
    uint8_t data[0x8000 + MEMORY_SIZE];
};

#endif // __BTYE_CODE_MEMORY_H__