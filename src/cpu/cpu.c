#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    MOUND_ERR_INVALID_INSTRUCTION,
} MoundException;

void dump_registers(mound_cpu* cpu) {
    printf("------- MOUNDVM REGISTER DUMP -------");
    for(int i = 0; i < 255 ; i++) {
        if(i % 8 == 0) printf("\n");
        printf("r%02x: %08x\t", i, cpu->regs[i]);
    }
    printf("\n\n");
}

void ram_dump(mound_cpu* cpu) {
    printf("------- MOUNDVM RAM DUMP ------");
    for(int b = 0; b < BANK_COUNT; b++) {
        for(int i = 0; i < BYTES_PER_BANK; i++) {
            if(i % 32 == 0) printf("\n%02x %04x: ", b, i);
            printf("%02x ", cpu->mem[b][i]);
        }
    }
    printf("\n\n");
}

void f0_dump(mound_cpu* cpu) {
    printf("------- MOUNDVM F0 DUMP -------");
    for(int i = 0; i < BYTES_PER_BANK; i++) {
        if(i % 32 == 0) printf("\n%02x %04x: ", 0xf0, i);
        printf("%02x ", cpu->mem[0xf0][i]);
    }
    printf("\n\n");
}

void bad_thing_happened(mound_cpu* cpu, MoundException err) {
    fprintf(stderr, "Fatal exception occured at address 0x%08x in bank %02x\nCurrent instruction: 0x%02x\n", cpu->pc, cpu->pc_bank, cpu->mem[cpu->pc_bank][cpu->pc]);
    switch(err) {
        case MOUND_ERR_INVALID_INSTRUCTION: fprintf(stderr, "Invalid instruction!\n"); break;
    }
    dump_registers(cpu);
    f0_dump(cpu);
    exit(-1);
}

mound_cpu* create_mound_cpu() {
    mound_cpu* cpu = malloc(sizeof * cpu);
    cpu->pc = 0;
    cpu->pc_bank = 0xf0;
    return cpu;
}

void mound_cpu_run(mound_cpu* cpu) {
    while(cpu->mem[cpu->pc] != 0x00) {
        switch(cpu->mem[cpu->pc_bank][cpu->pc]) {
            case 0xFF: break; // nop: do nothing
            case 0x01: { // mov8
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                cpu->regs[bb] = aa;
            } break;
            case 0x02: { // mov16
                uint16_t aaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                cpu->regs[bb] = aaaa;
            } break;
            case 0x03: { // mov32
                uint32_t aaaaaaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 24) & (cpu->mem[cpu->pc_bank][++cpu->pc] << 16) & (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                cpu->regs[bb] = aaaaaaaa;
            } break; 
            case 0x04: { // ld8
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                cpu->mem[bb][cccc] = aa;
            } break;
            case 0x05: { // ld16
                uint16_t aaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                cpu->mem[bb][cccc] = aaaa;
            } break;
            case 0x06: { // ld32
                uint32_t aaaaaaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 24) & (cpu->mem[cpu->pc_bank][++cpu->pc] << 16) & (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) & (cpu->mem[cpu->pc_bank][++cpu->pc]);
                cpu->mem[bb][cccc] = aaaaaaaa;
            } break;
            case 0x07: { // add
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                cpu->regs[aa] = cpu->regs[aa] + cpu->regs[bb];
            } break;
            default: bad_thing_happened(cpu, MOUND_ERR_INVALID_INSTRUCTION);
        }
        cpu->pc++;
    }
    printf("Finished execution, dumping registers...\n");
    dump_registers(cpu);
}

void mound_cpu_load(mound_cpu* cpu, uint8_t* program, size_t len) {
    int bank_offs = 0;
    for(int i = 0; i < len; i++) {
        if(i % BYTES_PER_BANK == 0 && i != 0) bank_offs++;
        cpu->mem[cpu->pc_bank + bank_offs][i] = program[i];
    }
}