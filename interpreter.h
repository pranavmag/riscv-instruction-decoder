#pragma once

#include "decoder.h"
#include <vector>
#include <array>
#include <string>
#include <cstdint>

struct Memory {
	std::vector<uint8_t> memory;

	Memory() {
		memory.resize(65536, 0);
	}

	uint32_t readByte(uint32_t addr);

	uint32_t readHalfWord(uint32_t addr);

	uint32_t readWord(uint32_t addr);

	void writeByte(uint32_t addr, uint32_t value);

	void writeHalfWord(uint32_t addr, uint32_t value);

	void writeWord(uint32_t addr, uint32_t value);

	void loadProgram(const std::string& fileName);
};

struct Core {
	std::array<uint32_t, 32> registers{};
	uint32_t pc{ };
	uint32_t core_id{ };

	void writeReg(int regNum, uint32_t value);

	uint32_t readReg(int regNum);


	std::array<float, 32> freg{};

	void writeFReg(int regNum, float value);

	float readFReg(int regNum);

	void execute(const DecodedInstruction& inst, Memory& mem);

	bool halted{ false };

	void run(Memory& mem);
};
