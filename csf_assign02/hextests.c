// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  // test zero
  hex_format_offset(0L, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

  // test digits
  hex_format_offset(305419896L, buf);
  ASSERT(0 == strcmp(buf, "12345678"));  
  hex_format_offset(9L, buf);
  ASSERT(0 == strcmp(buf, "00000009"));  

  // test letters
  hex_format_offset(2882461458L, buf);
  ASSERT(0 == strcmp(buf, "abcedf12"));

  // test largest hex
  hex_format_offset(4294967295L, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));


}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));

  // test small letter vs capital
  hex_format_byte_as_hex('h', buf);
  ASSERT(0 == strcmp(buf, "68"));
   
  // test symbols
  hex_format_byte_as_hex(' ', buf);
  ASSERT(0 == strcmp(buf, "20"));
  hex_format_byte_as_hex('@', buf);
  ASSERT(0 == strcmp(buf, "40"));

  // test unprintable characters
  hex_format_byte_as_hex('\0', buf);
  ASSERT(0 == strcmp(buf, "00"));
  hex_format_byte_as_hex((unsigned char) 10, buf);
  ASSERT(0 == strcmp(buf, "0a")); 
  hex_format_byte_as_hex((unsigned char) 127, buf);
  ASSERT(0 == strcmp(buf, "7f"));   
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));

  // test byte char < 32
  ASSERT('.' == hex_to_printable((unsigned char) 31));

  ASSERT(' ' == hex_to_printable((unsigned char) 32));
  ASSERT('~' == hex_to_printable((unsigned char) 126));

  // test byte char > 126
  ASSERT('.' == hex_to_printable((unsigned char) 127));
}
