#pragma once

#include <stdio.h>
#include <stdint.h>

#define BANK_COUNT 0xFF
#define BYTES_PER_BANK 0xFFFF
#define STACK_SIZE 0xFFFF
#define PROGRAM_START_BANK 0xF0

typedef struct {
    uint32_t regs[0xFF];
    uint8_t mem[BANK_COUNT+1][BYTES_PER_BANK];
    uint32_t stack[STACK_SIZE];
    uint32_t pc;
    uint32_t pc_bank;
    uint32_t sp;
} mound_cpu;

mound_cpu* create_mound_cpu();
void mound_cpu_run(mound_cpu* cpu);
void mound_cpu_load(mound_cpu* cpu, uint8_t* program, size_t len);