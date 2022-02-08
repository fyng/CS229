#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
// TODO: add more test functions
void test_add_two_zeros(TestObjs *objs);
void test_add_same_numbers(TestObjs *objs);
void test_add_opposite_numbers(TestObjs *objs);
void test_add_two_positive(TestObjs *objs);
void test_add_two_negative(TestObjs *objs);
void test_add_smallneg(TestObjs *objs);
void test_add_bigneg(TestObjs *objs);
void test_add_frac_overflow(TestObjs *objs);
void test_add_whole_overflow(TestObjs *objs);
void test_add_whole_overflow_neg(TestObjs *objs);
void test_add_frac_whole_overflow(TestObjs *objs);
void test_add_frac_whole_overflow_one(TestObjs *objs);
void test_double(TestObjs *objs);
void test_double_neg(TestObjs *objs);
void test_double_whole_overflow(TestObjs *objs);
void test_add_whole_and_frac(TestObjs *objs);
void test_double_whole_overflow_neg(TestObjs *objs);
void test_double_frac_overflow(TestObjs *objs);
void test_halve_whole(TestObjs *objs);
void test_halve_frac(TestObjs *objs);
void test_halve_odd_whole(TestObjs *objs);
void test_halve_neg(TestObjs *objs);
void test_halve_underflow_pos(TestObjs *objs);
void test_halve_underflow_neg(TestObjs *objs);
void test_compare_pos_neg(TestObjs *objs);
void test_compare_two_pos_whole(TestObjs *objs);
void test_compare_two_pos_frac(TestObjs *objs);
void test_compare_two_neg_whole(TestObjs *objs);
void test_compare_two_neg_frac(TestObjs *objs);
void test_compare_equals(TestObjs *objs);
void test_compare_equals_zeros(TestObjs *objs);

int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_is_overflow_pos);
  TEST(test_is_err);

  // Custome Add Tests
  TEST(test_add_two_zeros);
  TEST(test_add_same_numbers);
  TEST(test_add_opposite_numbers);
  TEST(test_add_two_positive);
  TEST(test_add_two_negative);
  TEST(test_add_smallneg);
  TEST(test_add_bigneg);
  TEST(test_add_frac_overflow);
  TEST(test_add_whole_overflow);
  TEST(test_add_whole_overflow_neg);
  TEST(test_add_frac_whole_overflow);
  TEST(test_add_frac_whole_overflow_one);
  TEST(test_add_whole_and_frac);

  //Custom Double Tests
  TEST(test_double);
  TEST(test_double_neg);
  TEST(test_double_whole_overflow);
  TEST(test_double_whole_overflow_neg);
  TEST(test_double_frac_overflow);

  //Custom Halve Tests
  TEST(test_halve_whole);
  TEST(test_halve_frac);
  TEST(test_halve_odd_whole);
  TEST(test_halve_neg);
  TEST(test_halve_underflow_pos);
  TEST(test_halve_underflow_neg);

  //Custom Compare Tests
  TEST(test_compare_pos_neg);
  TEST(test_compare_two_pos_whole);
  TEST(test_compare_two_pos_frac);
  TEST(test_compare_two_neg_whole);
  TEST(test_compare_two_neg_frac);
  TEST(test_compare_equals);
  TEST(test_compare_equals_zeros);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

// Adding Custom tests
void test_add_two_zeros(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(0, 0);
  rhs = fixedpoint_create2(0, 0);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 0);
  ASSERT(fixedpoint_frac_part(sum) == 0);
}

void test_add_same_numbers(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(1, 1);
  rhs = fixedpoint_create2(1, 1);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 2);
  ASSERT(fixedpoint_frac_part(sum) == 2);
}

void test_add_opposite_numbers(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(1, 1);
  rhs = fixedpoint_create2(1, 1);
  rhs = fixedpoint_negate(rhs);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 0);
  ASSERT(fixedpoint_frac_part(sum) == 0);
}

void test_add_two_positive(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(25, 25);
  rhs = fixedpoint_create2(30, 30);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 55);
  ASSERT(fixedpoint_frac_part(sum) == 55);
}

void test_add_two_negative(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(25, 25);
  rhs = fixedpoint_create2(30, 30);
  lhs = fixedpoint_negate(lhs);
  rhs = fixedpoint_negate(rhs);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(fixedpoint_whole_part(sum) == 55);
  ASSERT(fixedpoint_frac_part(sum) == 55);
}

void test_add_smallneg(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(25, 25);
  rhs = fixedpoint_create2(30, 30);
  lhs = fixedpoint_negate(lhs);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 5);
  ASSERT(fixedpoint_frac_part(sum) == 5);
}

void test_add_bigneg(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(25, 25);
  rhs = fixedpoint_create2(30, 30);
  rhs = fixedpoint_negate(rhs);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(fixedpoint_whole_part(sum) == 5);
  ASSERT(fixedpoint_frac_part(sum) == 5);
}

void test_add_whole_and_frac(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(2, 1UL);
  rhs = fixedpoint_create2(0, 0xFFFFFFFFFFFFFFFFUL);
  rhs = fixedpoint_negate(rhs);
  sum = fixedpoint_add(rhs, lhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 1);
  ASSERT(fixedpoint_frac_part(sum) == 2UL);
}

void test_add_frac_overflow(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create2(25, 18446744073709551615UL);
  rhs = fixedpoint_create2(30, 1);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 56);
  ASSERT(fixedpoint_frac_part(sum) == 0);
}

void test_add_whole_overflow(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF");
  rhs = fixedpoint_create_from_hex("1.0");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 0);
  ASSERT(fixedpoint_frac_part(sum) == 0xFFFFFFFFFFFFFFFFUL);
  ASSERT(fixedpoint_is_overflow_neg(sum) == 0);
  ASSERT(fixedpoint_is_overflow_pos(sum));
  ASSERT(fixedpoint_is_underflow_neg(sum) == 0);
  ASSERT(fixedpoint_is_underflow_pos(sum) == 0);
}

void test_add_whole_overflow_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF");
  rhs = fixedpoint_create_from_hex("-1.0");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(fixedpoint_whole_part(sum) == 0);
  ASSERT(fixedpoint_frac_part(sum) == 0xFFFFFFFFFFFFFFFFUL);
  ASSERT(fixedpoint_is_overflow_neg(sum));
  ASSERT(fixedpoint_is_overflow_pos(sum) == 0);
}

void test_add_frac_whole_overflow(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF");
  rhs = fixedpoint_create2(0, 1);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_frac_part(sum) == 0);
  ASSERT(fixedpoint_whole_part(sum) == 0);
  ASSERT(fixedpoint_is_overflow_pos(sum));
  ASSERT(fixedpoint_is_overflow_neg(sum) == 0);
}

void test_add_frac_whole_overflow_one(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF");
  rhs = fixedpoint_create2(1, 2);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum) == 0);
  ASSERT(fixedpoint_frac_part(sum) == 1);
  ASSERT(fixedpoint_whole_part(sum) == 1);
  ASSERT(fixedpoint_is_overflow_pos(sum));
  ASSERT(fixedpoint_is_overflow_neg(sum) == 0);
}

void test_double(TestObjs *objs) {
  (void) objs;

  Fixedpoint undoubled, doubled;

  undoubled = fixedpoint_create2(5, 4);
  doubled = fixedpoint_double(undoubled);
  ASSERT(fixedpoint_is_neg(doubled) == 0);
  ASSERT(fixedpoint_whole_part(doubled) == 10);
  ASSERT(fixedpoint_frac_part(doubled) == 8);
}

void test_double_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint undoubled, doubled;
  
  undoubled = fixedpoint_create2(5, 4);
  undoubled = fixedpoint_negate(undoubled);
  doubled = fixedpoint_double(undoubled);
  ASSERT(fixedpoint_is_neg(doubled));
  ASSERT(fixedpoint_whole_part(doubled) == 10);
  ASSERT(fixedpoint_frac_part(doubled) == 8);
}

void test_double_whole_overflow(TestObjs *objs) {
  (void) objs;

  Fixedpoint undoubled, doubled;

  undoubled = fixedpoint_create_from_hex("FFFFFFFFFFFFFFFF.0");
  doubled = fixedpoint_double(undoubled);
  ASSERT(fixedpoint_is_neg(doubled) == 0);
  ASSERT(fixedpoint_whole_part(doubled) == 0xFFFFFFFFFFFFFFFEUL);
  ASSERT(fixedpoint_frac_part(doubled) == 0);
  ASSERT(fixedpoint_is_overflow_pos(doubled));
}

void test_double_whole_overflow_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint undoubled, doubled;

  undoubled = fixedpoint_create_from_hex("-FFFFFFFFFFFFFFFF.0");
  doubled = fixedpoint_double(undoubled);
  ASSERT(fixedpoint_is_neg(doubled));
  ASSERT(fixedpoint_whole_part(doubled) == 0xFFFFFFFFFFFFFFFEUL);
  ASSERT(fixedpoint_frac_part(doubled) == 0);
  ASSERT(fixedpoint_is_overflow_neg(doubled));
  }

void test_double_frac_overflow(TestObjs *objs) {
  (void) objs;

  Fixedpoint undoubled, doubled;

  undoubled = fixedpoint_create_from_hex("0.FFFFFFFFFFFFFFFF");
  doubled = fixedpoint_double(undoubled);
  ASSERT(fixedpoint_whole_part(doubled) == 0x1UL);
  ASSERT(fixedpoint_frac_part(doubled) == 0xFFFFFFFFFFFFFFFEUL);
}

void test_halve_whole(TestObjs *objs) {
  (void) objs;

  Fixedpoint unhalved, halved;

  unhalved = fixedpoint_create_from_hex("00000000FFFFFFF0.0");
  halved = fixedpoint_halve(unhalved);
  ASSERT(fixedpoint_is_neg(halved) == 0);
  ASSERT(fixedpoint_whole_part(halved) == 0x000000007FFFFFF8UL);
  ASSERT(fixedpoint_frac_part(halved) == 0x0UL);
}

void test_halve_frac(TestObjs * objs) {
  (void) objs;

  Fixedpoint unhalved, halved;

  unhalved = fixedpoint_create_from_hex("0.00000000FFFFFFF0");
  halved = fixedpoint_halve(unhalved);
  ASSERT(fixedpoint_is_neg(halved) == 0);
  ASSERT(fixedpoint_whole_part(halved) == 0);
  ASSERT(fixedpoint_frac_part(halved) == 0x000000007FFFFFF8UL);
}

void test_halve_odd_whole(TestObjs * objs) {
  (void) objs;

  Fixedpoint unhalved, halved;

  unhalved = fixedpoint_create_from_hex("0000000000000003.0");
  halved = fixedpoint_halve(unhalved);
  ASSERT(fixedpoint_whole_part(halved) == 0x0000000000000001UL);
  ASSERT(fixedpoint_frac_part(halved) == 0x8000000000000000UL);
}

void test_halve_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint unhalved, halved;

  unhalved = fixedpoint_create_from_hex("-0000000000000002.0000000000000002");
  halved = fixedpoint_halve(unhalved);
  ASSERT(fixedpoint_is_neg(halved));
  ASSERT(fixedpoint_whole_part(halved) == 0x0000000000000001);
  ASSERT(fixedpoint_frac_part(halved) == 0x0000000000000001);
}

void test_halve_underflow_pos(TestObjs *objs) {
  (void) objs;

  Fixedpoint unhalved, halved;

  unhalved = fixedpoint_create_from_hex("0.0000000000000001");
  halved = fixedpoint_halve(unhalved);
  ASSERT(fixedpoint_is_neg(halved) == 0);
  ASSERT(fixedpoint_is_underflow_pos(halved));
  ASSERT(fixedpoint_is_underflow_neg(halved) == 0);
  ASSERT(fixedpoint_whole_part(halved) == 0x0UL);
  ASSERT(fixedpoint_frac_part(halved) == 0x0UL);
}

void test_halve_underflow_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint unhalved, halved;

  unhalved = fixedpoint_create_from_hex("-0.0000000000000001");
  halved = fixedpoint_halve(unhalved);
  ASSERT(fixedpoint_is_neg(halved));
  ASSERT(fixedpoint_is_underflow_neg(halved));
  ASSERT(fixedpoint_is_underflow_pos(halved) == 0);
  ASSERT(fixedpoint_whole_part(halved) == 0x0UL);
  ASSERT(fixedpoint_frac_part(halved) == 0x0UL);
}

void test_compare_pos_neg(TestObjs *objs) {
  (void) objs;

  Fixedpoint positive, negative;

  positive = fixedpoint_create_from_hex("000000000000000F.000000000000000F");
  negative = fixedpoint_create_from_hex("-000000000000000F.000000000000000F");
  ASSERT(fixedpoint_compare(positive, negative) == 1);
  ASSERT(fixedpoint_compare(negative, positive) == -1);
}

void test_compare_two_pos_whole(TestObjs *objs) {
  (void) objs;

  Fixedpoint biggerpos, smallerpos;

  biggerpos = fixedpoint_create_from_hex("000000000000000F.0");
  smallerpos = fixedpoint_create_from_hex("0000000000000001.0");
  ASSERT(fixedpoint_compare(biggerpos, smallerpos) == 1);
  ASSERT(fixedpoint_compare(smallerpos, biggerpos) == -1);
}

void test_compare_two_pos_frac(TestObjs *objs) {
  (void) objs;

  Fixedpoint biggerpos, smallerpos;

  biggerpos = fixedpoint_create_from_hex("0.000000000000000F");
  smallerpos = fixedpoint_create_from_hex("0.0000000000000001");
  ASSERT(fixedpoint_compare(biggerpos, smallerpos) == 1);
  ASSERT(fixedpoint_compare(smallerpos, biggerpos) == -1);
}

void test_compare_two_neg_whole(TestObjs *objs) {
  (void) objs;

  Fixedpoint biggerneg, smallerneg;

  biggerneg = fixedpoint_create_from_hex("-000000000000000F.0");
  smallerneg = fixedpoint_create_from_hex("-0000000000000001.0");
  ASSERT(fixedpoint_compare(biggerneg, smallerneg) == -1);
  ASSERT(fixedpoint_compare(smallerneg, biggerneg) == 1);
}

void test_compare_two_neg_frac(TestObjs *objs) {
  (void) objs;

  Fixedpoint biggerneg, smallerneg;

  biggerneg = fixedpoint_create_from_hex("-0.000000000000000F");
  smallerneg = fixedpoint_create_from_hex("-0.0000000000000001");
  ASSERT(fixedpoint_compare(biggerneg, smallerneg) == -1);
  ASSERT(fixedpoint_compare(smallerneg, biggerneg) == 1);
}

void test_compare_equals(TestObjs *objs) {
  (void) objs;

  Fixedpoint same_num_1, same_num_2;

  same_num_1 = fixedpoint_create_from_hex("0000000000000001.0000000000000001");
  same_num_2 = fixedpoint_create_from_hex("0000000000000001.0000000000000001");
  ASSERT(fixedpoint_compare(same_num_1, same_num_2) == 0);
  ASSERT(fixedpoint_compare(same_num_2, same_num_1) == 0);
  same_num_1 = fixedpoint_negate(same_num_1);
  same_num_2 = fixedpoint_negate(same_num_2);
  ASSERT(fixedpoint_compare(same_num_1, same_num_2) == 0);
  ASSERT(fixedpoint_compare(same_num_2, same_num_1) == 0);
}

void test_compare_equals_zeros(TestObjs *objs) {
  (void) objs;

  Fixedpoint zero1, zero2;

  zero1 = fixedpoint_create_from_hex("0.0");
  zero2 = fixedpoint_create_from_hex("-0.0");
  ASSERT(fixedpoint_compare(zero1, zero2) == 0);
}

// End of Custom Tests

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

// TODO: implement more test functions
