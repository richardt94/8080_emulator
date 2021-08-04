#include <stdlib.h>
#include <check.h>
#include "../src/cpu.h"

CPUState* cs;

void state_setup(void) {
    cs = newState(8192);
}

void state_teardown(void) {
    destroyState(cs);
}

START_TEST (test_stc)
{
    cs->memory[0] = 0x37;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_cmc)
{
    cs->memory[0] = 0x3f;
    cs->memory[1] = 0x3f;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.cy, 1);
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_inr)
{
    //increment single register
    cs->reg[1] = 0x0e;
    cs->memory[0] = 0x04;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[1], 0x0f);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 1);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.ac, 0);
}
END_TEST

START_TEST (test_inr_mem)
{
    cs->memory[0] = 0x34;
    cs->memory[0x0e36] = 0xff;
    cs->reg[5] = 0x0e;
    cs->reg[6] = 0x36;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x0e36], 0x00);
    ck_assert_int_eq(cs->fl.z, 1);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 1);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.ac, 1);
}
END_TEST

START_TEST (test_dcr)
{
    cs->memory[0] = 0x05;
    cs->reg[1] = 0xa2;
    ck_assert_int_eq(executeOp(cs),0);
    ck_assert_int_eq(cs->reg[1], 0xa1);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 1);
    ck_assert_int_eq(cs->fl.p, 0);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.ac, 1);
}
END_TEST

START_TEST (test_dcr_mem)
{
    cs->memory[0] = 0x35;
    cs->memory[0x0e36] = 0x00;
    cs->reg[5] = 0x0e;
    cs->reg[6] = 0x36;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x0e36], 0xff);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 1);
    ck_assert_int_eq(cs->fl.p, 1);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.ac, 0);
}
END_TEST

START_TEST (test_daa)
{
    cs->memory[0] = 0x27;
    cs->reg[0] = 0x9b;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x01);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 0);
    ck_assert_int_eq(cs->fl.cy, 1);
    ck_assert_int_eq(cs->fl.ac, 1);
}
END_TEST

START_TEST (test_cma)
{
    cs->memory[0] = 0x2f;
    cs->reg[0] = 0x55;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xaa);
    //no flags affected
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 0);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.ac, 0);
}
END_TEST

START_TEST (test_mov)
{
    cs->memory[0] = 0x41;
    cs->reg[2] = 0x37;
    //source unchanged, no flags affected
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[1], 0x37);
    ck_assert_int_eq(cs->reg[2], 0x37);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 0);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.ac, 0);
}
END_TEST

START_TEST (test_mov_from_mem)
{
    cs->memory[0] = 0x46;
    cs->memory[0x0f98] = 0x24;
    cs->reg[5] = 0x0f;
    cs->reg[6] = 0x98;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[1], 0x24);
    ck_assert_int_eq(cs->memory[0x0f98], 0x24);
}
END_TEST

START_TEST (test_mov_to_mem)
{
    cs->memory[0] = 0x70;
    cs->reg[5] = 0x11;
    cs->reg[6] = 0x22;
    cs->reg[1] = 0xce;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[1], 0xce);
    ck_assert_int_eq(cs->memory[0x1122], 0xce);
}
END_TEST

START_TEST (test_stax)
{
    cs->memory[0] = 0x02;
    cs->reg[0] = 0x4f;
    cs->reg[1] = 0x20;
    cs->reg[2] = 0xbb;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x20bb], 0x4f);
    ck_assert_int_eq(cs->reg[0], 0x4f);
}
END_TEST

START_TEST (test_ldax)
{
    cs->memory[0] = 0x1A;
    cs->memory[0x01b3] = 0xf4;
    cs->reg[3] = 0x01;
    cs->reg[4] = 0xb3;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x01b3], 0xf4);
    ck_assert_int_eq(cs->reg[0], 0xf4);
}
END_TEST

START_TEST (test_basic_add)
{
    //test an ADD instruction - set the accumulator to 1
    //and add 2. no fancy carries etc.
    cs->reg[0] = 1;
    cs->reg[1] = 2;
    //ADD B
    cs->memory[0] = 0x80;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 3);
}
END_TEST

START_TEST (test_add_from_memory)
{
    //store at address 3ff (1023)
    cs->reg[0] = 1;
    cs->reg[5] = 0x03;
    cs->reg[6] = 0xff;

    cs->memory[0] = 0x86;
    cs->memory[1023] = 2;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 3);
}
END_TEST

START_TEST (test_add_carry)
{
    cs->reg[0] = 0xf0;
    cs->reg[1] = 0x10;
    cs->memory[0] = 0x80;

    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_add_aux_carry)
{
    cs->reg[0] = 0x0f;
    cs->reg[1] = 0x01;
    cs->memory[0] = 0x80;

    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.ac, 1);
}
END_TEST

START_TEST (test_add_parity)
{
    cs->reg[0] = 0x2e;
    cs->reg[1] = 0x6c;
    cs->memory[0] = 0x80;

    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.p, 1);
}
END_TEST

START_TEST (test_adi)
{
    //immediate add (two-byte instruction)
    cs->reg[0] = 0x0f;
    cs->memory[0] = 0xc6;
    cs->memory[1] = 0xf0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xff);
    ck_assert_int_eq(cs->pc, 2);
}
END_TEST

START_TEST (test_adc_set)
{
    //add using the carry bit
    cs->reg[0] = 0x0f;
    cs->reg[1] = 0xf0;
    cs->fl.cy = 1;
    cs->memory[0] = 0x88;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_adc_reset)
{
    //make sure the carry bit is reset if the result of ADC doesn't
    //result in a further carry
    cs->reg[0] = 0x08;
    cs->reg[1] = 0x07;
    cs->fl.cy = 1;
    cs->memory[0] = 0x88;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x10);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_basic_sub)
{
    //test the SUB instruction with 2 - 1
    cs->reg[0] = 2;
    cs->reg[1] = 1;
    //SUB B
    cs->memory[0] = 0x90;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 1);
}
END_TEST

START_TEST (test_sub_greater)
{
    //number greater than acc should set the carry ("borrow") bit
    cs->reg[0] = 0x0e;
    cs->reg[1] = 0x12;
    cs->memory[0] = 0x90;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.cy, 1);
    //result should be negative
    ck_assert_int_eq(cs->fl.s, 1);
}
END_TEST

START_TEST (test_sub_self_reset) {
    //SUB A should reset the carry bit and clear the accumulator
    cs->reg[0] = 0x3e;
    cs->memory[0] = 0x97;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_sub_aux_carry) {
    //aux flag works same way as adding.
    cs->reg[0] = 8;
    cs->reg[1] = 1;
    cs->memory[0] = 0x90;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.ac, 1);
}
END_TEST

START_TEST (test_subi) {
    //immediate sub
    cs->reg[0] = 0xff;
    cs->memory[0] = 0xd6;
    cs->memory[1] = 0x0f;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xf0);
    ck_assert_int_eq(cs->pc, 2);
}
END_TEST

START_TEST (test_sbb_set)
{
    //sub with borrow, result still negative
    cs->reg[0] = 0x07;
    cs->reg[1] = 0x07;
    cs->fl.cy = 1;
    cs->memory[0] = 0x98;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xff);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_sbb_reset)
{
    //sub with borrow, result now positive
    cs->reg[0] = 0x11;
    cs->reg[1] = 0x0f;
    cs->fl.cy = 1;
    cs->memory[0] = 0x98;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x01);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_ana)
{
    cs->reg[0] = 0xfc;
    cs->reg[1] = 0x0f;
    cs->memory[0] = 0xa0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x0c);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 1);
}
END_TEST

START_TEST (test_xra)
{
    cs->reg[0] = 0x5c;
    cs->reg[1] = 0x78;
    cs->memory[0] = 0xa8;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x24);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 1);
}
END_TEST

START_TEST (test_ora)
{
    cs->reg[0] = 0x33;
    cs->reg[1] = 0x0f;
    cs->memory[0] = 0xb0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x3f);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.z, 0);
    ck_assert_int_eq(cs->fl.s, 0);
    ck_assert_int_eq(cs->fl.p, 1);
}
END_TEST

START_TEST (test_cmp)
{
    cs->reg[0] = 0x02;
    cs->reg[1] = 0x05;
    cs->memory[0] = 0xb8;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x02);
    ck_assert_int_eq(cs->fl.cy, 1);
    ck_assert_int_eq(cs->fl.z, 0);
}
END_TEST

START_TEST (test_cmp_opp_sign)
{
    cs->reg[0] = 0xe5;
    cs->reg[1] = 0x05;
    cs->memory[0] = 0xb8;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xe5);
    ck_assert_int_eq(cs->fl.cy, 0);
    ck_assert_int_eq(cs->fl.z, 0);
}
END_TEST

START_TEST (test_rlc)
{
    cs->memory[0] = 0x07;
    cs->reg[0] = 0xf2;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xe5);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_rrc)
{
    cs->memory[0] = 0x0f;
    cs->reg[0] = 0xf2;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x79);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_ral)
{
    cs->memory[0] = 0x17;
    cs->reg[0] = 0xb5;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x6a);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_rar)
{
    cs->memory[0] = 0x1f;
    cs->reg[0] = 0x6a;
    cs->fl.cy = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0xb5);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_push)
{
    //sp must be initialised
    //by the programmer before stack
    //operations have defined behaviour.
    //for these tests we just manually
    //set it ourselves.
    cs->sp = 8192;
    cs->memory[0] = 0xc5;
    cs->reg[1] = 0x11;
    cs->reg[2] = 0x22;
    int old_sp = cs->sp;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(old_sp - cs->sp, 2);
    ck_assert_int_eq(cs->memory[cs->sp], 0x22);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x11);
}
END_TEST

START_TEST (test_push_psw)
{
    cs->sp = 8192;
    cs->memory[0] = 0xf5;
    cs->fl.cy = 1;
    cs->fl.z = 1;
    cs->fl.p = 1;
    cs->reg[0] = 0x1f;
    int old_sp = cs->sp;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(old_sp - cs->sp, 2);
    ck_assert_int_eq(cs->memory[cs->sp], 0x47);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x1f);
}
END_TEST

START_TEST (test_pop)
{
    cs->sp = 8192;
    cs->memory[0] = 0xc1;
    cs->sp -= 2;
    cs->memory[cs->sp] = 0x3d;
    cs->memory[cs->sp + 1] = 0x93;
    int old_sp = cs->sp;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp - old_sp, 2);
    ck_assert_int_eq(cs->reg[1], 0x93);
    ck_assert_int_eq(cs->reg[2], 0x3d);
}
END_TEST

START_TEST (test_pop_psw)
{
    cs->sp = 8192;
    cs->memory[0] = 0xf1;
    cs->sp -= 2;
    cs->memory[cs->sp] = 0xc3;
    cs->memory[cs->sp + 1] = 0xff;
    int old_sp = cs->sp;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp - old_sp, 2);
    ck_assert_int_eq(cs->reg[0], 0xff);
    ck_assert_int_eq(cs->fl.cy, 1);
    ck_assert_int_eq(cs->fl.p, 0);
    ck_assert_int_eq(cs->fl.ac, 0);
    ck_assert_int_eq(cs->fl.z, 1);
    ck_assert_int_eq(cs->fl.s, 1);
}
END_TEST

START_TEST (test_dad)
{
    //HL is the "double accumulator"
    cs->memory[0] = 0x09;
    cs->reg[1] = 0x33;
    cs->reg[2] = 0x9f;
    cs->reg[5] = 0xa1;
    cs->reg[6] = 0x7b;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[6], 0x1a);
    ck_assert_int_eq(cs->reg[5], 0xd5);
    ck_assert_int_eq(cs->fl.cy, 0);
}
END_TEST

START_TEST (test_dad_sp)
{
    cs->memory[0] = 0x39;
    cs->sp = 0x339f;
    cs->reg[5] = 0xd1;
    cs->reg[6] = 0x7b;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[6], 0x1a);
    ck_assert_int_eq(cs->reg[5], 0x05);
    ck_assert_int_eq(cs->fl.cy, 1);
}
END_TEST

START_TEST (test_inx)
{
    cs->memory[0] = 0x13;
    cs->reg[3] = 0x38;
    cs->reg[4] = 0xff;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[3], 0x39);
    ck_assert_int_eq(cs->reg[4], 0x00);
}
END_TEST

START_TEST (test_inx_sp)
{
    cs->memory[0] = 0x33;
    cs->sp = 0xffff;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp, 0x0000);
    ck_assert_int_eq(cs->fl.cy, 0); //inx doesn't carry
}
END_TEST

START_TEST (test_dcx)
{
    cs->memory[0] = 0x2b;
    cs->reg[5] = 0x98;
    cs->reg[6] = 0x00;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[6], 0xff);
    ck_assert_int_eq(cs->reg[5], 0x97);
}
END_TEST

START_TEST (test_dcx_sp)
{
    cs->memory[0] = 0x3b;
    cs->sp = 0x0000;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp, 0xffff);
    ck_assert_int_eq(cs->fl.cy, 0); //dcx doesn't carry
}
END_TEST

START_TEST (test_xchg)
{
    cs->memory[0] = 0xeb;
    cs->reg[3] = 0x11;
    cs->reg[4] = 0x22;
    cs->reg[5] = 0x33;
    cs->reg[6] = 0x44;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[3], 0x33);
    ck_assert_int_eq(cs->reg[4], 0x44);
    ck_assert_int_eq(cs->reg[5], 0x11);
    ck_assert_int_eq(cs->reg[6], 0x22);
}
END_TEST

START_TEST (test_xthl)
{
    cs->memory[0] = 0xe3;
    cs->sp = 8190;
    cs->memory[cs->sp] = 0x11;
    cs->memory[cs->sp + 1] = 0x22;
    cs->reg[5] = 0x33;
    cs->reg[6] = 0x44;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[5], 0x22);
    ck_assert_int_eq(cs->reg[6], 0x11);
    ck_assert_int_eq(cs->sp, 8190);
    ck_assert_int_eq(cs->memory[cs->sp], 0x44);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x33);
}
END_TEST

START_TEST (test_sphl)
{
    cs->memory[0] = 0xf9;
    cs->sp = 0;
    cs->reg[5] = 0x11;
    cs->reg[6] = 0x22;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp, 0x1122);
}
END_TEST

START_TEST (test_lxi)
{
    cs->memory[0] = 0x21;
    cs->memory[1] = 0xab;
    cs->memory[2] = 0xcd;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[5], 0xcd);
    ck_assert_int_eq(cs->reg[6], 0xab);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_lxi_sp)
{
    cs->memory[0] = 0x31;
    cs->memory[1] = 0x12;
    cs->memory[2] = 0x34;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp, 0x3412);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_mvi)
{
    cs->memory[0] = 0x1e;
    cs->memory[1] = 0x5a;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[4], 0x5a);
    ck_assert_int_eq(cs->pc, 2);
}
END_TEST

START_TEST (test_mvi_mem)
{
    cs->memory[0] = 0x36;
    cs->memory[1] = 0x7a;
    cs->reg[5] = 0x1a;
    cs->reg[6] = 0x2b;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x1a2b], 0x7a);
    ck_assert_int_eq(cs->pc, 2);
}
END_TEST

START_TEST (test_sta)
{
    cs->memory[0] = 0x32;
    cs->memory[1] = 0xb3;
    cs->memory[2] = 0x05;
    cs->reg[0] = 0x12;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x05b3], 0x12);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_lda)
{
    cs->memory[0] = 0x3a;
    cs->memory[1] = 0xb3;
    cs->memory[2] = 0x05;
    cs->memory[0x05b3] = 0x12;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 0x12);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_shld)
{
    cs->memory[0] = 0x22;
    cs->memory[1] = 0x0a;
    cs->memory[2] = 0x01;
    cs->reg[5] = 0xae;
    cs->reg[6] = 0x29;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->memory[0x010a], 0x29);
    ck_assert_int_eq(cs->memory[0x010b], 0xae);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_lhld)
{
    cs->memory[0] = 0x2a;
    cs->memory[1] = 0x5b;
    cs->memory[2] = 0x02;
    cs->memory[0x025b] = 0xff;
    cs->memory[0x025c] = 0x03;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[5], 0x03);
    ck_assert_int_eq(cs->reg[6], 0xff);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_jmp)
{
    cs->memory[0] = 0xc3;
    cs->memory[1] = 0x28;
    cs->memory[2] = 0x1c;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x1c28);
}
END_TEST

START_TEST (test_jc)
{
    cs->memory[0] = 0xda;
    cs->memory[1] = 0x90;
    cs->memory[2] = 0x13;
    cs->fl.cy = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x1390);
}
END_TEST

START_TEST(test_jc_no_jump)
{
    cs->memory[0] = 0xda;
    cs->memory[1] = 0x90;
    cs->memory[2] = 0x13;
    cs->fl.cy = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 3);
}
END_TEST

START_TEST (test_jnc)
{
    cs->memory[0] = 0xd2;
    cs->memory[1] = 0xde;
    cs->memory[2] = 0x03;
    cs->fl.cy = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x03de);
}
END_TEST

START_TEST (test_jz)
{
    cs->memory[0] = 0xca;
    cs->memory[1] = 0x10;
    cs->memory[2] = 0x01;
    cs->fl.z = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0110);
}
END_TEST

START_TEST (test_jnz)
{
    cs->memory[0] = 0xc2;
    cs->memory[1] = 0x01;
    cs->memory[2] = 0x10;
    cs->fl.z = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x1001);
}
END_TEST

START_TEST (test_jm)
{
    cs->memory[0] = 0xfa;
    cs->memory[1] = 0x21;
    cs->memory[2] = 0x03;
    cs->fl.s = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0321);
}
END_TEST

START_TEST (test_jp)
{
    cs->memory[0] = 0xf2;
    cs->memory[1] = 0xba;
    cs->memory[2] = 0x0c;
    cs->fl.s = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0cba);
}
END_TEST

START_TEST (test_jpe)
{
    cs->memory[0] = 0xea;
    cs->memory[1] = 0xed;
    cs->memory[2] = 0x0f;
    cs->fl.p = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0fed);   
}
END_TEST

START_TEST (test_jpo)
{
    cs->memory[0] = 0xe2;
    cs->memory[1] = 0xef;
    cs->memory[2] = 0x0d;
    cs->fl.p = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0def);
}
END_TEST

START_TEST (test_call)
{
    //CALL pushes return address
    //onto the stack then does a JMP
    //to the indicated address
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xcd;
    cs->memory[cs->pc+1] = 0xcd;
    cs->memory[cs->pc+2] = 0x0b;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0bcd);
    //return to instruction after the CALL
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cc)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xdc;
    cs->memory[cs->pc+1] = 0x90;
    cs->memory[cs->pc+2] = 0x13;
    cs->fl.cy = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x1390);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST(test_cc_no_jump)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xdc;
    cs->memory[cs->pc+1] = 0x90;
    cs->memory[cs->pc+2] = 0x13;
    cs->fl.cy = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);//stack still empty
}
END_TEST

START_TEST (test_cnc)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xd4;
    cs->memory[cs->pc+1] = 0xde;
    cs->memory[cs->pc+2] = 0x03;
    cs->fl.cy = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x03de);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cz)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xcc;
    cs->memory[cs->pc+1] = 0x10;
    cs->memory[cs->pc+2] = 0x01;
    cs->fl.z = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0110);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cnz)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xc4;
    cs->memory[cs->pc+1] = 0x01;
    cs->memory[cs->pc+2] = 0x10;
    cs->fl.z = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x1001);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cm)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xfc;
    cs->memory[cs->pc+1] = 0x21;
    cs->memory[cs->pc+2] = 0x03;
    cs->fl.s = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0321);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cp)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xf4;
    cs->memory[cs->pc+1] = 0xba;
    cs->memory[cs->pc+2] = 0x0c;
    cs->fl.s = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0cba);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cpe)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xec;
    cs->memory[cs->pc+1] = 0xed;
    cs->memory[cs->pc+2] = 0x0f;
    cs->fl.p = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0fed);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_cpo)
{
    cs->sp = 8192;
    cs->pc = 0x0abc;
    cs->memory[cs->pc] = 0xe4;
    cs->memory[cs->pc+1] = 0xef;
    cs->memory[cs->pc+2] = 0x0d;
    cs->fl.p = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0def);
    ck_assert_int_eq(cs->memory[cs->sp], 0xbf);
    ck_assert_int_eq(cs->memory[cs->sp+1], 0x0a);
}
END_TEST

START_TEST (test_ret)
{
    //pop an address off the stack and go there
    cs->sp = 8192;
    cs->sp -= 2;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->pc = 0x0bcd;
    cs->memory[cs->pc] = 0xc9;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp, 8192);
    ck_assert_int_eq(cs->pc, 0x0abf);
}
END_TEST


START_TEST (test_rc)
{
    cs->sp = 8190;
    cs->pc = 0x1390;
    cs->memory[cs->pc] = 0xd8;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.cy = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST(test_rc_no_jump)
{
    cs->sp = 8190;
    cs->pc = 0x1390;
    cs->memory[cs->pc] = 0xd8;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.cy = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x1391);
    ck_assert_int_eq(cs->sp, 8190);//stack still filled
}
END_TEST

START_TEST (test_rnc)
{
    cs->sp = 8190;
    cs->pc = 0x03de;
    cs->memory[cs->pc] = 0xd0;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.cy = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST (test_rz)
{
    cs->sp = 8190;
    cs->pc = 0x0110;
    cs->memory[cs->pc] = 0xc8;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.z = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST (test_rnz)
{
    cs->sp = 8190;
    cs->pc = 0x1001;
    cs->memory[cs->pc] = 0xc0;
    cs->memory[cs->sp] = 0x01;
    cs->memory[cs->sp+1] = 0x10;
    cs->fl.z = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST (test_rm)
{
    cs->sp = 8190;
    cs->pc = 0x0321;
    cs->memory[cs->pc] = 0xf8;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.s = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST (test_rp)
{
    cs->sp = 8190;
    cs->pc = 0x0cba;
    cs->memory[cs->pc] = 0xf0;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.s = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST (test_rpe)
{
    cs->sp = 8190;
    cs->pc = 0x0fed;
    cs->memory[cs->pc] = 0xe8;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.p = 1;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->pc, 0x0abf);
    ck_assert_int_eq(cs->sp, 8192);
}
END_TEST

START_TEST (test_rpo)
{
    cs->sp = 8190;
    cs->pc = 0x0def;
    cs->memory[cs->pc] = 0xe0;
    cs->memory[cs->sp] = 0xbf;
    cs->memory[cs->sp+1] = 0x0a;
    cs->fl.p = 0;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->sp, 8192);
    ck_assert_int_eq(cs->pc, 0x0abf);
}
END_TEST

Suite *cpu_suite(void) {
    Suite *s;

    TCase *tc_carry;
    TCase *tc_single;
    TCase *tc_transfer;
    TCase *tc_arithmetic;
    TCase *tc_logcomp;
    TCase *tc_rotate;
    TCase *tc_tworeg;
    TCase *tc_immediate;
    TCase *tc_direct;
    TCase *tc_jumps;
    TCase *tc_calls;
    TCase *tc_rets;

    s = suite_create("CPU Instructions");

    tc_carry = tcase_create("Carry bit instructions");
    tc_single = tcase_create("Single-register ops");
    tc_transfer = tcase_create("Data transfer instructions");
    tc_arithmetic = tcase_create("Arithmetic instructions");
    tc_logcomp = tcase_create("Logical comparisons");
    tc_rotate = tcase_create("Accumulator rotations");
    tc_tworeg = tcase_create("Register pair instructions");
    tc_immediate = tcase_create("Immediate instructions");
    tc_direct = tcase_create("Direct addressing instructions");
    tc_jumps = tcase_create("Jumps");
    tc_calls = tcase_create("Calls");
    tc_rets = tcase_create("Returns");

    tcase_add_checked_fixture(tc_carry, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_single, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_transfer, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_arithmetic, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_logcomp, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_rotate, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_tworeg, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_immediate, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_direct, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_jumps, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_calls, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_rets, state_setup, state_teardown);

    tcase_add_test(tc_carry, test_stc);
    tcase_add_test(tc_carry, test_cmc);

    tcase_add_test(tc_single, test_inr);
    tcase_add_test(tc_single, test_inr_mem);
    tcase_add_test(tc_single, test_dcr);
    tcase_add_test(tc_single, test_dcr_mem);
    tcase_add_test(tc_single, test_daa);
    tcase_add_test(tc_single, test_cma);

    tcase_add_test(tc_transfer, test_mov);
    tcase_add_test(tc_transfer, test_mov_from_mem);
    tcase_add_test(tc_transfer, test_mov_to_mem);
    tcase_add_test(tc_transfer, test_stax);
    tcase_add_test(tc_transfer, test_ldax);

    tcase_add_test(tc_arithmetic, test_basic_add);
    tcase_add_test(tc_arithmetic, test_add_from_memory);
    tcase_add_test(tc_arithmetic, test_add_carry);
    tcase_add_test(tc_arithmetic, test_add_aux_carry);
    tcase_add_test(tc_arithmetic, test_add_parity);
    tcase_add_test(tc_arithmetic, test_adi);
    tcase_add_test(tc_arithmetic, test_adc_set);
    tcase_add_test(tc_arithmetic, test_adc_reset);
    tcase_add_test(tc_arithmetic, test_basic_sub);
    tcase_add_test(tc_arithmetic, test_sub_greater);
    tcase_add_test(tc_arithmetic, test_sub_self_reset);
    tcase_add_test(tc_arithmetic, test_sub_aux_carry);
    tcase_add_test(tc_arithmetic, test_subi);
    tcase_add_test(tc_arithmetic, test_sbb_set);
    tcase_add_test(tc_arithmetic, test_sbb_reset);

    tcase_add_test(tc_logcomp, test_ana);
    tcase_add_test(tc_logcomp, test_xra);
    tcase_add_test(tc_logcomp, test_ora);
    tcase_add_test(tc_logcomp, test_cmp);
    tcase_add_test(tc_logcomp, test_cmp_opp_sign);

    tcase_add_test(tc_rotate, test_rlc);
    tcase_add_test(tc_rotate, test_rrc);
    tcase_add_test(tc_rotate, test_ral);
    tcase_add_test(tc_rotate, test_rar);

    tcase_add_test(tc_tworeg, test_push);
    tcase_add_test(tc_tworeg, test_push_psw);
    tcase_add_test(tc_tworeg, test_pop);
    tcase_add_test(tc_tworeg, test_pop_psw);
    tcase_add_test(tc_tworeg, test_dad);
    tcase_add_test(tc_tworeg, test_dad_sp);
    tcase_add_test(tc_tworeg, test_inx);
    tcase_add_test(tc_tworeg, test_inx_sp);
    tcase_add_test(tc_tworeg, test_dcx);
    tcase_add_test(tc_tworeg, test_dcx_sp);
    tcase_add_test(tc_tworeg, test_xchg);
    tcase_add_test(tc_tworeg, test_xthl);
    tcase_add_test(tc_tworeg, test_sphl);

    tcase_add_test(tc_immediate, test_lxi);
    tcase_add_test(tc_immediate, test_lxi_sp);
    tcase_add_test(tc_immediate, test_mvi);
    tcase_add_test(tc_immediate, test_mvi_mem);

    tcase_add_test(tc_direct, test_sta);
    tcase_add_test(tc_direct, test_lda);
    tcase_add_test(tc_direct, test_shld);
    tcase_add_test(tc_direct, test_lhld);

    tcase_add_test(tc_jumps, test_jmp);
    tcase_add_test(tc_jumps, test_jc);
    tcase_add_test(tc_jumps, test_jc_no_jump);
    tcase_add_test(tc_jumps, test_jnc);
    tcase_add_test(tc_jumps, test_jz);
    tcase_add_test(tc_jumps, test_jnz);
    tcase_add_test(tc_jumps, test_jm);
    tcase_add_test(tc_jumps, test_jp);
    tcase_add_test(tc_jumps, test_jpe);
    tcase_add_test(tc_jumps, test_jpo);

    tcase_add_test(tc_calls, test_call);
    tcase_add_test(tc_calls, test_cc);
    tcase_add_test(tc_calls, test_cc_no_jump);
    tcase_add_test(tc_calls, test_cnc);
    tcase_add_test(tc_calls, test_cz);
    tcase_add_test(tc_calls, test_cnz);
    tcase_add_test(tc_calls, test_cm);
    tcase_add_test(tc_calls, test_cp);
    tcase_add_test(tc_calls, test_cpe);
    tcase_add_test(tc_calls, test_cpo);

    tcase_add_test(tc_rets, test_ret);
    tcase_add_test(tc_rets, test_rc);
    tcase_add_test(tc_rets, test_rc_no_jump);
    tcase_add_test(tc_rets, test_rnc);
    tcase_add_test(tc_rets, test_rz);
    tcase_add_test(tc_rets, test_rnz);
    tcase_add_test(tc_rets, test_rm);
    tcase_add_test(tc_rets, test_rp);
    tcase_add_test(tc_rets, test_rpe);
    tcase_add_test(tc_rets, test_rpo);


    suite_add_tcase(s, tc_carry);
    suite_add_tcase(s, tc_single);
    suite_add_tcase(s, tc_transfer);
    suite_add_tcase(s, tc_arithmetic);
    suite_add_tcase(s, tc_logcomp);
    suite_add_tcase(s, tc_rotate);
    suite_add_tcase(s, tc_tworeg);
    suite_add_tcase(s, tc_immediate);
    suite_add_tcase(s, tc_direct);
    suite_add_tcase(s, tc_jumps);
    suite_add_tcase(s, tc_calls);
    suite_add_tcase(s, tc_rets);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = cpu_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
