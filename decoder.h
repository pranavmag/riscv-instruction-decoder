#pragma once

#include <string>
#include <cstdint>

std::string getBinary();

uint32_t convertBinary(std::string);

uint32_t getOpCode(uint32_t);

uint32_t getRd(uint32_t);

uint32_t getFunct3(uint32_t);

uint32_t getRs1(uint32_t);

int32_t getImm(uint32_t);



