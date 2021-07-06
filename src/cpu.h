#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

    byte sp; //stack pointer
    byte pc; //program counter/instruction pointer
    byte *memory; //RAM
    Flags fl;
    byte int_enable;
} CPUState;

CPUState* newState() {
    CPUState* cs = malloc(sizeof(CPUState));
    cs->fl.z = 0;
    cs->fl.s = 0;
    cs->fl.p = 0;
    cs->fl.cy = 0;
    cs->fl.ac = 0;
    cs->fl.pad = 0;

    for (int r = 0; r < 7; r++) {
        cs->reg[r] = 0;
    }
    cs->sp = 0;
    cs->pc = 0;
    cs->int_enable = 0;

    cs->memory = (byte *) calloc(8192, sizeof(byte));
    return cs;
}

void destroyState(CPUState *cs) {
    free(cs->memory);
    free(cs);
}

//handling for unimplemented ops
void unknownOp (CPUState *state) {
    fprintf(stderr, "Error: unimplemented instruction. Exiting...\n");
    exit(1);
}

// helper functions
byte arithmeticOperand(uint16_t atype, CPUState *state) {
    byte r2;
    if (atype < 6) { //b, c, d, e, h, l addto a
        r2 = state->reg[atype + 1];
    } else if (atype == 7) {
        r2 = state->reg[0];
    } else {
        //hl is a memory address
        uint16_t adr = ((uint16_t) state->reg[5] << 8) | (state->reg[6]);
        r2 = state->memory[adr];
    }
    return r2;
}

void set_result(uint16_t res, CPUState *state) {
    state->fl.z = (res & 0xff) == 0; //zero
    state->fl.s = (res & 0x80) != 0; //sign
    state->fl.cy = res > 0xff; //carry (result > 255)
    state->fl.p = 0;//parity(answer & 0xff);
    state->reg[0] = res & 0xff; //res is always sent to A (the accumulator)
}

//TODO bool parity(byte b);

//instruction implementations
//returns true once CPU halts
int executeOp(CPUState *state) {
    byte *opcode = &state->memory[state->pc];
    switch (*opcode) {
        //nops
        case 0x00: case 0x08: case 0x10: case 0x18:
        case 0x20: case 0x28: case 0x30: case 0x38:
        case 0xcb: case 0xd9: case 0xdd: case 0xed:
        case 0xfd: break;
        //ADD, ADI
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85: case 0x86: case 0x87:
        case 0xc6:
        {
            byte r2;
            //immediate
            if (*opcode == 0xc6) {
                r2 = opcode[1]; state->pc++;
            } else {
                //registers are just an array of bytes
                r2 = arithmeticOperand(*opcode - 0x80, state);
            }
            //avoid overflow with 16-bit precision
            uint16_t res = (uint16_t) state->reg[0] + (uint16_t) r2;
            //set aux carry
            state->fl.ac = (state->reg[0] & 0x0f) + (r2 & 0x0f) > 0x0f;
            set_result(res, state);
            break;
        }
        //SUB, SUBI
        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97:
        case 0xd6:
        {
            byte r2;
            if (*opcode == 0xd6) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0x90, state);
            }
            //explicit two's complement to ensure flags are set correctly
            uint16_t sub_2c = (uint16_t) (~r2) + 1;
            uint16_t res = (uint16_t) state->reg[0] + sub_2c;
            state->fl.ac = (state->reg[0] & 0x0f) + (sub_2c & 0x0f) > 0x0f;
            set_result(res, state);
            //carry flag works opposite to addition on 8080 (but not aux carry)
            state->fl.cy = !state->fl.cy;
            break;
        }
        //HLT
        case 0x76: return 1; break;
        default: unknownOp(state); break;
    }
    state->pc++;
    return 0;
}

#endif //_CPU_H