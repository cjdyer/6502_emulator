#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include <cstdint>
#include <memory>
#include "byte_code_memory.h"

enum class OpCode
{
    // Load/Store Operations
    LDA_IMM = 0xA9,
    LDX_IMM = 0xA2,
    LDY_IMM = 0xA0,
    STA_ZP = 0x85,
    STX_ZP = 0x86,
    STY_ZP = 0x84,
    LDA_ABS = 0xAD,
    LDX_ABS = 0xAE,
    LDY_ABS = 0xAC,
    STA_ABS = 0x8D,
    STX_ABS = 0x8E,
    STY_ABS = 0x8C,

    // Register Transfers
    TAX = 0xAA,
    TAY = 0xA8,
    TXA = 0x8A,
    TYA = 0x98,
    TSX = 0xBA,
    TXS = 0x9A,

    // Stack Operations
    PHA = 0x48,
    PHP = 0x08,
    PLA = 0x68,
    PLP = 0x28,

    // Logical
    AND_IMM = 0x29,
    AND_ZP = 0x25,
    EOR_IMM = 0x49,
    EOR_ZP = 0x45,
    ORA_IMM = 0x09,
    ORA_ZP = 0x05,
    BIT_ZP = 0x24,

    // Arithmetic
    ADC_IMM = 0x69,
    ADC_ZP = 0x65,
    SBC_IMM = 0xE9,
    SBC_ZP = 0xE5,
    CMP_IMM = 0xC9,
    CMP_ZP = 0xC5,
    CPX_IMM = 0xE0,
    CPX_ZP = 0xE4,
    CPY_IMM = 0xC0,
    CPY_ZP = 0xC4,

    // Increments & Decrements
    INC_ZP = 0xE6,
    INX = 0xE8,
    INY = 0xC8,
    DEC_ZP = 0xC6,
    DEX = 0xCA,
    DEY = 0x88,

    // Shifts
    ASL_ACC = 0x0A,
    ASL_ZP = 0x06,
    LSR_ACC = 0x4A,
    LSR_ZP = 0x46,
    ROL_ACC = 0x2A,
    ROL_ZP = 0x26,
    ROR_ACC = 0x6A,
    ROR_ZP = 0x66,

    // Jumps & Calls
    JMP_ABS = 0x4C,
    JSR_ABS = 0x20,
    RTS = 0x60,

    // Branches
    BPL = 0x10,
    BMI = 0x30,
    BVC = 0x50,
    BVS = 0x70,
    BCC = 0x90,
    BCS = 0xB0,
    BNE = 0xD0,
    BEQ = 0xF0,

    // Status Flag Changes
    CLC = 0x18,
    SEC = 0x38,
    CLI = 0x58,
    SEI = 0x78,
    CLV = 0xB8,
    CLD = 0xD8,
    SED = 0xF8,

    // System Functions
    BRK = 0x00,
    NOP = 0xEA,
    RTI = 0x40
};

class Processor
{
public:
    Processor(std::unique_ptr<ByteCodeMemory> byte_code_memory);
    ~Processor();

    void set_PC(uint16_t new_PC);
    OpCode fetch_opcode();
    void reset();
    void execute(OpCode opcode);

private:
    // Helper methods to manipulate flags
    enum StatusFlag : uint8_t;
    bool get_flag(StatusFlag flag) const;
    void set_flag(StatusFlag flag, bool value);
    void update_zero_and_negative_flags(uint8_t value);

    // Addressing modes
    uint8_t immediate();
    uint8_t zero_page();
    uint16_t absolute();

    // Instruction implementations
    void LDA(uint8_t value);
    void LDX(uint8_t value);
    void LDY(uint8_t value);
    void STA(uint16_t address);
    void STX(uint16_t address);
    void STY(uint16_t address);

    void TAX();
    void TAY();
    void TXA();
    void TYA();
    void TSX();
    void TXS();

    void PHA();
    void PHP();
    void PLA();
    void PLP();

    void AND(uint8_t value);
    void EOR(uint8_t value);
    void ORA(uint8_t value);
    void BIT(uint8_t value);

    void ADC(uint8_t value);
    void SBC(uint8_t value);
    void CMP(uint8_t value);
    void CPX(uint8_t value);
    void CPY(uint8_t value);

    void INC(uint16_t address);
    void INX();
    void INY();
    void DEC(uint16_t address);
    void DEX();
    void DEY();

    void ASL(uint16_t address);
    void LSR(uint16_t address);
    void ROL(uint16_t address);
    void ROR(uint16_t address);

    void JMP(uint16_t address);
    void JSR(uint16_t address);
    void RTS();

    void branch_if(bool condition);

    void CLC();
    void SEC();
    void CLI();
    void SEI();
    void CLV();
    void CLD();
    void SED();

    void BRK();
    void NOP();
    void RTI();

private:
    std::unique_ptr<ByteCodeMemory> memory;

    // Registers
    uint8_t A;      // Accumulator
    uint8_t X;      // X index register
    uint8_t Y;      // Y index register
    uint8_t status; // Status register (P)
    uint16_t PC;    // Program counter
    uint8_t SP;     // Stack pointer

    enum StatusFlag : uint8_t
    {
        CARRY = (1 << 0),
        ZERO = (1 << 1),
        INTERRUPT = (1 << 2),
        DECIMAL = (1 << 3),
        BREAK = (1 << 4),
        UNUSED = (1 << 5),
        OVERFLOW = (1 << 6),
        NEGATIVE = (1 << 7)
    };
};

#endif // __PROCESSOR_H__