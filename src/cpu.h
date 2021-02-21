#ifndef _CPU_H

typedef unsigned char byte;

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
    //registers
    byte a;
    byte b;
    byte c;
    byte d:
    byte e;
    byte h;
    byte l;

    byte sp; //stack pointer
    byte pc; //program counter/instruction pointer
    byte *memory; //RAM
    struct Flags fl;
    byte int_enable;
} CPUState;

//instruction implementations
void unknownOp (CPUState *state) {
    fprintf(stderr, "Error: unimplemented instruction. Exiting...\n");
    exit(1);
}

void executeOp(CPUState *state) {
    byte *opcode = state->memory[state->pc];
    switch (*opcode) {
        default: unknownOp(state); break;
    }
    state->pc += 1;
}

#endif _CPU_H