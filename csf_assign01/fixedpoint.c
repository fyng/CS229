#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  // create Fixedpoint variable to return via value
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = 0UL;
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint fp;

  // "f6a5865.00f2"

  if (hex[0] == NULL || hex[0] == ".") fp.is_err = 1;
  // 1. scan for "-", if found, set is_neg to 1
  if (hex[0] == '-'){
    fp.is_neg = 1;
    if (hex[1] == "0") fp.is_err = 1;
  }
  char* token;
  token = strtok(hex, ".");
  // hex is just a "-"
  if (token == NULL) {
    fp.is_err = 1;
    fp.whole = 0;
    fp.frac = 0;
    return fp;
  } 
  uint64_t whole = (int)strtol(token, NULL, 16);
  if (whole = 0) fp.is_err = 1;

  uint64_t frac = 0;
  token = strtok(NULL, ".");
  if (token != NULL) {
    frac = (int)strtol(token, NULL, 16);
    if (frac == 0) {
      fp.is_err = 1;
      fp.whole = 0;
      fp.frac = 0;
      return fp;
    }
  } 
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_zero(Fixedpoint val) {
  //if (fixedpoint_is_valid(val) == 0) {
  //return 0;
  //}

  if (val.whole == 0UL && val.frac == 0UL) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_err(Fixedpoint val) {
  return val.is_err;
}

int fixedpoint_is_neg(Fixedpoint val) {
  return val.is_neg;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
