#pragma once

#include <string>
#include <cstdint>

enum class InstructionType {
	I, R, S, SHIFT, B, J, U, LOAD, JALR, ENVIRONMENT, UNKNOWN
};

enum class Instruction {
    // I-type
    ADDI, ANDI, ORI, XORI, SLTI, SLTIU,
    // Shifts
    SLLI, SRLI, SRAI,
    // R-type
    ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT, SLTU,
    MUL, MULH, MULHU, MULHSU, DIV, DIVU, REM, REMU,
    // S-type
    SB, SH, SW,
    FSW,
    // B-type
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    // J-type
    JAL,
    // U-type
    LUI, AUIPC,
    // Loads
    LB, LH, LW, LBU, LHU,
    FLW,
    // JALR
    JALR,
    // Environment
    ECALL, EBREAK,
    // Unknown
    UNKNOWN
};

std::string instructionToString(Instruction);

struct DecodedInstruction {
    Instruction name{ Instruction::UNKNOWN }; // exact instruction: addi, slli, srai

	uint32_t rd{};
	uint32_t rs1{};
	uint32_t rs2{};

	int32_t imm{};
	uint32_t shamt{};

    InstructionType type{ InstructionType::UNKNOWN };
};

DecodedInstruction decodeInstruction(uint32_t);

uint32_t convertBinary(const std::string&);

uint32_t getOpCode(uint32_t);

uint32_t getRd(uint32_t);

uint32_t getFunct3(uint32_t);

uint32_t getRs1(uint32_t);

int32_t getImm(uint32_t);

int32_t getStoreImm(uint32_t);

int32_t getBranchImm(uint32_t);

int32_t getJalImm(uint32_t);

int32_t getUpperImm(uint32_t);

uint32_t getFunct7(uint32_t);

uint32_t getShamt(uint32_t);

uint32_t getRs2(uint32_t);
