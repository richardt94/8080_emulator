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

START_TEST (test_sub_carry)
{
    //number greater than acc should set the carry ("borrow") bit
    cs->reg[0] = 0x0e;
    cs->reg[1] = 0x12;
    cs->memory[0] = 0x90;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->fl.cy, 1);
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

Suite *cpu_suite(void) {
    Suite *s;
    TCase *tc_arithmetic;

    s = suite_create("CPU Instructions");
    tc_arithmetic = tcase_create("Arithmetic instructions");
    tcase_add_checked_fixture(tc_arithmetic, state_setup, state_teardown);

    tcase_add_test(tc_arithmetic, test_basic_add);
    tcase_add_test(tc_arithmetic, test_add_from_memory);
    tcase_add_test(tc_arithmetic, test_basic_sub);
    tcase_add_test(tc_arithmetic, test_add_carry);
    tcase_add_test(tc_arithmetic, test_add_aux_carry);
    tcase_add_test(tc_arithmetic, test_sub_carry);
    tcase_add_test(tc_arithmetic, test_sub_self_reset);

    suite_add_tcase(s, tc_arithmetic);

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
