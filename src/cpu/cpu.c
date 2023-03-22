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

void f0_dump(mound_cpu* cpu, size_t bytes) {
    printf("------- MOUNDVM F0 DUMP(First %ld bytes) -------", bytes);
    for(int i = 0; i < bytes; i++) {
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
    f0_dump(cpu, 255);
    exit(-1);
}

mound_cpu* create_mound_cpu() {
    mound_cpu* cpu = malloc(sizeof * cpu);
    cpu->pc = 0;
    cpu->pc_bank = 0xf0;
    return cpu;
}

void mound_cpu_run(mound_cpu* cpu) {
    while(1) {
        switch(cpu->mem[cpu->pc_bank][cpu->pc]) {
            case 0xFF: break; // nop: do nothing
            case 0x01: { // mov8
                printf("mov8\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[bb] = aa;
            } break;
            case 0x02: { // mov16
                printf("mov16\n");
                uint16_t aaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("aaaa: %04x\n", aaaa);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bb: %02x\n", bb);
                cpu->regs[bb] = aaaa;
            } break;
            case 0x03: { // mov32
                printf("mov32\n");
                uint32_t aaaaaaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 24) | (cpu->mem[cpu->pc_bank][++cpu->pc] << 16) | (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("aaaaaaaa: %08x\n", aaaaaaaa);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bb: %02x\n", bb);
                cpu->regs[bb] = aaaaaaaa;
            } break; 
            case 0x04: { // ld8
                printf("ld8\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bb: %02x\n", bb);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cccc: %04x\n", cccc);
                cpu->mem[bb][cccc] = aa;
            } break;
            case 0x05: { // ld16
                printf("ld16\n");
                uint16_t aaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("aaaa: %04x\n", aaaa);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bb: %02x\n", bb);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cccc: %04x\n", cccc);
                cpu->mem[bb][cccc] = aaaa;
            } break;
            case 0x06: { // ld32
                printf("ld32\n");
                uint32_t aaaaaaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 24) | (cpu->mem[cpu->pc_bank][++cpu->pc] << 16) | (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("aaaaaaaa: %08x\n", aaaaaaaa);
                uint8_t bb = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bb: %02x\n", bb);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cccc: %04x\n", cccc);
                cpu->mem[bb][cccc] = aaaaaaaa;
            } break;
            case 0x07: { // add
                printf("add\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[aa] += cpu->regs[bb];
            } break;
            case 0x08: { // sub
                printf("sub\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[aa] -= cpu->regs[bb];
            } break;
            case 0x09: { // mul
                printf("mul\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[aa] *= cpu->regs[bb];
            } break;
            case 0x0a: { // jmp
                printf("jmp\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint16_t bbbb = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bbbb: %04x\n", bbbb);
                cpu->pc_bank = aa;
                cpu->pc = bbbb;
            } break;
            case 0x0b: { // jmpe
                printf("jmpe\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                uint8_t cc = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cc: %02x\n", cc);
                uint16_t dddd = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("dddd: %04x\n", dddd);
                if(cpu->regs[aa] == cpu->regs[bb]) cpu->pc_bank = cc, cpu->pc = dddd;
            } break;
            case 0x0c: { //jmpne
                printf("jmpne\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                uint8_t cc = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cc: %02x\n", cc);
                uint16_t dddd = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("dddd: %04x\n", dddd);
                if(cpu->regs[aa] != cpu->regs[bb]) cpu->pc_bank = cc, cpu->pc = dddd;
            } break;
            case 0x0d: { // jmpl
                printf("jmpl\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                uint8_t cc = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cc: %02x\n", cc);
                uint16_t dddd = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("dddd: %04x\n", dddd);
                if(cpu->regs[aa] < cpu->regs[bb]) cpu->pc_bank = cc, cpu->pc = dddd;
            } break;
            case 0x0e: { //jmpg
                printf("jmpe\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                uint8_t cc = (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cc: %02x\n", cc);
                uint16_t dddd = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("dddd: %04x\n", dddd);
                if(cpu->regs[aa] > cpu->regs[bb]) cpu->pc_bank = cc, cpu->pc = dddd;
            } break;
            case 0x0f: { //push 
                printf("push\n");
                uint32_t aaaaaaaa = (cpu->mem[cpu->pc_bank][++cpu->pc] << 24) | (cpu->mem[cpu->pc_bank][++cpu->pc] << 16) | (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("aaaaaaaa: %08x\n", aaaaaaaa);
                cpu->stack[cpu->sp++] = aaaaaaaa;
            } break;
            case 0x10: { //pop
                printf("pop\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                cpu->regs[aa] = cpu->stack[--cpu->sp];
            } break;
            case 0x11: { // ldr 
                printf("ldr\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                uint16_t cccc = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("cccc: %04x\n", cccc);
                cpu->mem[bb][cccc] = cpu->regs[aa];
            } break;
            case 0x12: { // call
                printf("call\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint16_t bbbb = (cpu->mem[cpu->pc_bank][++cpu->pc] << 8) | (cpu->mem[cpu->pc_bank][++cpu->pc]);
                printf("bbbb: %04x\n", bbbb);
                cpu->regs[0xfe] = (aa << 16) | bbbb;
                cpu->pc_bank = aa;
                cpu->pc = bbbb;
            } break;
            case 0x13: { //ret
                printf("ret\n");
                cpu->pc_bank = (cpu->regs[0xfe] & 0xff0000)>> 16;
                cpu->pc = cpu->regs[0xfe] & 0xffff;
            } break;
            case 0x14: { // movr
                printf("movr\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[bb] = cpu->regs[aa];
            } break;
            case 0x15: { // shl
                printf("shl\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                cpu->regs[aa] <<= cpu->regs[aa];
            } break;
            case 0x16: { // shr
                printf("shr\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                cpu->regs[aa] >>= cpu->regs[aa];
            } break;
            case 0x17: { // and
                printf("and\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[aa] &= cpu->regs[bb];
            } break;
            case 0x18: { // or
                printf("or\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[aa] |= cpu->regs[bb];
            } break;
            case 0x19: { // xor
                printf("xor\n");
                uint8_t aa = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("aa: %02x\n", aa);
                uint8_t bb = cpu->mem[cpu->pc_bank][++cpu->pc];
                printf("bb: %02x\n", bb);
                cpu->regs[aa] ^= cpu->regs[bb];
            } break;
            case 0x00: goto finish;
            default: bad_thing_happened(cpu, MOUND_ERR_INVALID_INSTRUCTION);
        }
        cpu->pc++;
        if(cpu->pc % BYTES_PER_BANK == 0) cpu->pc_bank++;
    }
    finish:
    printf("Finished execution, dumping moundvm info...\n");
    dump_registers(cpu);
    f0_dump(cpu, 200);
}   

void mound_cpu_load(mound_cpu* cpu, uint8_t* program, size_t len) {
    int bank_offs = 0;
    for(int i = 0; i < len; i++) {
        if(i % BYTES_PER_BANK == 0 && i != 0) bank_offs++, printf("Swapping bank...\n");
        cpu->mem[cpu->pc_bank + bank_offs][i] = program[i];
    }
}