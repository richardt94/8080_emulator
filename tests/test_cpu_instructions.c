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
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[5], 0x22);
    ck_assert_int_eq(cs->reg[6], 0x11);
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

Suite *cpu_suite(void) {
    Suite *s;
    TCase *tc_single;
    TCase *tc_transfer;
    TCase *tc_arithmetic;
    TCase *tc_logcomp;
    TCase *tc_rotate;
    TCase *tc_tworeg;
    TCase *tc_immediate;

    s = suite_create("CPU Instructions");

    tc_single = tcase_create("Single-register ops");
    tc_transfer = tcase_create("Data transfer instructions");
    tc_arithmetic = tcase_create("Arithmetic instructions");
    tc_logcomp = tcase_create("Logical comparisons");
    tc_rotate = tcase_create("Accumulator rotations");
    tc_tworeg = tcase_create("Register pair instructions");
    tc_immediate = tcase_create("Immediate instructions");

    tcase_add_checked_fixture(tc_single, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_transfer, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_arithmetic, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_logcomp, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_rotate, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_tworeg, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_immediate, state_setup, state_teardown);

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

    suite_add_tcase(s, tc_single);
    suite_add_tcase(s, tc_transfer);
    suite_add_tcase(s, tc_arithmetic);
    suite_add_tcase(s, tc_logcomp);
    suite_add_tcase(s, tc_rotate);
    suite_add_tcase(s, tc_tworeg);
    suite_add_tcase(s, tc_immediate);

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
