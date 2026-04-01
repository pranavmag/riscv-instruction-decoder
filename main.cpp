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
	std::cout << "2 - Interpreter (manual-input mode)\n";
	std::cout << "3 - Interpreter (load .bin file)\n";

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

		int addr{};

		while (true) {

			std::cout << "Enter binary instructions (type 'done' if finished): ";
			std::string binaryString{ getBinary() };

			if (binaryString == "done") {
				break;
			}

			uint32_t value = convertBinary(binaryString);

			mem.writeWord(addr, value);

			addr += 4;
		}

		mem.writeWord(addr, 0x100073);


		core.run(mem);

		for (int i{}; i < 32; i++) {
			std::cout << "x" << i <<  " = " << core.readReg(i) << '\n';
		}
	}

	if (choice == 3) {
		Memory mem;
		Core core;

		std::cout << "Enter the name of the .bin file (test.bin): ";
		std::string fileName{};
		std::getline(std::cin >> std::ws, fileName);

		mem.loadProgram(fileName);

		core.run(mem);

		for (int i{}; i < 32; i++) {
			std::cout << "x" << i << " = " << core.readReg(i) << '\n';
		}
	}

	return 0;
}
