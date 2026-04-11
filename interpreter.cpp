#include "decoder.h"
#include "interpreter.h"
#include <vector>
#include <array>
#include <iostream>
#include <cstdint>
#include <fstream>

void Memory::loadProgram(const std::string& fileName) {

	std::ifstream istrm(fileName, std::ios::binary);
	if (!istrm.is_open()) {
		std::cout << "failed to open " << fileName << '\n';
	}
	else {
		istrm.seekg(0, std::ios::end);
		size_t fileSize = istrm.tellg();
		istrm.seekg(0, std::ios::beg);
		istrm.read(reinterpret_cast<char*>(memory.data()), fileSize);
	}
}

void Core::writeReg(int regNum, uint32_t value) {
	if (regNum != 0) {
		registers[regNum] = value;
	}
}

void Core::writeFReg(int regNum, float value) {
	freg[regNum] = value;
}

uint32_t Core::readReg(int regNum) {
	return registers[regNum];
}

float Core::readFReg(int regNum) {
	return freg[regNum];
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
	uint32_t old_pc = pc;
	pc += 4;

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
			pc = old_pc + inst.imm;
		}
		break;
	}
	case Instruction::BNE: {
		if (readReg(inst.rs1) != readReg(inst.rs2)) {
			pc = old_pc + inst.imm;
		}
		break;
	}
	case Instruction::BLT: {
		if (static_cast<int32_t>(readReg(inst.rs1)) < static_cast<int32_t>(readReg(inst.rs2))) {
			pc = old_pc + inst.imm;
		}
		break;
	}
	case Instruction::BLTU: {
		if (readReg(inst.rs1) < readReg(inst.rs2)) {
			pc = old_pc + inst.imm;
		}
		break;
	}
	case Instruction::BGE: {
		if (static_cast<int32_t>(readReg(inst.rs1)) >= static_cast<int32_t>(readReg(inst.rs2))) {
			pc = old_pc + inst.imm;
		}
		break;
	}
	case Instruction::BGEU: {
		if (readReg(inst.rs1) >= readReg(inst.rs2)) {
			pc = old_pc + inst.imm;
		}
		break;
	}
	case Instruction::JAL: {
		writeReg(inst.rd, old_pc + 4);
		pc = old_pc + inst.imm;
		break;
	}
	case Instruction::JALR: {
		writeReg(inst.rd, old_pc + 4);
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
	case Instruction::SB: {
		uint32_t address = readReg(inst.rs1) + inst.imm;
		uint32_t value = readReg(inst.rs2);
		mem.writeByte(address, value);
		break;
	}
	case Instruction::SH: {
		uint32_t address = readReg(inst.rs1) + inst.imm;
		uint32_t value = readReg(inst.rs2);
		mem.writeHalfWord(address, value);
		break;
	}
	case Instruction::SW: {
		uint32_t address = readReg(inst.rs1) + inst.imm;
		uint32_t value = readReg(inst.rs2);
		mem.writeWord(address, value);
		break;
	}
	case Instruction::AUIPC: {
		uint32_t upperImmediate = old_pc + inst.imm;
		writeReg(inst.rd, upperImmediate);
		break;
	}
	case Instruction::LUI: {
		uint32_t upperImmediate = inst.imm;
		writeReg(inst.rd, upperImmediate);
		break;
	}
	case Instruction::MUL: {
		writeReg(inst.rd, readReg(inst.rs1) * readReg(inst.rs2));
		break;
	}
	case Instruction::MULH: {
		int64_t result = int64_t((int32_t)readReg(inst.rs1)) * int64_t((int32_t)readReg(inst.rs2));
		uint32_t upper = result >> 32;
		writeReg(inst.rd, upper);
		break;
	}
	case Instruction::MULHU: {
		uint64_t result = uint64_t((uint32_t)readReg(inst.rs1)) * uint64_t((uint32_t)readReg(inst.rs2));
		uint32_t upper = result >> 32;
		writeReg(inst.rd, upper);
		break;
	}
	case Instruction::MULHSU: {
		int64_t result = int64_t((int32_t)readReg(inst.rs1)) * int64_t((uint32_t)readReg(inst.rs2));
		uint32_t upper = result >> 32;
		writeReg(inst.rd, upper);
		break;
	}
	case Instruction::DIV: {
		int32_t dividend = (int32_t)readReg(inst.rs1);
		int32_t divisor = (int32_t)readReg(inst.rs2);
		int32_t result{};
		if (divisor == 0) {
			result = -1;
		}
		else if (dividend == INT32_MIN && divisor == -1) {
			result = INT32_MIN;
		}
		else {
			result = dividend / divisor;
		}
		writeReg(inst.rd, static_cast<uint32_t>(result));
		break;
	}
	case Instruction::DIVU: {
		uint32_t dividend = readReg(inst.rs1);
		uint32_t divisor = readReg(inst.rs2);
		uint32_t result = divisor == 0 ? -1 : (dividend / divisor);
		writeReg(inst.rd, result);
		break;
	}
	case Instruction::REM: {
		int32_t dividend = (int32_t)readReg(inst.rs1);
		int32_t divisor = (int32_t)readReg(inst.rs2);
		int32_t result{};
		if (divisor == 0) {
			result = dividend;
		}
		else if (dividend == INT32_MIN && divisor == -1) {
			result = 0;
		}
		else {
			result = dividend % divisor;
		}
		writeReg(inst.rd, static_cast<uint32_t>(result));
		break;
	}
	case Instruction::REMU: {
		uint32_t dividend = readReg(inst.rs1);
		uint32_t divisor = readReg(inst.rs2);
		uint32_t result = divisor == 0 ? dividend : (dividend % divisor);
		writeReg(inst.rd, result);
		break;
	}
	case Instruction::FLW: {
		uint32_t memory = mem.readWord(readReg(inst.rs1) + inst.imm);
		float f{};
		std::memcpy(&f, &memory, sizeof(f));
		writeFReg(inst.rd, f);
		break;
	}
	case Instruction::FSW: {
		uint32_t address = readReg(inst.rs1) + inst.imm;
		float f = readFReg(inst.rs2);
		uint32_t value{};
		std::memcpy(&value, &f, sizeof(value));
		mem.writeWord(address, value);
		break;
	}
	case Instruction::EBREAK: {
		halted = true;
		break;
	}
	case Instruction::ECALL: {
		halted = true;
		break;
	}
	case Instruction::UNKNOWN: {
		std::cout << "Unknown Instruction at PC: " << pc << '\n';
		halted = true;
		break;
	}
	}
}

void Core::run(Memory& mem) {
	while (!halted) {
		uint32_t rawInstruction = mem.readWord(pc);
		DecodedInstruction inst = decodeInstruction(rawInstruction);
		execute(inst, mem);
	}
}