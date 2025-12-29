#include "vm.h"
#include "debug.h"
#include <iostream>
#include "compiler.h"


InterpretResult VM::run() {
    while (ip < chunk->code.size()) {

        #ifdef DEBUG_TRACE_EXECUTION
            std::cout << "        ";
            for (auto value : stack) {
                std::cout << "[ " << value << " ]";
            }
            std::cout << "\n";
            disassembleInstruction(*chunk, ip);
        #endif

        uint8_t instruction = readByte();

        switch (static_cast<OpCode>(instruction)) {
            case OpCode::CONSTANT: {
                const Value& constant = readConstant();
                push(constant);
                break;
            }

            case OpCode::ADD:
                binaryOp(std::plus<>());
                break;

            case OpCode::SUBTRACT:
                binaryOp(std::minus<>());
                break;

            case OpCode::MULTIPLY:
                binaryOp(std::multiplies<>());
                break;

            case OpCode::DIVIDE:
                binaryOp(std::divides<>());
                break;

            case OpCode::NEGATE: 
                push(-pop());
                break;
            
            case OpCode::RETURN: 
                printValue(pop());
                std::cout << "\n";
                return InterpretResult::OK;
            
            default:
                return InterpretResult::RUNTIME_ERROR;
        }
    }
    return InterpretResult::OK;
}



InterpretResult VM::interpret(const std::string& source) {
    Chunk chunk;

    if (!compile(source, chunk)) {
        return InterpretResult::COMPILE_ERROR;
    }

    this->chunk = &chunk;
    ip = 0;

    InterpretResult result = run();

    return result;
}


uint8_t VM::readByte() {
    return chunk->code[ip++];
}

const Value& VM::readConstant() {
    uint8_t index = readByte();
    return chunk->constants.values[index];
}

void VM::push(const Value& value) {
    stack.push_back(value);
}

Value VM::pop() {
    Value value = stack.back();
    stack.pop_back();
    return value;
}

void VM::resetStack() {
    stack.clear();
}