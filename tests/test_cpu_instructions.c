#include <stdlib.h>
#include <check.h>
#include "../src/cpu.h"

CPUState* cs;

void state_setup(void) {
    cs = newState();
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

Suite *cpu_suite(void) {
    Suite *s;
    TCase *tc_single;
    TCase *tc_transfer;
    TCase *tc_arithmetic;
    TCase *tc_logcomp;

    s = suite_create("CPU Instructions");
    tc_single = tcase_create("Single-register ops");
    tc_transfer = tcase_create("Data transfer instructions");
    tc_arithmetic = tcase_create("Arithmetic instructions");
    tc_logcomp = tcase_create("Logical comparisons");
    tcase_add_checked_fixture(tc_single, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_transfer, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_arithmetic, state_setup, state_teardown);
    tcase_add_checked_fixture(tc_logcomp, state_setup, state_teardown);

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

    suite_add_tcase(s, tc_single);
    suite_add_tcase(s, tc_transfer);
    suite_add_tcase(s, tc_arithmetic);
    suite_add_tcase(s, tc_logcomp);

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
