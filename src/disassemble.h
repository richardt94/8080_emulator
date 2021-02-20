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
        case 0x10: fprintf(out, "NOP"); break;
        case 0x11: fprintf(out, "LXI    D,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x12: fprintf(out, "STAX   D"); break;
        case 0x13: fprintf(out, "INX    D"); break;
        case 0x14: fprintf(out, "INR    D"); break;
        case 0x15: fprintf(out, "DCR    D"); break;
        case 0x16: fprintf(out, "MVI    D,#$%02x", code[1]); opbytes = 2; break;
        case 0x17: fprintf(out, "RAL"); break;
        case 0x18: fprintf(out, "NOP"); break;
        case 0x19: fprintf(out, "DAD    D"); break;
        case 0x1A: fprintf(out, "LDAX   D"); break;
        case 0x1B: fprintf(out, "DCX    D"); break;
        case 0x1C: fprintf(out, "INR    E"); break;
        case 0x1D: fprintf(out, "DCR    E"); break;
        case 0x1E: fprintf(out, "MVI    E,#$%02x", code[1]); opbytes = 2; break;
        case 0x1F: fprintf(out, "RAR"); break;
        case 0x20: fprintf(out, "NOP"); break;
        case 0x21: fprintf(out, "LXI    H,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x22: fprintf(out, "SHLD   $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x23: fprintf(out, "INX    H"); break;
        case 0x24: fprintf(out, "INR    H"); break;
        case 0x25: fprintf(out, "DCR    H"); break;
        case 0x26: fprintf(out, "MVI    H,#$%02x", code[1]); opbytes = 2; break;
        case 0x27: fprintf(out, "DAA"); break;
        case 0x28: fprintf(out, "NOP"); break;
        case 0x29: fprintf(out, "DAD    H"); break;
        case 0x2A: fprintf(out, "LHLD   $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x2B: fprintf(out, "DCX    H"); break;
        case 0x2C: fprintf(out, "INR    L"); break;
        case 0x2D: fprintf(out, "DCR    L"); break;
        case 0x2E: fprintf(out, "MVI    L,#$%02x", code[1]); opbytes = 2; break;
        case 0x2F: fprintf(out, "CMA"); break;
        case 0x30: fprintf(out, "NOP"); break;
        case 0x31: fprintf(out, "LXI   SP,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x32: fprintf(out, "STA    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x33: fprintf(out, "INX   SP"); break;
        case 0x34: fprintf(out, "INR    M"); break;
        case 0x35: fprintf(out, "DCR    M"); break;
        case 0x36: fprintf(out, "MVI    M,#$%02x", code[1]); opbytes = 2; break;
        case 0x37: fprintf(out, "STC"); break;
        case 0x38: fprintf(out, "NOP"); break;
        case 0x39: fprintf(out, "DAD   SP"); break;
        case 0x3A: fprintf(out, "LDA    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0x3B: fprintf(out, "DCX   SP"); break;
        case 0x3C: fprintf(out, "INR    A"); break;
        case 0x3D: fprintf(out, "DCR    A"); break;
        case 0x3E: fprintf(out, "MVI    A,#$%02x", code[1]); opbytes = 2; break;
        case 0x3F: fprintf(out, "CMC"); break;
        case 0x40: fprintf(out, "MOV    B,B"); break;
        case 0x41: fprintf(out, "MOV    B,C"); break;
        case 0x42: fprintf(out, "MOV    B,D"); break;
        case 0x43: fprintf(out, "MOV    B,E"); break;
        case 0x44: fprintf(out, "MOV    B,H"); break;
        case 0x45: fprintf(out, "MOV    B,L"); break;
        case 0x46: fprintf(out, "MOV    B,M"); break;
        case 0x47: fprintf(out, "MOV    B,A"); break;
        case 0x48: fprintf(out, "MOV    C,B"); break;
        case 0x49: fprintf(out, "MOV    C,C"); break;
        case 0x4a: fprintf(out, "MOV    C,D"); break;
        case 0x4b: fprintf(out, "MOV    C,E"); break;
        case 0x4c: fprintf(out, "MOV    C,H"); break;
        case 0x4d: fprintf(out, "MOV    C,L"); break;
        case 0x4e: fprintf(out, "MOV    C,M"); break;
        case 0x4f: fprintf(out, "MOV    C,A"); break;
        case 0x50: fprintf(out, "MOV    D,B"); break;
        case 0x51: fprintf(out, "MOV    D,C"); break;
        case 0x52: fprintf(out, "MOV    D,D"); break;
        case 0x53: fprintf(out, "MOV    D,E"); break;
        case 0x54: fprintf(out, "MOV    D,H"); break;
        case 0x55: fprintf(out, "MOV    D,L"); break;
        case 0x56: fprintf(out, "MOV    D,M"); break;
        case 0x57: fprintf(out, "MOV    D,A"); break;
        case 0x58: fprintf(out, "MOV    E,B"); break;
        case 0x59: fprintf(out, "MOV    E,C"); break;
        case 0x5a: fprintf(out, "MOV    E,D"); break;
        case 0x5b: fprintf(out, "MOV    E,E"); break;
        case 0x5c: fprintf(out, "MOV    E,H"); break;
        case 0x5d: fprintf(out, "MOV    E,L"); break;
        case 0x5e: fprintf(out, "MOV    E,M"); break;
        case 0x5f: fprintf(out, "MOV    E,A"); break;
        case 0x60: fprintf(out, "MOV    H,B"); break;
        case 0x61: fprintf(out, "MOV    H,C"); break;
        case 0x62: fprintf(out, "MOV    H,D"); break;
        case 0x63: fprintf(out, "MOV    H,E"); break;
        case 0x64: fprintf(out, "MOV    H,H"); break;
        case 0x65: fprintf(out, "MOV    H,L"); break;
        case 0x66: fprintf(out, "MOV    H,M"); break;
        case 0x67: fprintf(out, "MOV    H,A"); break;
        case 0x68: fprintf(out, "MOV    L,B"); break;
        case 0x69: fprintf(out, "MOV    L,C"); break;
        case 0x6a: fprintf(out, "MOV    L,D"); break;
        case 0x6b: fprintf(out, "MOV    L,E"); break;
        case 0x6c: fprintf(out, "MOV    L,H"); break;
        case 0x6d: fprintf(out, "MOV    L,L"); break;
        case 0x6e: fprintf(out, "MOV    L,M"); break;
        case 0x6f: fprintf(out, "MOV    L,A"); break;
        case 0x70: fprintf(out, "MOV    M,B"); break;
        case 0x71: fprintf(out, "MOV    M,C"); break;
        case 0x72: fprintf(out, "MOV    M,D"); break;
        case 0x73: fprintf(out, "MOV    M,E"); break;
        case 0x74: fprintf(out, "MOV    M,H"); break;
        case 0x75: fprintf(out, "MOV    M,L"); break;
        case 0x76: fprintf(out, "HLT"); break;
        case 0x77: fprintf(out, "MOV    M,A"); break;
        case 0x78: fprintf(out, "MOV    A,B"); break;
        case 0x79: fprintf(out, "MOV    A,C"); break;
        case 0x7a: fprintf(out, "MOV    A,D"); break;
        case 0x7b: fprintf(out, "MOV    A,E"); break;
        case 0x7c: fprintf(out, "MOV    A,H"); break;
        case 0x7d: fprintf(out, "MOV    A,L"); break;
        case 0x7e: fprintf(out, "MOV    A,M"); break;
        case 0x7f: fprintf(out, "MOV    A,A"); break;
        case 0x80: fprintf(out, "ADD    B"); break;
        case 0x81: fprintf(out, "ADD    C"); break;
        case 0x82: fprintf(out, "ADD    D"); break;
        case 0x83: fprintf(out, "ADD    E"); break;
        case 0x84: fprintf(out, "ADD    H"); break;
        case 0x85: fprintf(out, "ADD    L"); break;
        case 0x86: fprintf(out, "ADD    M"); break;
        case 0x87: fprintf(out, "ADD    A"); break;
        case 0x88: fprintf(out, "ADC    B"); break;
        case 0x89: fprintf(out, "ADC    C"); break;
        case 0x8a: fprintf(out, "ADC    D"); break;
        case 0x8b: fprintf(out, "ADC    E"); break;
        case 0x8c: fprintf(out, "ADC    H"); break;
        case 0x8d: fprintf(out, "ADC    L"); break;
        case 0x8e: fprintf(out, "ADC    M"); break;
        case 0x8f: fprintf(out, "ADC    A"); break;
        case 0x90: fprintf(out, "SUB    B"); break;
        case 0x91: fprintf(out, "SUB    C"); break;
        case 0x92: fprintf(out, "SUB    D"); break;
        case 0x93: fprintf(out, "SUB    E"); break;
        case 0x94: fprintf(out, "SUB    H"); break;
        case 0x95: fprintf(out, "SUB    L"); break;
        case 0x96: fprintf(out, "SUB    M"); break;
        case 0x97: fprintf(out, "SUB    A"); break;
        case 0x98: fprintf(out, "SBB    B"); break;
        case 0x99: fprintf(out, "SBB    C"); break;
        case 0x9a: fprintf(out, "SBB    D"); break;
        case 0x9b: fprintf(out, "SBB    E"); break;
        case 0x9c: fprintf(out, "SBB    H"); break;
        case 0x9d: fprintf(out, "SBB    L"); break;
        case 0x9e: fprintf(out, "SBB    M"); break;
        case 0x9f: fprintf(out, "SBB    A"); break;
        case 0xa0: fprintf(out, "ANA    B"); break;
        case 0xa1: fprintf(out, "ANA    C"); break;
        case 0xa2: fprintf(out, "ANA    D"); break;
        case 0xa3: fprintf(out, "ANA    E"); break;
        case 0xa4: fprintf(out, "ANA    H"); break;
        case 0xa5: fprintf(out, "ANA    L"); break;
        case 0xa6: fprintf(out, "ANA    M"); break;
        case 0xa7: fprintf(out, "ANA    A"); break;
        case 0xa8: fprintf(out, "XRA    B"); break;
        case 0xa9: fprintf(out, "XRA    C"); break;
        case 0xaa: fprintf(out, "XRA    D"); break;
        case 0xab: fprintf(out, "XRA    E"); break;
        case 0xac: fprintf(out, "XRA    H"); break;
        case 0xad: fprintf(out, "XRA    L"); break;
        case 0xae: fprintf(out, "XRA    M"); break;
        case 0xaf: fprintf(out, "XRA    A"); break;
        case 0xb0: fprintf(out, "ORA    B"); break;
        case 0xb1: fprintf(out, "ORA    C"); break;
        case 0xb2: fprintf(out, "ORA    D"); break;
        case 0xb3: fprintf(out, "ORA    E"); break;
        case 0xb4: fprintf(out, "ORA    H"); break;
        case 0xb5: fprintf(out, "ORA    L"); break;
        case 0xb6: fprintf(out, "ORA    M"); break;
        case 0xb7: fprintf(out, "ORA    A"); break;
        case 0xb8: fprintf(out, "CMP    B"); break;
        case 0xb9: fprintf(out, "CMP    C"); break;
        case 0xba: fprintf(out, "CMP    D"); break;
        case 0xbb: fprintf(out, "CMP    E"); break;
        case 0xbc: fprintf(out, "CMP    H"); break;
        case 0xbd: fprintf(out, "CMP    L"); break;
        case 0xbe: fprintf(out, "CMP    M"); break;
        case 0xbf: fprintf(out, "CMP    A"); break;
        case 0xc0: fprintf(out, "RNZ"); break;
        case 0xc1: fprintf(out, "POP    B"); break;
        case 0xc2: fprintf(out, "JNZ    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc3: fprintf(out, "JMP    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc4: fprintf(out, "CNZ    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xc5: fprintf(out, "PUSH   B"); break;
        case 0xc6: fprintf(out, "ADI    #$%02x", code[1]); opbytes = 2; break;
        case 0xc7: fprintf(out, "RST    0"); break;
        case 0xc8: fprintf(out, "RZ"); break;
        case 0xc9: fprintf(out, "RET"); break;
        case 0xca: fprintf(out, "JZ     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcb: fprintf(out, "NOP"); break;
        case 0xcc: fprintf(out, "CZ     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xcd: fprintf(out, "CALL   $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xce: fprintf(out, "ACI    #$%02x", code[1]); opbytes = 2; break;
        case 0xcf: fprintf(out, "RST    1"); break;
        case 0xd0: fprintf(out, "RNC"); break;
        case 0xd1: fprintf(out, "POP    D"); break;
        case 0xd2: fprintf(out, "JNC    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xd3: fprintf(out, "OUT    #$%02x", code[1]); opbytes = 2; break;
        case 0xd4: fprintf(out, "CNC    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xd5: fprintf(out, "PUSH   D"); break;
        case 0xd6: fprintf(out, "SUI    #$%02x", code[1]); opbytes = 2; break;
        case 0xd7: fprintf(out, "RST    2"); break;
        case 0xd8: fprintf(out, "RC"); break;
        case 0xd9: fprintf(out, "NOP"); break;
        case 0xda: fprintf(out, "JC     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xdb: fprintf(out, "IN     #$%02x", code[1]); opbytes = 2; break;
        case 0xdc: fprintf(out, "CC     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xdd: fprintf(out, "NOP"); break;
        case 0xde: fprintf(out, "SBI    #$%02x", code[1]); opbytes = 2; break;
        case 0xdf: fprintf(out, "RST    3"); break;
        case 0xe0: fprintf(out, "RPO"); break;
        case 0xe1: fprintf(out, "POP    H"); break;
        case 0xe2: fprintf(out, "JPO    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xe3: fprintf(out, "XTHL"); break;
        case 0xe4: fprintf(out, "CPO    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xe5: fprintf(out, "PUSH   H"); break;
        case 0xe6: fprintf(out, "ANI    #$%02x", code[1]); opbytes = 2; break;
        case 0xe7: fprintf(out, "RST    4"); break;
        case 0xe8: fprintf(out, "RPE"); break;
        case 0xe9: fprintf(out, "PCHL"); break;
        case 0xea: fprintf(out, "JPE    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xeb: fprintf(out, "XCHG"); break;
        case 0xec: fprintf(out, "CPE    $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xed: fprintf(out, "NOP"); break;
        case 0xee: fprintf(out, "XRI    #$%02x", code[1]); opbytes = 2; break;
        case 0xef: fprintf(out, "RST    5"); break;
        case 0xf0: fprintf(out, "RP"); break;
        case 0xf1: fprintf(out, "POP  PSW"); break;
        case 0xf2: fprintf(out, "JP     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xf3: fprintf(out, "DI"); break;
        case 0xf4: fprintf(out, "CP     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xf5: fprintf(out, "PUSH PSW"); break;
        case 0xf6: fprintf(out, "ORI    #$%02x", code[1]); opbytes = 2; break;
        case 0xf7: fprintf(out, "RST    6"); break;
        case 0xf8: fprintf(out, "RM"); break;
        case 0xf9: fprintf(out, "SPHL"); break;
        case 0xfa: fprintf(out, "JM     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xfb: fprintf(out, "EI"); break;
        case 0xfc: fprintf(out, "CM     $%02x%02x", code[2], code[1]); opbytes = 3; break;
        case 0xfd: fprintf(out, "NOP"); break;
        case 0xfe: fprintf(out, "CPI    #$%02x", code[1]); opbytes = 2; break;
        case 0xff: fprintf(out, "RST    7"); break;
    }

    fprintf(out, "\n");

    return opbytes;
}
#endif