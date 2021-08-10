#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>

typedef uint8_t byte;

//data structures for storing the state of the 8080 CPU and memory

typedef struct Flags {
    byte z:1;
    byte s:1;
    byte p:1;
    byte cy:1;
    byte ac:1;
    byte pad:3;
} Flags;

typedef struct CPUState {
    //registers (order a, b, c, d, e, h, l)
    byte reg[7];

    uint16_t sp; //stack pointer
    uint16_t pc; //program counter/instruction pointer
    byte *memory; //RAM
    Flags fl;
    byte int_enable;
    unsigned int mem_size;
} CPUState;

//create a new CPU state with mem_size bytes of RAM
CPUState *newState(unsigned int mem_size);

//clean up and free the CPU state
void destroyState(CPUState *cs);

//fetch and execute one instruction, return the number of cycles
//it took
int stepCPU(CPUState *cs);

//execute one instruction from an interrupt, return the number of
//cycles it took
int interruptCPU(CPUState *cs, byte opcode);

#endif //_CPU_H
