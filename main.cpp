#include "decoder.h"
#include <iostream>
#include <string>
#include <cstdint>

std::string getBinary() {
	std::cout << "Enter a 32 bit binary string: ";
	std::string binaryString{};
	std::getline(std::cin >> std::ws, binaryString);

	return binaryString;
}

int main() {

	std::string binaryString{ getBinary() };
	uint32_t value{ convertBinary(binaryString) };

	DecodedInstruction inst = decodeInstruction(value);

	uint32_t opcode{ getOpCode(value) };
	
	if (inst.type == "I-type") {
		std::cout << inst.name << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm << '\n';
	}
	else if (inst.type == "Special I-type") {
		std::cout << inst.name << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.shamt << '\n';
	}
	else if (inst.type == "R-type") {
		std::cout << inst.name << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.rs2 << '\n';
	}
	else if (inst.type == "S-type") {
		std::cout << inst.name << " x" << inst.rs2 << ", " << inst.imm << "(x" << inst.rs1 << ")" << '\n';
	}
	

	std::cout << "Opcode: " << opcode << '\n';
	std::cout << "rd: " << inst.rd << '\n';
	std::cout << "rs1: " << inst.rs1 << '\n';


	std::cout << std::hex << "Raw Instruction (Hex): " << value << '\n';

	return 0;
}
