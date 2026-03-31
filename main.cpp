#include "decoder.h"
#include "interpreter.h"
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
	std::cout << "Select mode:\n";
	std::cout << "1 - Disassembler\n";
	std::cout << "2 - Interpreter\n";

	int choice{};
	std::cin >> choice;

	if (choice == 1) {

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
	}

	if (choice == 2) {
		Memory mem;
		Core core;

		mem.writeWord(0, convertBinary("00000000010100000000000010010011"));
		mem.writeWord(4, convertBinary("00000000101000000000000100010011"));
		mem.writeWord(8, convertBinary("00000000001000001000000110110011"));
		mem.writeWord(12, convertBinary("00000000000100000000000001110011"));


		core.run(mem);

		std::cout << "x1 = " << core.readReg(1) << '\n';
		std::cout << "x2 = " << core.readReg(2) << '\n';
		std::cout << "x3 = " << core.readReg(3) << '\n';
		std::cout << "x4 = " << core.readReg(4) << '\n';
	}

	return 0;
}
