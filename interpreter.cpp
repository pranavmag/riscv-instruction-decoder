#include "decoder.h"
#include "interpreter.h"
#include <vector>
#include <array>
#include <iostream>
#include <cstdint>

void Core::writeReg(int regNum, uint32_t value) {
	if (regNum != 0) {
		registers[regNum] = value;
	}
}

uint32_t Core::readReg(int regNum) {
	return registers[regNum];
}

void Core::execute(const DecodedInstruction& inst) {
	switch (inst.name) {
	case Instruction::ADDI: {
		writeReg(inst.rd, readReg(inst.rs1) + inst.imm);
		break;
	}
	case Instruction::ANDI: {
		writeReg(inst.rd, readReg(inst.rs1) & inst.imm);
		break;
	}
	case Instruction::ORI: {
		writeReg(inst.rd, readReg(inst.rs1) | inst.imm);
		break;
	}
	case Instruction::XORI: {
		writeReg(inst.rd, readReg(inst.rs1) ^ inst.imm);
		break;
	}
	case Instruction::SLTI: {
		uint32_t result = static_cast<int32_t>(readReg(inst.rs1)) < inst.imm ? 1 : 0;
		writeReg(inst.rd, result);
		break;
	}
	case Instruction::SLTIU: {
		uint32_t result = readReg(inst.rs1) < static_cast<uint32_t>(inst.imm) ? 1 : 0;
		writeReg(inst.rd, result);
		break;
	}
	case Instruction::SLLI: {
		writeReg(inst.rd, readReg(inst.rs1) << inst.shamt);
		break;
	}
	case Instruction::SRLI: {
		writeReg(inst.rd, readReg(inst.rs1) >> inst.shamt);
		break;
	}
	case Instruction::SRAI: {
		writeReg(inst.rd, static_cast<uint32_t>(static_cast<int32_t>(readReg(inst.rs1)) >> inst.shamt));
		break;
	}
	case Instruction::ADD: {
		writeReg(inst.rd, readReg(inst.rs1) + readReg(inst.rs2));
		break;
	}
	case Instruction::SUB: {
		writeReg(inst.rd, readReg(inst.rs1) - readReg(inst.rs2));
		break;
	}
	case Instruction::AND: {
		writeReg(inst.rd, readReg(inst.rs1) & readReg(inst.rs2));
		break;
	}
	case Instruction::OR: {
		writeReg(inst.rd, readReg(inst.rs1) | readReg(inst.rs2));
		break;
	}
	case Instruction::XOR: {
		writeReg(inst.rd, readReg(inst.rs1) ^ readReg(inst.rs2));
		break;
	}
	case Instruction::SLL: {
		writeReg(inst.rd, readReg(inst.rs1) << readReg(inst.rs2));
		break;
	}
	case Instruction::SRL: {
		writeReg(inst.rd, readReg(inst.rs1) >> readReg(inst.rs2));
		break;
	}
	case Instruction::SRA: {
		writeReg(inst.rd, static_cast<uint32_t>(static_cast<int32_t>(readReg(inst.rs1)) >> readReg(inst.rs2)));
		break;
	}
	case Instruction::SLT: {
		uint32_t result = (static_cast<int32_t>(readReg(inst.rs1)) < static_cast<int32_t>(readReg(inst.rs2))) ? 1 : 0;
		writeReg(inst.rd, result);
		break;
	}
	case Instruction::SLTU: {
		uint32_t result = (readReg(inst.rs1) < readReg(inst.rs2)) ? 1 : 0;
		writeReg(inst.rd, result);
		break;
	}



	}
}