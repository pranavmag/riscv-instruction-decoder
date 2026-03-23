#include <iostream>
#include <string>
#include <cstdint>

uint32_t convertBinary(std::string binaryString) {
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