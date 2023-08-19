#include <iostream>
#include "processor.h"

Processor::Processor(std::unique_ptr<ByteCodeMemory> byte_code_memory) : memory(std::move(byte_code_memory)), A(0), X(0), Y(0), status(StatusFlag::UNUSED), PC(0), SP(0xFD) {}

Processor::~Processor()
{
}

void Processor::set_PC(uint16_t new_PC)
{
    PC = new_PC;
}

OpCode Processor::fetch_opcode()
{
    // Read OpCode and increment Program Counter
    return static_cast<OpCode>(memory->read(PC++));
}

void Processor::reset()
{
    // Set Program Counter to the reset vector
    uint8_t low_byte = memory->read(0xFFFC);
    uint8_t high_byte = memory->read(0xFFFD);
    PC = (high_byte << 8) | low_byte;

    // Reset the processor status. Most flags will be set to 0 on reset
    status = 0;

    // Reset other registers
    A = 0;
    X = 0;
    Y = 0;

    // Reset stack pointer to the top of the stack
    SP = 0xFD;
}

void Processor::execute(OpCode opcode)
{
    switch (opcode)
    {
    case OpCode::LDA_IMM:
        LDA(immediate());
        break;
    case OpCode::LDX_IMM:
        LDX(immediate());
        break;
    case OpCode::LDY_IMM:
        LDY(immediate());
        break;
    case OpCode::STA_ZP:
        STA(zero_page());
        break;
    case OpCode::STX_ZP:
        STX(zero_page());
        break;
    case OpCode::STY_ZP:
        STY(zero_page());
        break;
    case OpCode::LDA_ABS:
        LDA(absolute());
        break;
    case OpCode::LDX_ABS:
        LDX(absolute());
        break;
    case OpCode::LDY_ABS:
        LDY(absolute());
        break;
    case OpCode::STA_ABS:
        STA(absolute());
        break;
    case OpCode::STX_ABS:
        STX(absolute());
        break;
    case OpCode::STY_ABS:
        STY(absolute());
        break;

    case OpCode::TAX:
        TAX();
        break;
    case OpCode::TAY:
        TAY();
        break;
    case OpCode::TXA:
        TXA();
        break;
    case OpCode::TYA:
        TYA();
        break;
    case OpCode::TSX:
        TSX();
        break;
    case OpCode::TXS:
        TXS();
        break;

    case OpCode::PHA:
        PHA();
        break;
    case OpCode::PHP:
        PHP();
        break;
    case OpCode::PLA:
        PLA();
        break;
    case OpCode::PLP:
        PLP();
        break;

    case OpCode::AND_IMM:
        AND(immediate());
        break;
    case OpCode::AND_ZP:
        AND(zero_page());
        break;
    case OpCode::EOR_IMM:
        EOR(immediate());
        break;
    case OpCode::EOR_ZP:
        EOR(zero_page());
        break;
    case OpCode::ORA_IMM:
        ORA(immediate());
        break;
    case OpCode::ORA_ZP:
        ORA(zero_page());
        break;
    case OpCode::BIT_ZP:
        BIT(zero_page());
        break;

    case OpCode::ADC_IMM:
        ADC(immediate());
        break;
    case OpCode::ADC_ZP:
        ADC(zero_page());
        break;
    case OpCode::SBC_IMM:
        SBC(immediate());
        break;
    case OpCode::SBC_ZP:
        SBC(zero_page());
        break;
    case OpCode::CMP_IMM:
        CMP(immediate());
        break;
    case OpCode::CMP_ZP:
        CMP(zero_page());
        break;
    case OpCode::CPX_IMM:
        CPX(immediate());
        break;
    case OpCode::CPX_ZP:
        CPX(zero_page());
        break;
    case OpCode::CPY_IMM:
        CPY(immediate());
        break;
    case OpCode::CPY_ZP:
        CPY(zero_page());
        break;

    case OpCode::INC_ZP:
        INC(zero_page());
        break;
    case OpCode::INX:
        INX();
        break;
    case OpCode::INY:
        INY();
        break;
    case OpCode::DEC_ZP:
        DEC(zero_page());
        break;
    case OpCode::DEX:
        DEX();
        break;
    case OpCode::DEY:
        DEY();
        break;

    case OpCode::ASL_ACC:
        ASL(MEMORY_SIZE);
        break;
    case OpCode::ASL_ZP:
        ASL(zero_page());
        break;
    case OpCode::LSR_ACC:
        LSR(MEMORY_SIZE);
        break;
    case OpCode::LSR_ZP:
        LSR(zero_page());
        break;
    case OpCode::ROL_ACC:
        ROL(MEMORY_SIZE);
        break;
    case OpCode::ROL_ZP:
        ROL(zero_page());
        break;
    case OpCode::ROR_ACC:
        ROR(MEMORY_SIZE);
        break;
    case OpCode::ROR_ZP:
        ROR(zero_page());
        break;

    case OpCode::JMP_ABS:
        JMP(absolute());
        break;
    case OpCode::JSR_ABS:
        JSR(absolute());
        break;
    case OpCode::RTS:
        RTS();
        break;

    case OpCode::BPL:
        branch_if(!(status & NEGATIVE));
        break;
    case OpCode::BMI:
        branch_if(status & NEGATIVE);
        break;
    case OpCode::BVC:
        branch_if(!(status & OVERFLOW));
        break;
    case OpCode::BVS:
        branch_if(status & OVERFLOW);
        break;
    case OpCode::BCC:
        branch_if(!(status & CARRY));
        break;
    case OpCode::BCS:
        branch_if(status & CARRY);
        break;
    case OpCode::BNE:
        branch_if(!(status & ZERO));
        break;
    case OpCode::BEQ:
        branch_if(status & ZERO);
        break;

    case OpCode::CLC:
        CLC();
        break;
    case OpCode::SEC:
        SEC();
        break;
    case OpCode::CLI:
        CLI();
        break;
    case OpCode::SEI:
        SEI();
        break;
    case OpCode::CLV:
        CLV();
        break;
    case OpCode::CLD:
        CLD();
        break;
    case OpCode::SED:
        SED();
        break;

    case OpCode::BRK:
        BRK();
        break;
    case OpCode::NOP:
        NOP();
        break;
    case OpCode::RTI:
        RTI();
        break;

    default:
        std::cout << "Unknown OPCODE: " << std::hex << static_cast<uint8_t>(opcode) << std::endl;
        break;
    }
}

bool Processor::get_flag(StatusFlag flag) const
{
    return status & flag;
}

void Processor::set_flag(StatusFlag flag, bool value)
{
    // Set
    if (value)
    {
        status |= flag;
        return;
    }

    // Unset
    if (!value)
    {
        status &= ~flag;
        return;
    }
}

void Processor::update_zero_and_negative_flags(uint8_t value)
{
    // Update the zero flag
    set_flag(ZERO, value == 0);

    // Update the negative flag (check the 7th bit)
    set_flag(NEGATIVE, (value & 0x80) != 0);
}

uint8_t Processor::immediate()
{
    return memory->read(PC++);
}

uint8_t Processor::zero_page()
{
    return memory->read(PC++);
}

uint16_t Processor::absolute()
{
    uint16_t low_byte = memory->read(PC++);
    uint16_t high_byte = memory->read(PC++);
    return (high_byte << 8) | low_byte;
}

// Load/Store Operations
void Processor::LDA(uint8_t value)
{
    A = value;
    update_zero_and_negative_flags(A);
}

void Processor::LDX(uint8_t value)
{
    X = value;
    update_zero_and_negative_flags(X);
}

void Processor::LDY(uint8_t value)
{
    Y = value;
    update_zero_and_negative_flags(Y);
}

void Processor::STA(uint16_t address)
{
    memory->write(address, A);
}

void Processor::STX(uint16_t address)
{
    memory->write(address, X);
}

void Processor::STY(uint16_t address)
{
    memory->write(address, Y);
}

// Register Transfers
void Processor::TAX()
{
    X = A;
    update_zero_and_negative_flags(X);
}

void Processor::TAY()
{
    Y = A;
    update_zero_and_negative_flags(Y);
}

void Processor::TXA()
{
    A = X;
    update_zero_and_negative_flags(A);
}

void Processor::TYA()
{
    A = Y;
    update_zero_and_negative_flags(A);
}

void Processor::TSX()
{
    X = SP;
    update_zero_and_negative_flags(X);
}

void Processor::TXS()
{
    SP = X;
    // TXS does not affect the processor status flags.
}

// Stack
void Processor::PHA()
{
    memory->write(0x0100 + SP, A);
    SP--;
}

void Processor::PHP()
{
    memory->write(0x0100 + SP, status);
    SP--;
}

void Processor::PLA()
{
    SP++;
    A = memory->read(0x0100 + SP);
    update_zero_and_negative_flags(A);
}

void Processor::PLP()
{
    SP++;
    status = memory->read(0x0100 + SP);
}

// Logical
void Processor::AND(uint8_t value)
{
    A &= value;
    update_zero_and_negative_flags(A);
}

void Processor::EOR(uint8_t value)
{
    A ^= value;
    update_zero_and_negative_flags(A);
}

void Processor::ORA(uint8_t value)
{
    A |= value;
    update_zero_and_negative_flags(A);
}

void Processor::BIT(uint8_t value)
{
    uint8_t result = A & value;

    status = (status & ~ZERO) | (result == 0 ? ZERO : 0);
    status = (status & ~NEGATIVE) | (value & NEGATIVE);
    status = (status & ~OVERFLOW) | (value & OVERFLOW);
}

// Arithmetic
void Processor::ADC(uint8_t value)
{
    // Start by adding the accumulator, the value, and the current carry bit together
    uint16_t temp = static_cast<uint16_t>(A) + value + (status & CARRY);
    // Clear the flags that will be affected by the ADC operation
    status &= ~(CARRY | ZERO | OVERFLOW | NEGATIVE);

    // If the result is 256 or above (i.e., only fits in 9 bits), the carry flag should be set
    if (temp & 0x0100)
    {
        status |= CARRY;
    }

    // If the 7th bit (sign bit in 2's complement) of the result is set, set the negative flag
    if (temp & 0x0080)
    {
        status |= NEGATIVE;
    }

    // Check for overflow. Overflow in addition occurs if both operands have the same sign
    // but their sum has a different sign. We use bitwise XOR to check for this condition.
    if (!((A ^ value) & 0x0080) && ((A ^ temp) & 0x0080))
    {
        status |= OVERFLOW;
    }

    // Store the 8-bit result into the accumulator
    A = static_cast<uint8_t>(temp);

    // If the result is 0, set the zero flag
    if (A == 0)
    {
        status |= ZERO;
    }
}

void Processor::SBC(uint8_t value)
{
    // Use two's complement arithmetic to handle subtraction
    ADC(value ^ 0xFF);
}

void Processor::CMP(uint8_t value)
{
    uint8_t result = A - value;
    status &= ~(CARRY | ZERO | NEGATIVE);

    if (A >= value)
    {
        status |= CARRY;
    }
    if (A == value)
    {
        status |= ZERO;
    }
    if (result & 0x80)
    {
        status |= NEGATIVE;
    }
}

void Processor::CPX(uint8_t value)
{
    uint8_t result = X - value;
    status &= ~(CARRY | ZERO | NEGATIVE);

    if (X >= value)
    {
        status |= CARRY;
    }
    if (X == value)
    {
        status |= ZERO;
    }
    if (result & 0x80)
    {
        status |= NEGATIVE;
    }
}

void Processor::CPY(uint8_t value)
{
    uint8_t result = Y - value;
    status &= ~(CARRY | ZERO | NEGATIVE);

    if (Y >= value)
    {
        status |= CARRY;
    }
    if (Y == value)
    {
        status |= ZERO;
    }
    if (result & 0x80)
    {
        status |= NEGATIVE;
    }
}

// Increments & Decrements
void Processor::INC(uint16_t address)
{
    uint8_t value = memory->read(address);
    value++;
    memory->write(address, value);
    update_zero_and_negative_flags(value);
}

void Processor::INX()
{
    X++;
    update_zero_and_negative_flags(X);
}

void Processor::INY()
{
    Y++;
    update_zero_and_negative_flags(Y);
}

void Processor::DEC(uint16_t address)
{
    uint8_t value = memory->read(address);
    value--;
    memory->write(address, value);
    update_zero_and_negative_flags(value);
}

void Processor::DEX()
{
    X--;
    update_zero_and_negative_flags(X);
}

void Processor::DEY()
{
    Y--;
    update_zero_and_negative_flags(Y);
}

// Shifts
void Processor::ASL(uint16_t address)
{
    uint8_t value = memory->read(address);
    if (address == MEMORY_SIZE)
    {
        // Special address for accumulator
        value = A;
    }

    // Clear the CARRY flag
    status &= ~CARRY;
    if (value & 0x80)
    {
        // Check if highest bit is set
        status |= CARRY;
    }

    // Shift left by one bit
    value <<= 1;
    update_zero_and_negative_flags(value);

    if (address == MEMORY_SIZE)
    {
        A = value;
    }
    else
    {
        memory->write(address, value);
    }
}

void Processor::LSR(uint16_t address)
{
    uint8_t value = memory->read(address);
    if (address == MEMORY_SIZE)
    {
        // Special address for accumulator
        value = A;
    }

    status &= ~CARRY;
    if (value & 0x01)
    {
        status |= CARRY;
    }

    // Shift right by one bit
    value >>= 1;
    update_zero_and_negative_flags(value);

    if (address == MEMORY_SIZE)
    {
        A = value;
    }
    else
    {
        memory->write(address, value);
    }
}

void Processor::ROL(uint16_t address)
{
    uint8_t value = memory->read(address);
    if (address == MEMORY_SIZE)
    {
        value = A;
    }

    // Store the current high bit
    uint8_t new_carry = (value & 0x80) ? 1 : 0;
    value <<= 1;
    if (status & CARRY)
    {
        // Set the low bit if CARRY was set
        value |= 0x01;
    }

    status &= ~CARRY;
    if (new_carry)
    {
        status |= CARRY;
    }

    update_zero_and_negative_flags(value);

    if (address == MEMORY_SIZE)
    {
        A = value;
    }
    else
    {
        memory->write(address, value);
    }
}

void Processor::ROR(uint16_t address)
{
    uint8_t value = memory->read(address);
    if (address == MEMORY_SIZE)
    {
        value = A;
    }

    // Store the current low bit
    uint8_t new_carry = value & 0x01;
    value >>= 1;
    if (status & CARRY)
    {
        // Set the high bit if CARRY was set
        value |= 0x80;
    }

    status &= ~CARRY;
    if (new_carry)
    {
        status |= CARRY;
    }

    update_zero_and_negative_flags(value);

    if (address == MEMORY_SIZE)
    {
        A = value;
    }
    else
    {
        memory->write(address, value);
    }
}

// Jumps & Calls
void Processor::JMP(uint16_t address)
{
    PC = address;
}

void Processor::JSR(uint16_t address)
{
    // Push the return address - 1 onto the stack.
    // The -1 is because when returning with RTS, the PC is incremented after fetching the address
    uint16_t return_address = PC - 1;
    // Push high byte
    memory->write(0x0100 + SP, return_address >> 8);
    SP--;
    // Push low byte
    memory->write(0x0100 + SP, return_address & 0xFF);
    SP--;

    // Jump to subroutine
    PC = address;
}

void Processor::RTS()
{
    SP++;
    uint8_t low_byte = memory->read(0x0100 + SP);
    SP++;
    uint8_t high_byte = memory->read(0x0100 + SP);

    PC = (high_byte << 8) | low_byte;
    // Increment PC because the saved address was -1 from the actual return address
    PC++;
}

// Branches
void Processor::branch_if(bool condition)
{
    // Read signed byte
    int8_t offset = static_cast<int8_t>(memory->read(PC++));

    if (condition)
    {
        // If branch is taken, adjust the program counter by the offset.
        PC += offset;
    }
}

// Status Flag Changes
void Processor::CLC()
{
    status &= ~CARRY;
}

void Processor::SEC()
{
    status |= CARRY;
}

void Processor::CLI()
{
    status &= ~INTERRUPT;
}

void Processor::SEI()
{
    status |= INTERRUPT;
}

void Processor::CLV()
{
    status &= ~OVERFLOW;
}

void Processor::CLD()
{
    status &= ~DECIMAL;
}

void Processor::SED()
{
    status |= DECIMAL;
}

// System Functions
void Processor::BRK()
{
    // Increment PC to skip the padding byte after the BRK opcode.
    PC++;

    // Push the program counter and status onto the stack.
    memory->write(0x0100 + SP, (PC >> 8) & 0xFF);
    SP--;
    memory->write(0x0100 + SP, PC & 0xFF);
    SP--;

    // Set the Break flag.
    uint8_t statusWithBreak = status | BREAK;
    memory->write(0x0100 + SP, statusWithBreak);
    SP--;

    // Load interrupt vector and jump to the interrupt routine.
    uint8_t low_byte = memory->read(0xFFFE);
    uint8_t high_byte = memory->read(0xFFFF);
    PC = (high_byte << 8) | low_byte;
}

void Processor::NOP()
{
    // Do nothing.
}

void Processor::RTI()
{
    // Pull the processor status from the stack.
    SP++;
    status = memory->read(0x0100 + SP);

    // Pull the program counter from the stack.
    SP++;
    uint8_t low_byte = memory->read(0x0100 + SP);
    SP++;
    uint8_t high_byte = memory->read(0x0100 + SP);
    PC = (high_byte << 8) | low_byte;
}