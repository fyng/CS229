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
<<<<<<< HEAD
  fp.is_neg = 0;
  fp.is_overflow_neg = 0;
  fp.is_overflow.pos = 0;
  fp.is_underflow.pos = 0;
  fp.is_underflow.neg = 0;
  fp.is_valid = 1;
=======

  fp.is_neg = 0;
  fp.is_err = 0;
  fp.invalid = 0;
>>>>>>> refs/remotes/origin/main
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
<<<<<<< HEAD
  fp.is_neg = 0;
  fp.is_overflow_neg = 0;
  fp.is_overflow_pos = 0;
  fp.is_underflow_neg = 0;
  fp.is_underflow_pos = 0;
  fp.is_valid = 1;
=======

  fp.is_neg = 0;
  fp.is_err = 0;
  fp.invalid = 0;
>>>>>>> refs/remotes/origin/main
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
  Fixedpoint sum;
  if (left.is_neg == right.is_neg) {
    sum.whole = left.whole + right.whole;
    sum.frac = left.frac + right.frac;
    if (sum.frac < left.frac || sum.frac < right.frac) {
      sum.whole += 1;
    }
    sum.is_neg = left.is_neg;
    sum.is_overflow_neg = 0;
    sum.is_overflow_pos = 0;
    if (sum.whole < left.whole || sum.whole < right.whole) {
      if (sum.is_neg == 1) {
	sum.is_overflow_neg = 1;
      }
      else {
	sum.is_overflow_pos = 1;
      }
    }
  }
  else {
    sum.is_neg = 0;
    if (left.is_neg > right.is_neg) {
      sum.whole = right.whole - left.whole;
      sum.frac = right.frac - left.frac;
    }
    else {
      sum.whole = left.whole - right.whole;
      sum.frac = left.frac - right.frac;
    }
    if (sum.whole > left.whole || sum.whole > right.whole) {
      sum.whole = 18446744073709551615 - sum.whole;
      sum.is_neg = 1;
    }
    if (sum.frac > left.frac || sum.frac > right.frac) {
      sum.frac = 18446744073709551615 - sum.frac;
      sume.whole -= 1;
    }
  }
  return sum;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  Fixedpoint difference;
  Fixedpoint negated_right = fixedpoint_negate(right);
  difference = fixedpoint_add(left, negated_right);
  return difference;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (val.is_neg == 0) {
    val.is_neg = 1;
  }
  else {
    val.is_neg = 0;
  }
  return val;
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
  return val.is_overflow_neg;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  return val.is_overflow_pos;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  return val.is_underflow_neg;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  return val.is_underflow_pos;
}

int fixedpoint_is_valid(Fixedpoint val) {
  return val.is_valid;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
