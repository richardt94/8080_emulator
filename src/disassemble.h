#ifndef _DISASSEMBLER_H
#include <stdio.h>

typedef unsigned char byte;

int disassembleOp(byte *ibuf, int pc, FILE *out)
{
    byte *code = &ibuf[pc];
    int opbytes = 1;
    fprintf(out, "%04x ", pc);
    switch (*code)
    {
        case 0x00: fprintf(out, "NOP"); break;
        case 0x01: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x02: fprintf(out, "STAX   B"); break;
        case 0x03: fprintf(out, "INX    B"); break;
        case 0x04: fprintf(out, "INR    B"); break;
        case 0x05: fprintf(out, "DCR    B"); break;
        case 0x06: fprintf(out, "MVI    B,#$%02x", code[1]); opbytes = 2; break;
        case 0x07: fprintf(out, "RLC"); break;
        case 0x08: fprintf(out, "NOP"); break;
        case 0x09: fprintf(out, "DAD    B"); break;
        case 0x0A: fprintf(out, "LDAX   B"); break;
        case 0x0B: fprintf(out, "DCX    B"); break;
        case 0x0C: fprintf(out, "INR    C"); break;
        case 0x0D: fprintf(out, "DCR    C"); break;
        case 0x0E: fprintf(out, "MVI    C,#$%02x", code[1]); opbytes = 2; break;
        case 0x0F: fprintf(out, "RRC"); break;
        default: fprintf(out, "instruction not known"); break;
        // case 0x10: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x11: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x12: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x13: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x14: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x15: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x16: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x17: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x18: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x19: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x1A:
        // case 0x1B:
        // case 0x1C:
        // case 0x1D:
        // case 0x1E:
        // case 0x1F:
        // case 0x20: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
        // case 0x21: fprintf(out, "LXI    B,#$%02x%02x", code[2], code[1]); break;
    }

    fprintf(out, "\n");

    return opbytes;
}
#endif