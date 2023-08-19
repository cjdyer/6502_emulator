#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <cstdint>
#include <memory>
#include "byte_code_memory.h"

class CharacterDisplayDevice
{
public:
    void write(uint16_t address, uint8_t value);
    uint8_t read(uint16_t address);

private:
    void display_character(uint8_t ch);
    void clear();

private:
    uint8_t character;
};

class ExtendedMemory : public ByteCodeMemory
{
public:
    ExtendedMemory(std::unique_ptr<CharacterDisplayDevice> device);

    virtual uint8_t read(uint16_t address) override;
    virtual void write(uint16_t address, uint8_t value) override;

private:
    bool is_device_address(uint16_t address) const;

private:
    std::unique_ptr<CharacterDisplayDevice> device;
};

#endif // __DEVICE_H__