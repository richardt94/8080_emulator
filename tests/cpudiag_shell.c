#include "cpu.h"
#include <stdio.h>

/*
    Emulator shell specifically to run the CPU diagnostics binary file.
    Catches calls to CP/M print routine and emulates them separately to the
    CPU emulation.
    Use this as an integration test for the CPU emulation. Requires assembled
    CPUDIAG program for the 8080 from: www.emulator101.com/files/cpudiag.bin
*/

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <cpudiag binary file>\n", argv[0]);
        return 1;
    }

    unsigned int mem_size = 65536;

    CPUState *cs = newState(mem_size);
    FILE *bin_file = fopen(argv[1], "rb");

    //size of file in bytes
    fseek(bin_file, 0L, SEEK_END);
    int fsize = ftell(bin_file);
    fseek(bin_file, 0L, SEEK_SET);

    if (fsize > mem_size - 0x100) {
        printf("file too big!\n");
        return 1;
    }
    fread(cs->memory + 0x0100, sizeof(byte), fsize, bin_file);
    fclose(bin_file);

    cs->memory[0] = 0x31;//lxi sp (for 8080PRE)
    cs->memory[1] = 0x06;
    cs->memory[2] = 0x00;
    cs->memory[3] = 0xc3;//JMP to start of CP/M program
    cs->memory[4] = 0x00;
    cs->memory[5] = 0x01;
    cs->memory[6] = 0xff;//this is the "stack pointer" address for 8080EX1
    cs->memory[7] = 0xff;

    while (1) {
        //catch print calls (code copied and modified from emulator101)
        if (cs->memory[cs->pc] == 0xcd && cs->memory[cs->pc+1] == 0x05 && cs->memory[cs->pc+2] == 0x00) {
            if (cs->reg[2] == 9) {
                uint16_t offset = (cs->reg[3]<<8) | (cs->reg[4]);    
                char *str = &cs->memory[offset];
                while (*str != '$')
                    printf("%c", *str++);
                // printf("\n");
            } else if (cs->reg[2] == 2) {
                //accumulator is a single character (or maybe E register)
                //(from cp/m programmers' manual here http://www.cpm.z80.de/manuals/cpm22-m.pdf)
                printf("%c", cs->reg[4]);
            }
            //"return"
            cs->pc += 3;
        }
        //catch JMP $0000 (restart called at end of program)
        else if (cs->memory[cs->pc] == 0xc3 && (cs->memory[cs->pc+2] << 8 | cs->memory[cs->pc+1]) == 0) break;
        else if (stepCPU(cs) == 0) break;
    }
    printf("\n");

    destroyState(cs);

    return 0;
}