#pragma once

#include "decoder.h"
#include <vector>
#include <array>
#include <iostream>
#include <cstdint>

struct Core {
	std::array<uint32_t, 32> registers;
	uint32_t pc;
	uint32_t core_id;

	void writeReg(int regNum, uint32_t value);

	uint32_t readReg(int regNum);

	void execute(const DecodedInstruction& inst);
};
