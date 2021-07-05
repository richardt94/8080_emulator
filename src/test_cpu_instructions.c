#include <stdlib.h>
#include <check.h>
#include "cpu.h"

START_TEST (test_basic_add)
{
    CPUState* cs = newState();
    //test an ADD instruction - set the accumulator to 1
    //and add 2. no fancy carries etc.
    cs->reg[0] = 1;
    cs->reg[1] = 2;
    //ADD B
    cs->memory[0] = 0x80;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 3);
    destroyState(cs);
}
END_TEST

START_TEST (test_add_from_memory)
{
    CPUState* cs = newState();
    //store at address 3ff (1023)
    cs->reg[0] = 1;
    cs->reg[5] = 0x03;
    cs->reg[6] = 0xff;

    cs->memory[0] = 0x86;
    cs->memory[1023] = 2;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 3);
    destroyState(cs);
}
END_TEST

START_TEST (test_add_carry)
{

}
END_TEST

START_TEST (test_basic_sub)
{
    CPUState* cs = newState();
    //test the SUB instruction with 2 - 1
    cs->reg[0] = 2;
    cs->reg[1] = 1;
    //SUB B
    cs->memory[0] = 0x90;
    ck_assert_int_eq(executeOp(cs), 0);
    ck_assert_int_eq(cs->reg[0], 1);
    destroyState(cs);
}
END_TEST

Suite *cpu_suite(void) {
    Suite *s;
    TCase *tc_arithmetic;

    s = suite_create("CPU Instructions");
    tc_arithmetic = tcase_create("Arithmetic instructions");

    tcase_add_test(tc_arithmetic, test_basic_add);
    tcase_add_test(tc_arithmetic, test_add_from_memory);
    tcase_add_test(tc_arithmetic, test_basic_sub);
    tcase_add_test(tc_arithmetic, test_add_carry);

    suite_add_tcase(s, tc_arithmetic);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = cpu_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}