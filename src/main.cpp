#include "logging.h"
#include "processor.h"
#include "device.h"
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

const std::map<std::string, OpCode> OPCODE_MAP = {
    {"LDA", OpCode::LDA_IMM},
    {"STA", OpCode::STA_ABS},
    {"BRK", OpCode::BRK}};

std::vector<uint8_t> interpret(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<uint8_t> byte_code;

    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;

        LOG_DEBUG("Processing line " + std::to_string(lineNumber) + ": " + line);

        std::istringstream iss(line);
        std::string instruction;
        iss >> instruction;

        if (OPCODE_MAP.find(instruction) != OPCODE_MAP.end())
        {
            OpCode op_code = OPCODE_MAP.at(instruction);
            byte_code.push_back(static_cast<uint8_t>(op_code));

            LOG_DEBUG("  Matched instruction: " + instruction + " to opcode: " + std::to_string(static_cast<int>(op_code)));

            switch (op_code)
            {
            case OpCode::LDA_IMM:
            {
                uint16_t value;
                iss >> std::hex >> value;
                byte_code.push_back(static_cast<uint8_t>(value));

                LOG_DEBUG("    Loaded immediate value: " + std::to_string((int)value));
            }
            break;

            case OpCode::STA_ABS:
            {
                uint16_t address;
                iss >> std::hex >> address;
                byte_code.push_back(static_cast<uint8_t>(address & 0xFF));
                byte_code.push_back(static_cast<uint8_t>((address >> 8) & 0xFF));

                LOG_DEBUG("    Stored value to absolute address: " + std::to_string((int)address));
            }
            break;

            default:
                LOG_DEBUG("    Instruction doesn't require additional bytes.");
                break;
            }
        }
        else
        {
            LOG_WARN("  Instruction " + instruction + " not found in opcode map.");
        }

        LOG_DEBUG("");
    }

    return byte_code;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_asm_file>" << std::endl;
        exit(1);
    }

    std::string asm_file_path = argv[1];

    // Create memory
    auto device = std::make_unique<CharacterDisplayDevice>();
    std::unique_ptr<ExtendedMemory> memory = std::make_unique<ExtendedMemory>(std::move(device));

    LOG_INFO("Initialized ByteCodeMemory.");

    // Write memory
    std::vector<uint8_t> program = interpret(asm_file_path);
    LOG_INFO("Interpreted program.asm into " + std::to_string(program.size()) + " bytes.");
    for (size_t i = 0; i < program.size(); i++)
    {
        memory->write(0x8000 + i, static_cast<uint8_t>(program[i]));
        LOG_DEBUG("Wrote byte " + std::to_string(i) + " to address " + std::to_string(0x8000 + i) + ": " + std::to_string(static_cast<int>(program[i])));
    }

    // Setup processor
    Processor cpu(std::move(memory));
    LOG_INFO("Initialized Processor and set memory.");
    cpu.reset();
    LOG_INFO("Processor reset.");
    cpu.set_PC(0x8000);
    LOG_INFO("Program Counter set to 0x8000.");

    // Run code
    int steps = 0;
    while (true)
    {
        OpCode op_code = cpu.fetch_opcode();
        LOG_DEBUG("Step " + std::to_string(steps++) + ": Fetched opcode " + std::to_string(static_cast<int>(op_code)));
        if (op_code == OpCode::BRK)
        {
            LOG_INFO("Encountered BRK. Exiting loop.");
            break;
        }

        cpu.execute(op_code);
        LOG_DEBUG("Executed opcode.");
    }

    LOG_INFO("Program completed after " + std::to_string(steps) + " steps.");
    return 0;
}
