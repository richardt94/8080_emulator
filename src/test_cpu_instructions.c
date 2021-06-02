#include <assert.h>
#include "cpu.h"

void test_basic_add() {
    CPUState* cs = newState();
    //test an ADD instruction - set the accumulator to 1
    //and add 2. no fancy carries etc.
    cs->reg[0] = 1;
    cs->reg[1] = 2;
    //ADD B
    cs->memory[0] = 0x80;
    assert(executeOp(cs) == 0);
    assert(cs->reg[0] == 3);
    destroyState(cs);
}

void test_add_from_memory() {
    CPUState* cs = newState();
    //store at address 3ff (1023)
    cs->reg[0] = 1;
    cs->reg[5] = 0x03;
    cs->reg[6] = 0xff;

    cs->memory[0] = 0x86;
    cs->memory[1023] = 2;
    assert(executeOp(cs) == 0);
    assert(cs->reg[0] == 3);
    destroyState(cs);
}

void test_basic_sub() {
    CPUState* cs = newState();
    //test the SUB instruction with 2 - 1
    cs->reg[0] = 2;
    cs->reg[1] = 1;
    //SUB B
    cs->memory[0] = 0x90;
    assert(executeOp(cs) == 0);
    assert(cs->reg[0] == 1);
    destroyState(cs);
}

void main()
{
    test_basic_add();
    test_basic_sub();
    test_add_from_memory();
}