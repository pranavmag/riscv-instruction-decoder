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

struct IF_ID {
	uint32_t pc{};
	uint32_t bits{};
	bool bubble{ true };
};

struct ID_EX {
	DecodedInstruction inst{};
	uint32_t pc{};
	uint32_t rs1Value{};
	uint32_t rs2Value{};

	float rs1FValue{};
	float rs2FValue{};
	float rs3FValue{};

	bool bubble{ true };
};

struct EX_MEM {
	DecodedInstruction inst{};
	uint32_t pc{};
	uint32_t aluResult{};
	float aluFResult{};
	uint32_t rs2Value{};
	bool takeBranch{ false };
	uint32_t branchTarget{};
	bool bubble{ true };
};

struct MEM_WB {
	DecodedInstruction inst{};
	uint32_t pc{};
	uint32_t aluResult{};
	uint32_t memRead{};
	bool bubble{ true };
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

	IF_ID if_id_reg;
	ID_EX id_ex_reg;
	EX_MEM ex_mem_reg;
	MEM_WB mem_wb_reg;

	IF_ID next_if_id_reg;
	ID_EX next_id_ex_reg;
	EX_MEM next_ex_mem_reg;
	MEM_WB next_mem_wb_reg;

	void fetchStage(Memory& mem);

	void decodeStage(Memory& mem);

	void executeStage(Memory& mem);

	void execute(const DecodedInstruction& inst, Memory& mem);

	bool halted{ false };

	void run(Memory& mem);
};