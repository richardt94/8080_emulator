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

CPUState *newState() {
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

uint8_t parity(byte b) {
    // even bits set - parity 1,
    // odd bits set - parity 0
    int set = 0;
    while (b > 0) {
        set += b % 2;
        b /= 2;
    }
    return !(set % 2);
}

void set_flags(uint16_t res, CPUState *state) {
    state->fl.z = (res & 0xff) == 0; //zero
    state->fl.s = (res & 0x80) != 0; //sign
    state->fl.cy = res > 0xff; //carry (result > 255)
    state->fl.p = parity(res & 0xff);
}

void set_result(uint16_t res, CPUState *state) {
    //set accumulator value and flags
    set_flags(res, state);
    state->reg[0] = res & 0xff; //res is always sent to A (the accumulator)
}

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
        //INR
        case 0x04: case 0x0c: case 0x14: case 0x1c:
        case 0x24: case 0x2c: case 0x34: case 0x3c:
        {
            int atype = (*opcode - 0x04) / 8;
            byte res;
            int ri = -1;
            uint16_t mem_adr = 0;
            if (atype < 6) {
                ri = atype + 1;
                res = state->reg[ri];
            } else if (atype == 7) {
                ri = 0;
                res = state->reg[ri];
            } else {
                mem_adr = state->reg[5] << 8 | state->reg[6];
                res = state->memory[mem_adr];
            }
            state->fl.ac = (res & 0x0f) == 0x0f;
            res++;
            //INR does not affect carry. this is implicit
            //as res (byte) will overflow when there is a carry.
            set_flags(res, state);
            if (ri < 0) {
                state->memory[mem_adr] = res;
            } else {
                state->reg[ri] = res;
            }
            break;
        }
        //DCR
        case 0x05: case 0x0d: case 0x15: case 0x1d:
        case 0x25: case 0x2d: case 0x35: case 0x3d:
        {
            int atype = (*opcode - 0x05) / 8;
            byte res;
            int ri = -1;
            uint16_t mem_adr = 0;
            if (atype < 6) {
                ri = atype + 1;
                res = state->reg[ri];
            } else if (atype == 7) {
                ri = 0;
                res = state->reg[ri];
            } else {
                mem_adr = state->reg[5] << 8 | state->reg[6];
                res = state->memory[mem_adr];
            }
            state->fl.ac = (res & 0x0f) != 0x00;
            res--;
            set_flags(res, state);
            if (ri < 0) {
                state->memory[mem_adr] = res;
            } else {
                state->reg[ri] = res;
            }
            break;
        }
        //DAA
        case 0x27:
        {
            uint16_t acc = state->reg[0];
            if (acc & 0x0f > 0x9 || state->fl.ac) {
                if (acc > 0x09) state->fl.ac = 1;
                else state->fl.ac = 0;
                acc += 0x06;
            }
            if (acc >= 0xa0) acc += 0x60;
            set_result(acc, state);
            break;
        }
        //CMA
        case 0x2f: state->reg[0] = ~state->reg[0]; break;
        //MOV
        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b:
        case 0x4c: case 0x4d: case 0x4e: case 0x4f:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55: case 0x56: case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b:
        case 0x5c: case 0x5d: case 0x5e: case 0x5f:
        case 0x60: case 0x61: case 0x62: case 0x63:
        case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b:
        case 0x6c: case 0x6d: case 0x6e: case 0x6f:
        case 0x70: case 0x71: case 0x72: case 0x73:
        case 0x74: case 0x75: case 0x77:
        case 0x78: case 0x79: case 0x7a: case 0x7b:
        case 0x7c: case 0x7d: case 0x7e: case 0x7f:
        {
            int srctype = *opcode & 0x07;
            byte data = arithmeticOperand(srctype, state);
            int dsttype = (*opcode & 0x38) / 8;
            if (dsttype < 6) {
                state->reg[dsttype + 1] = data;
            } else if (dsttype == 7) {
                state->reg[0] = data;
            } else {
                int mem_adr = state->reg[5] << 8 | state->reg[6];
                state->memory[mem_adr] = data;
            }
            break;
        }
        //STAX
        case 0x02: case 0x12:
        {
            int base_reg = 1 + 2 * (*opcode == 0x12);
            int mem_adr = state->reg[base_reg] << 8 | state->reg[base_reg + 1];
            state->memory[mem_adr] = state->reg[0];
            break;
        }
        //LDAX
        case 0x0A: case 0x1A:
        {
            int base_reg = 1 + 2 * (*opcode == 0x1A);
            int mem_adr = state->reg[base_reg] << 8 | state->reg[base_reg + 1];
            state->reg[0] = state->memory[mem_adr];
            break;
        }
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
        //ADC, ACI
        case 0x88: case 0x89: case 0x8a: case 0x8b:
        case 0x8c: case 0x8d: case 0x8e: case 0x8f:
        case 0xce:
        {
            byte r2;
            //immediate
            if (*opcode == 0xce) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0x88, state);
            }
            uint16_t res = (uint16_t) state->reg[0] + (uint16_t) r2 + state->fl.cy;
            state->fl.ac = (state->reg[0] & 0x0f) + (r2 & 0x0f) + state->fl.cy > 0x0f;
            set_result(res, state);
            break;
        }
        //SUB, SUI
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
            //negative of an explicitly unsigned type is automatically two's complement
            //(not implementation dependent)
            r2 = -(unsigned int) r2;
            uint16_t res = (uint16_t) state->reg[0] + (uint16_t) r2;
            state->fl.ac = (state->reg[0] & 0x0f) + (r2 & 0x0f) > 0x0f;
            set_result(res, state);
            //carry flag works opposite to addition on 8080 (but not aux carry)
            state->fl.cy = !state->fl.cy;
            break;
        }
        //SBB, SBI
        case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d: case 0x9e: case 0x9f:
        case 0xde:
        {
            //from the 8080 programmers' manual:
            //the carry is internally added to the
            //second operand and subtraction then performed with normal
            //two's complement rules.
            byte r2;
            if (*opcode == 0xde) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0x98, state);
            }
            r2 += state->fl.cy;
            r2 = -(unsigned int) r2;
            uint16_t res = (uint16_t) state->reg[0] + (uint16_t) r2;
            state->fl.ac = (state->reg[0] & 0x0f) + (r2 & 0x0f) > 0x0f;
            set_result(res, state);
            state->fl.cy = !state->fl.cy;
            break;
        }
        //ANA, ANI
        case 0xa0: case 0xa1: case 0xa2: case 0xa3:
        case 0xa4: case 0xa5: case 0xa6: case 0xa7:
        case 0xe6:
        {
            byte r2;
            if (*opcode == 0xe6) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0xa0, state);
            }
            uint16_t res = (uint16_t) state->reg[0] & (uint16_t) r2;
            set_result(res, state);
            break;
        }
        //XRA, XRI
        case 0xa8: case 0xa9: case 0xaa: case 0xab:
        case 0xac: case 0xad: case 0xae: case 0xaf:
        case 0xee:
        {
            byte r2;
            if (*opcode == 0xee) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0xa8, state);
            }
            uint16_t res = (uint16_t) state->reg[0] ^ (uint16_t) r2;
            set_result(res, state);
            break;
        }
        //ORA, ORI
        case 0xb0: case 0xb1: case 0xb2: case 0xb3:
        case 0xb4: case 0xb5: case 0xb6: case 0xb7:
        case 0xf6:
        {
            byte r2;
            if (*opcode == 0xf6) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0xb0, state);
            }
            uint16_t res = (uint16_t) state->reg[0] | (uint16_t) r2;
            set_result(res, state);
            break;
        }
        //CMP, CPI
        case 0xb8: case 0xb9: case 0xba: case 0xbb:
        case 0xbc: case 0xbd: case 0xbe: case 0xbf:
        case 0xfe:
        {
            //CMP does not affect the accumulator.
            //arg is subtracted from accumulator internally to
            //set flags.
            byte r2;
            if (*opcode == 0xf6) {
                r2 = opcode[1]; state->pc++;
            } else {
                r2 = arithmeticOperand(*opcode - 0xb8, state);
            }
            r2 = -(unsigned int) r2;
            uint16_t res = (uint16_t) state->reg[0] + (uint16_t) r2;
            state->fl.ac = (state->reg[0] & 0x0f) + (r2 & 0x0f) > 0x0f;
            set_flags(res, state);
            state->fl.cy = !state->fl.cy;
            break;
        }
        //RLC
        case 0x07:
        {
            state->fl.cy = state->reg[0] >= 0x80;
            state->reg[0] <<= 1;
            state->reg[0] += state->fl.cy;
            break;
        }
        //RRC
        case 0x0f:
        {
            state->fl.cy = state->reg[0] & 0x01;
            state->reg[0] >>= 1;
            state->reg[0] += state->fl.cy * 0x80;
            break;
        }
        //RAL
        case 0x17:
        {
            int tmp = state->fl.cy;
            state->fl.cy = state->reg[0] >= 0x80;
            state->reg[0] <<= 1;
            state->reg[0] += tmp;
            break;
        }
        //RAR
        case 0x1f:
        {
            int tmp = state->fl.cy;
            state->fl.cy = state->reg[0] & 0x01;
            state->reg[0] >>= 1;
            state->reg[0] += tmp * 0x80;
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