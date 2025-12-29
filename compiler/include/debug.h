#pragma once

#include "chunk.h"
#include <string>

void disassembleChunk(const Chunk& chunk, const std::string& name);
static int constantInstruction(const std::string& name, const Chunk& chunk, int offset);
static int simpleInstruction(const std::string& name, int offset);
int disassembleInstruction(const Chunk& chunk, int offset);
