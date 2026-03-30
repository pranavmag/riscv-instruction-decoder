#pragma once

#include "decoder.h"
#include <vector>
#include <array>
#include <iostream>
#include <cstdint>

struct Memory {
	std::array<uint8_t, 65536> memory;

	uint32_t readByte(uint32_t addr);

	uint32_t readHalfWord(uint32_t addr);

	uint32_t readWord(uint32_t addr);

	void writeByte(uint32_t addr, uint32_t value);

	void writeHalfWord(uint32_t addr, uint32_t value);

	void writeWord(uint32_t addr, uint32_t value);
};

struct Core {
	std::array<uint32_t, 32> registers;
	uint32_t pc;
	uint32_t core_id;

	void writeReg(int regNum, uint32_t value);

	uint32_t readReg(int regNum);

	void execute(const DecodedInstruction& inst, Memory& mem);
};
