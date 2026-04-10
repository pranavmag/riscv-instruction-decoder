#include "decoder.h"
#include <iostream>
#include <string>
#include <cstdint>

uint32_t convertBinary(const std::string& binaryString) {
	uint32_t value{};

	for (char c : binaryString) {
		if (c != '0' && c != '1') {
			std::cout << "Invalid binary input\n";
			return 0;
		}
		int bit = c - '0';
		value = (value << 1) | bit;
	}

	return value;
}

uint32_t getOpCode(uint32_t value) {
	uint32_t opcode = value & 0x7F;

	return opcode;
}

uint32_t getRd(uint32_t value) {
	uint32_t rd = (value >> 7) & 0x1F;

	return rd;
}

uint32_t getFunct3(uint32_t value) {
	uint32_t funct3 = (value >> 12) & 0x7;

	return funct3;
}

uint32_t getRs1(uint32_t value) {
	uint32_t rs1 = (value >> 15) & 0x1F;

	return rs1;
}

int32_t getImm(uint32_t value) {
	uint32_t imm = (value >> 20) & 0xFFF;

	if (imm & 0x800) {
		return imm | 0xFFFFF000;
	}

	return imm;
}

int32_t getStoreImm(uint32_t value) {
	uint32_t imm_11_5 = (value >> 25) & 0x7F;

	uint32_t imm_4_0 = (value >> 7) & 0x1F;

	uint32_t imm = (imm_11_5 << 5) | imm_4_0;

	if (imm & 0x800) {
		return imm | 0xFFFFF000;
	}

	return imm;
}

int32_t getBranchImm(uint32_t value) {
	uint32_t imm_12 = (value >> 31) & 0x1;

	uint32_t imm_10_5 = (value >> 25) & 0x3F;

	uint32_t imm_4_1 = (value >> 8) & 0xF;

	uint32_t imm_11 = (value >> 7) & 0x1;

	uint32_t imm = (imm_12 << 12) | (imm_10_5 << 5) | (imm_4_1 << 1) | (imm_11 << 11);

	if (imm & 0x1000) {
		return imm | 0xFFFFE000;
	}

	return imm;

}

int32_t getJalImm(uint32_t value) {
	uint32_t imm_20 = (value >> 31) & 0x1;

	uint32_t imm_10_1 = (value >> 21) & 0x3FF;

	uint32_t imm_11 = (value >> 20) & 0x1;

	uint32_t imm_19_12 = (value >> 12) & 0xFF;

	uint32_t imm = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);

	if (imm & 0x100000) {
		return imm | 0xFFE00000;
	}

	return imm;
}

int32_t getUpperImm(uint32_t value) {
	uint32_t imm = value & 0xFFFFF000;

	return imm;
}

uint32_t getFunct7(uint32_t value) {
	uint32_t funct7 = (value >> 25) & 0x7F;

	return funct7;
}

uint32_t getShamt(uint32_t value) {
	uint32_t shamt = (value >> 20) & 0x1F;

	return shamt;
}

uint32_t getRs2(uint32_t value) {
	uint32_t rs2 = (value >> 20) & 0x1F;

	return rs2;
}

std::string instructionToString(Instruction inst) {
	switch (inst) {
		// Immediate Arithmetic
	case Instruction::ADDI:   return "addi";
	case Instruction::ANDI:   return "andi";
	case Instruction::ORI:    return "ori";
	case Instruction::XORI:   return "xori";
	case Instruction::SLTI:   return "slti";
	case Instruction::SLTIU:  return "sltiu";

		// Immediate Shifts
	case Instruction::SLLI:   return "slli";
	case Instruction::SRLI:   return "srli";
	case Instruction::SRAI:   return "srai";

		// Register-Register Arithmetic
	case Instruction::ADD:    return "add";
	case Instruction::SUB:    return "sub";
	case Instruction::AND:    return "and";
	case Instruction::OR:     return "or";
	case Instruction::XOR:    return "xor";
	case Instruction::SLL:    return "sll";
	case Instruction::SRL:    return "srl";
	case Instruction::SRA:    return "sra";
	case Instruction::SLT:    return "slt";
	case Instruction::SLTU:   return "sltu";

		// Stores
	case Instruction::SB:     return "sb";
	case Instruction::SH:     return "sh";
	case Instruction::SW:     return "sw";

		// Loads
	case Instruction::LB:     return "lb";
	case Instruction::LBU:    return "lbu";
	case Instruction::LH:     return "lh";
	case Instruction::LHU:    return "lhu";
	case Instruction::LW:     return "lw";

		// Branches
	case Instruction::BEQ:    return "beq";
	case Instruction::BNE:    return "bne";
	case Instruction::BLT:    return "blt";
	case Instruction::BLTU:   return "bltu";
	case Instruction::BGE:    return "bge";
	case Instruction::BGEU:   return "bgeu";

		// Jumps & Upper Immediates
	case Instruction::JAL:    return "jal";
	case Instruction::JALR:   return "jalr";
	case Instruction::LUI:    return "lui";
	case Instruction::AUIPC:  return "auipc";

		// System
	case Instruction::ECALL:  return "ecall";
	case Instruction::EBREAK: return "ebreak";

		// Default
	case Instruction::UNKNOWN:
	default:                  return "unknown";
	}
}

DecodedInstruction decodeInstruction(uint32_t value) {
	DecodedInstruction inst{};

	uint32_t opcode{ getOpCode(value) };
	uint32_t funct3{ getFunct3(value) };
	uint32_t funct7{ getFunct7(value) };

	if (opcode == 0x13 && funct3 == 0x0) {
		inst.name = Instruction::ADDI;
		inst.type = InstructionType::I;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x13 && funct3 == 0x7) {
		inst.name = Instruction::ANDI;
		inst.type = InstructionType::I;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x13 && funct3 == 0x6) {
		inst.name = Instruction::ORI;
		inst.type = InstructionType::I;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x13 && funct3 == 0x4) {
		inst.name = Instruction::XORI;
		inst.type = InstructionType::I;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x13 && funct3 == 0x2) {
		inst.name = Instruction::SLTI;
		inst.type = InstructionType::I;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x13 && funct3 == 0x3) {
		inst.name = Instruction::SLTIU;
		inst.type = InstructionType::I;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x13 && funct3 == 0x1 && funct7 == 0x0) {
		inst.name = Instruction::SLLI;
		inst.type = InstructionType::SHIFT;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.shamt = getShamt(value);
	}
	else if (opcode == 0x13 && funct3 == 0x5 && funct7 == 0x0) {
		inst.name = Instruction::SRLI;
		inst.type = InstructionType::SHIFT;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.shamt = getShamt(value);
	}
	else if (opcode == 0x13 && funct3 == 0x5 && funct7 == 0x20) {
		inst.name = Instruction::SRAI;
		inst.type = InstructionType::SHIFT;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.shamt = getShamt(value);
	}
	else if (opcode == 0x33 && funct3 == 0x0 && funct7 == 0x0) {
		inst.name = Instruction::ADD;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x0 && funct7 == 0x20) {
		inst.name = Instruction::SUB;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x7 && funct7 == 0x0) {
		inst.name = Instruction::AND;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x6 && funct7 == 0x0) {
		inst.name = Instruction::OR;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x4 && funct7 == 0x0) {
		inst.name = Instruction::XOR;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x1 && funct7 == 0x0) {
		inst.name = Instruction::SLL;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x5 && funct7 == 0x0) {
		inst.name = Instruction::SRL;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x5 && funct7 == 0x20) {
		inst.name = Instruction::SRA;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x2 && funct7 == 0x0) {
		inst.name = Instruction::SLT;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x3 && funct7 == 0x0) {
		inst.name = Instruction::SLTU;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x23 && funct3 == 0x0) {
		inst.name = Instruction::SB;
		inst.type = InstructionType::S;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getStoreImm(value);
	}
	else if (opcode == 0x23 && funct3 == 0x1) {
		inst.name = Instruction::SH;
		inst.type = InstructionType::S;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getStoreImm(value);
	}
	else if (opcode == 0x23 && funct3 == 0x2) {
		inst.name = Instruction::SW;
		inst.type = InstructionType::S;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getStoreImm(value);
	}
	else if (opcode == 0x63 && funct3 == 0x0) {
		inst.name = Instruction::BEQ;
		inst.type = InstructionType::B;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getBranchImm(value);
	}
	else if (opcode == 0x63 && funct3 == 0x1) {
		inst.name = Instruction::BNE;
		inst.type = InstructionType::B;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getBranchImm(value);
	}
	else if (opcode == 0x63 && funct3 == 0x4) {
		inst.name = Instruction::BLT;
		inst.type = InstructionType::B;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getBranchImm(value);
	}
	else if (opcode == 0x63 && funct3 == 0x6) {
		inst.name = Instruction::BLTU;
		inst.type = InstructionType::B;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getBranchImm(value);
	}
	else if (opcode == 0x63 && funct3 == 0x5) {
		inst.name = Instruction::BGE;
		inst.type = InstructionType::B;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getBranchImm(value);
	}
	else if (opcode == 0x63 && funct3 == 0x7) {
		inst.name = Instruction::BGEU;
		inst.type = InstructionType::B;
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
		inst.imm = getBranchImm(value);
	}
	else if (opcode == 0x6F) {
		inst.name = Instruction::JAL;
		inst.type = InstructionType::J;
		inst.rd = getRd(value);
		inst.imm = getJalImm(value);
	}
	else if (opcode == 0x37) {
		inst.name = Instruction::LUI;
		inst.type = InstructionType::U;
		inst.rd = getRd(value);
		inst.imm = getUpperImm(value);
	}
	else if (opcode == 0x17) {
		inst.name = Instruction::AUIPC;
		inst.type = InstructionType::U;
		inst.rd = getRd(value);
		inst.imm = getUpperImm(value);
	}
	else if (opcode == 0x3 && funct3 == 0x0) {
		inst.name = Instruction::LB;
		inst.type = InstructionType::LOAD;
		inst.rs1 = getRs1(value);
		inst.rd = getRd(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x3 && funct3 == 0x4) {
		inst.name = Instruction::LBU;
		inst.type = InstructionType::LOAD;
		inst.rs1 = getRs1(value);
		inst.rd = getRd(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x3 && funct3 == 0x1) {
		inst.name = Instruction::LH;
		inst.type = InstructionType::LOAD;
		inst.rs1 = getRs1(value);
		inst.rd = getRd(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x3 && funct3 == 0x5) {
		inst.name = Instruction::LHU;
		inst.type = InstructionType::LOAD;
		inst.rs1 = getRs1(value);
		inst.rd = getRd(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x3 && funct3 == 0x2) {
		inst.name = Instruction::LW;
		inst.type = InstructionType::LOAD;
		inst.rs1 = getRs1(value);
		inst.rd = getRd(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x33 && funct3 == 0x0 && funct7 == 0x1) {
		inst.name = Instruction::MUL;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x1 && funct7 == 0x1) {
		inst.name = Instruction::MULH;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x3 && funct7 == 0x1) {
		inst.name = Instruction::MULHU;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x2 && funct7 == 0x1) {
		inst.name = Instruction::MULHSU;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x4 && funct7 == 0x1) {
		inst.name = Instruction::DIV;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x5 && funct7 == 0x1) {
		inst.name = Instruction::DIVU;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x6 && funct7 == 0x1) {
		inst.name = Instruction::REM;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x33 && funct3 == 0x7 && funct7 == 0x1) {
		inst.name = Instruction::REMU;
		inst.type = InstructionType::R;
		inst.rd = getRd(value);
		inst.rs1 = getRs1(value);
		inst.rs2 = getRs2(value);
	}
	else if (opcode == 0x67 && funct3 == 0x0) {
		inst.name = Instruction::JALR;
		inst.type = InstructionType::JALR;
		inst.rs1 = getRs1(value);
		inst.rd = getRd(value);
		inst.imm = getImm(value);
	}
	else if (opcode == 0x73 && funct3 == 0x0) {
		uint32_t funct12 = (value >> 20) & 0xFFF;
		if (funct12 == 0x1) {
			inst.name = Instruction::EBREAK;
			inst.type = InstructionType::ENVIRONMENT;
		}
		else if (funct12 == 0x0) {
			inst.name = Instruction::ECALL;
			inst.type = InstructionType::ENVIRONMENT;
		}
	}
	else {
		inst.name = Instruction::UNKNOWN;
		inst.type = InstructionType::UNKNOWN;
	}

	return inst;
}
