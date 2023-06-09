#include <stdio.h>
#include <stdlib.h>
#include "cpu/cpu.h"

void usage(char* filename) {
    printf("Usage:\n%s <mvm file>\n", filename);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        usage(argv[0]);
    }
    FILE* file = fopen(argv[1], "rb");
    if(file == NULL) {
        printf("Failed to open file '%s' for reading\nExiting...\n", argv[1]);
        exit(-1);
    }
    uint8_t* buf;
    long l;
    fseek(file, 0, SEEK_END);
    l = ftell(file);
    rewind(file);
    buf = (char*)malloc(l * sizeof(uint8_t));
    fread(buf, l, 1, file);
    fclose(file);
    mound_cpu* cpu = create_mound_cpu();
    mound_cpu_load(cpu, buf, l);
    printf("Loaded program into memory... starting execution...\n");
    mound_cpu_run(cpu);
}