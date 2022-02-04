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

  fp.is_neg = 0;
  fp.is_err = 0;
  fp.invalid = 0;
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;

  fp.is_neg = 0;
  fp.is_err = 0;
  fp.invalid = 0;
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint fp;
  // "f6a5865.00f2"
  // "258627685.242"
  fp.whole = 0UL;
  fp.frac = 0UL;

  uint64_t whole, frac;
  // Do i need this?

  char * negptr;
  char * pptr; 
  negptr = strchr(hex, '-');
  pptr = strchr(hex, '.');

  // check legal length
  unsigned int length = (unsigned)strlen(hex);
  if (negptr != NULL) {
    length--;
    hex++;
  }
  if (pptr == NULL) {
    if (length > 16){
      fp.is_err = 1;
      return fp;
    }
  } 
  // else if (length > 33 || (pptr - hex) > 16) {
  //   fp.is_err = 1;
  //   return fp;
  // }
  
  whole = strtoull(hex, &hex, 16);
  if (pptr != NULL) {
    frac = strtoull(pptr+1, &hex, 16);
  }
  
  if (whole == 0UL || frac == 0UL) {
    fp.is_err = 1;
    return fp;
  }

  frac = frac << (64 - (hex - (pptr+1)) * 4 );

  if (pptr != NULL) {
    return fixedpoint_create2(whole, frac);
  } else return fixedpoint_create(whole);
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
  return !val.invalid;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
