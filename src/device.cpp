#include "logging.h"
#include "device.h"

void CharacterDisplayDevice::write(uint16_t address, uint8_t value)
{
    if (address % 2 == 0)
    {
        character = value;
    }
    else
    {
        if (value == 0x01)
        {
            display_character(character);
        }
        else if (value == 0x00)
        {
            clear();
        }
    }
}

uint8_t CharacterDisplayDevice::read(uint16_t address)
{
    return 0;
}

void CharacterDisplayDevice::display_character(uint8_t ch)
{
    LOG_INFO(static_cast<char>(ch));
}

void CharacterDisplayDevice::clear()
{
    LOG_INFO("[CLEAR]");
}

ExtendedMemory::ExtendedMemory(std::unique_ptr<CharacterDisplayDevice> device) : device(std::move(device)) {}

uint8_t ExtendedMemory::read(uint16_t address)
{
    if (is_device_address(address))
    {
        return device->read(address);
    }
    return ByteCodeMemory::read(address);
}

void ExtendedMemory::write(uint16_t address, uint8_t value)
{
    if (is_device_address(address))
    {
        device->write(address, value);
    }
    else
    {
        ByteCodeMemory::write(address, value);
    }
}

bool ExtendedMemory::is_device_address(uint16_t address) const
{
    return address >= 0xD000 && address <= 0xDFFF; // Assuming device occupies 0xD000-0xDFFF
}