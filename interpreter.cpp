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

uint32_t Memory::readByte(uint32_t addr) {
	return memory[addr];
}

uint32_t Memory::readHalfWord(uint32_t addr) {
	return memory[addr] |
		(memory[addr + 1] << 8);
}

uint32_t Memory::readWord(uint32_t addr) {
	return  memory[addr] |
		(memory[addr + 1] << 8) |
		(memory[addr + 2] << 16) |
		(memory[addr + 3] << 24);
}

void Memory::writeByte(uint32_t addr, uint32_t value) {
	memory[addr] = value & 0xFF;
}

void Memory::writeHalfWord(uint32_t addr, uint32_t value) {
	memory[addr] = value & 0xFF;
	memory[addr + 1] = (value >> 8) & 0xFF;
}

void Memory::writeWord(uint32_t addr, uint32_t value) {
	memory[addr] = value & 0xFF;
	memory[addr + 1] = (value >> 8) & 0xFF;
	memory[addr + 2] = (value >> 16) & 0xFF;
	memory[addr + 3] = (value >> 24) & 0xFF;
}

void Core::execute(const DecodedInstruction& inst, Memory& mem) {
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
	case Instruction::BEQ: {
		if (readReg(inst.rs1) == readReg(inst.rs2)) {
			pc = pc + inst.imm;
		}
		else {
			pc = pc + 4;
		}
		break;
	}
	case Instruction::BNE: {
		if (readReg(inst.rs1) != readReg(inst.rs2)) {
			pc = pc + inst.imm;
		}
		else {
			pc = pc + 4;
		}
		break;
	}
	case Instruction::BLT: {
		if (static_cast<int32_t>(readReg(inst.rs1)) < static_cast<int32_t>(readReg(inst.rs2))) {
			pc = pc + inst.imm;
		}
		else {
			pc = pc + 4;
		}
		break;
	}
	case Instruction::BLTU: {
		if (readReg(inst.rs1) < readReg(inst.rs2)) {
			pc = pc + inst.imm;
		}
		else {
			pc = pc + 4;
		}
		break;
	}
	case Instruction::BGE: {
		if (static_cast<int32_t>(readReg(inst.rs1)) >= static_cast<int32_t>(readReg(inst.rs2))) {
			pc = pc + inst.imm;
		}
		else {
			pc = pc + 4;
		}
		break;
	}
	case Instruction::BGEU: {
		if (readReg(inst.rs1) >= readReg(inst.rs2)) {
			pc = pc + inst.imm;
		}
		else {
			pc = pc + 4;
		}
		break;
	}
	case Instruction::JAL: {
		writeReg(inst.rd, pc + 4);
		pc = pc + inst.imm;
		break;
	}
	case Instruction::JALR: {
		writeReg(inst.rd, pc + 4);
		pc = (readReg(inst.rs1) + inst.imm) & 0xFFFFFFFE;
		break;
	}
	case Instruction::LB: {
		uint32_t memory = (mem.readByte(readReg(inst.rs1) + inst.imm));
		if (memory & 0x80) {
			memory = memory | 0xFFFFFF00;
		}
		writeReg(inst.rd, memory);
		break;
	}
	case Instruction::LBU: {
		uint32_t memory = mem.readByte(readReg(inst.rs1) + inst.imm);
		writeReg(inst.rd, memory);
		break;
	}

	case Instruction::LH: {
		uint32_t memory = mem.readHalfWord(readReg(inst.rs1) + inst.imm);
		if (memory & 0x8000) {
			memory = memory | 0xFFFF0000;
		}
		writeReg(inst.rd, memory);
		break;
	}
	case Instruction::LHU: {
		uint32_t memory = mem.readHalfWord(readReg(inst.rs1) + inst.imm);
		writeReg(inst.rd, memory);
		break;
	}
	case Instruction::LW: {
		uint32_t memory = mem.readWord(readReg(inst.rs1) + inst.imm);
		writeReg(inst.rd, memory);
		break;
	}


	}
}