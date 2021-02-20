#include "disassemble.h"
#include <stdlib.h>

int main (int argc, char **argv)
{
    if (argc < 2)
    {
        printf("error: must supply a filename to disassemble\n");
        exit(1);
    }
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        printf("error: failed to open ROM file %s\n", argv[1]);
        exit(1);
    }

    // find size of the file and read all the bytes
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    byte *buf = malloc(fsize);
    fread(buf, fsize, 1, f);
    fclose(f);

    int pc = 0;
    while (pc < fsize)
    {
        pc += disassembleOp(buf, pc, stdout);
    }

    return 0;
}