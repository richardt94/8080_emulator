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
    //registers (order a, b, c, d, e, h, l)
    byte[7] reg;

    byte sp; //stack pointer
    byte pc; //program counter/instruction pointer
    byte *memory; //RAM
    struct Flags fl;
    byte int_enable;
} CPUState;

//handling for unimplemented ops
void unknownOp (CPUState *state) {
    fprintf(stderr, "Error: unimplemented instruction. Exiting...\n");
    exit(1);
}

// helper functions TODO
// byte arithmeticOperand(uint16_t atype, CPUState *state) {

// }

//instruction implementations
void executeOp(CPUState *state) {
    byte *opcode = state->memory[state->pc];
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
                r2 = opcode[1]; pc++;
            } else {
                //registers are just an array of bytes
                uint16_t atype = *opcode - 0x80;
                if (atype < 6) { //b, c, d, e, h, l addto a
                    r2 = state->reg[atype + 1];
                } else if atype == 7 {
                    r2 = state->reg[0];
                } else {
                    //hl is a memory address
                    uint16_t adr = ((uint16_t) state->reg[5] << 8) | (state->reg[6]);
                    r2 = state->memory[adr];
                }
            }
            //avoid overflow with 16-bit precision
            uint16_t res = (uint16_t) state->reg[0] + (uint16_t) r2;

            state->fl.z = (res & 0xff) == 0; //zero
            state->fl.s = (res & 0x80) != 0; //sign
            state->fl.cy = res > 0xff; //carry (result > 255)
            state->fl.p = parity(answer & 0xff);
            state->reg[0] = answer & 0xff; //answer is always sent to A (the accumulator)
        }
        break;
        
        //HLT
        case 0x76: exit(0); break;
        default: unknownOp(state); break;
    }
    state->pc++;
}

#endif _CPU_H