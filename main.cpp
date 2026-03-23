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

	uint32_t opcode{ getOpCode(value) };
	uint32_t rd{ getRd(value) };
	uint32_t funct3{ getFunct3(value) };
	uint32_t rs1{ getRs1(value) };
	int32_t imm{ getImm(value) };
	
	std::cout << std::hex << value << '\n';

	std::cout << std::dec;
	std::cout << "Opcode: " << opcode << '\n';
	std::cout << "rd: " << rd << '\n';
	std::cout << "Funct3: " << funct3 << '\n';
	std::cout << "rs1: " << rs1 << '\n';
	std::cout << "immediate: " << imm << '\n';

	return 0;
}
