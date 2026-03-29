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
	while (true) {

		std::string binaryString{ getBinary() };

		if (binaryString == "quit") break;

		uint32_t value{ convertBinary(binaryString) };

		DecodedInstruction inst = decodeInstruction(value);

		if (inst.type == InstructionType::I) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm << '\n';
		}
		else if (inst.type == InstructionType::SHIFT) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.shamt << '\n';
		}
		else if (inst.type == InstructionType::R) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.rs2 << '\n';
		}
		else if (inst.type == InstructionType::S) {
			std::cout << instructionToString(inst.name) << " x" << inst.rs2 << ", " << inst.imm << "(x" << inst.rs1 << ")" << '\n';
		}
		else if (inst.type == InstructionType::B) {
			std::cout << instructionToString(inst.name) << " x" << inst.rs1 << ", x" << inst.rs2 << ", " << inst.imm << '\n';
		}
		else if (inst.type == InstructionType::J) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", " << inst.imm << '\n';
		}
		else if (inst.type == InstructionType::U) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", " << inst.imm << '\n';
		}
		else if (inst.type == InstructionType::LOAD) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", " << inst.imm << "(x" << inst.rs1 << ")" << '\n';
		}
		else if (inst.type == InstructionType::JALR) {
			std::cout << instructionToString(inst.name) << " x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm << '\n';
		}
		else if (inst.type == InstructionType::ENVIRONMENT) {
			std::cout << instructionToString(inst.name) << '\n';
		}
		else {
			std::cout << instructionToString(inst.name) << '\n';
		}
	
	}

	return 0;
}
