#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

Fixedpoint fixedpoint_create(uint64_t whole) {
  // create Fixedpoint variable to return via value
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = 0UL;
  fp.is_neg = 0;
  fp.is_overflow_neg = 0;
  fp.is_overflow_pos = 0;
  fp.is_underflow_pos = 0;
  fp.is_underflow_neg = 0;
  return fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint fp;
  fp.whole = whole;
  fp.frac = frac;
  fp.is_neg = 0;
  fp.is_overflow_neg = 0;
  fp.is_overflow_pos = 0;
  fp.is_underflow_neg = 0;
  fp.is_underflow_pos = 0;
  return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint fp;
  fp.whole = 0UL;
  fp.frac = 0UL;
  fp.is_neg = 0;
  fp.is_err = 0;

  uint64_t whole, frac;
  char * negptr;
  char * pptr; 
  negptr = strchr(hex, '-');
  pptr = strchr(hex, '.');

  // check legal length
  unsigned int length = (unsigned)strlen(hex);
  if (negptr != NULL) {
    if ((negptr - hex) != 0) {
      fp.is_err = 1;
      return fp;
    }
    length--;
    hex++;
    fp.is_neg = 1;
  }
  if (pptr != NULL){
    if ((pptr - hex) > 16 || (length - (pptr - hex + 1)) > 16) {
      fp.is_err = 1;
      return fp;
    } 
  }   
  
  whole = strtoul(hex, &hex, 16);
  if (pptr != NULL) {
    frac = strtoul(pptr+1, &hex, 16);
  }
  
  if (whole == 0UL || frac == 0UL) {
    fp.is_err = 1;
    return fp;
  }

  frac = frac << (64 - (hex - (pptr+1)) * 4 );

  fp.whole = whole;
  if (pptr != NULL) {
    fp.frac = frac;
  }
  return fp;
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
      sum.whole += 1UL;
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
      if (left.whole > right.whole) {
	sum.whole -= 1;
      }
      sum.frac = right.frac - left.frac;
    }
    else {
      sum.whole = left.whole - right.whole;
      if (right.whole > left.whole) {
	sum.whole -= 1;
      }
      sum.frac = left.frac - right.frac;
    }
    if (sum.whole > left.whole || sum.whole > right.whole) {
      sum.whole = 18446744073709551615UL + 1UL - sum.whole;
      sum.is_neg = 1;
    }
    if (sum.frac > left.frac || sum.frac > right.frac) {
      sum.frac = 18446744073709551615UL + 1UL - sum.frac;
      sum.whole = sum.whole - 1UL;
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
  if (val.whole == 0 && val.frac == 0) {
    return val;
  }
  if (val.is_neg == 0) {
    val.is_neg = 1;
  }
  else {
    val.is_neg = 0;
  }
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  uint64_t init_whole = val.whole;
  val.whole >> 1;

  if ((val.frac & 1UL) == 1){
    if (val.is_neg = 1) {
      val.is_underflow_neg = 1;
    }
    else {
      val.is_underflow_pos = 1;
    }
  }
  val.frac >> 1;

  if((init_whole & 0UL) == 1){
    val.frac = val.frac + (1UL << 63);
  }
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  uint64_t initial_frac = val.frac;
  int did_overflow = 0;
  val.frac << 1;
  if (val.frac < initial_frac) {
    did_overflow = 1;
  }
  uint64_t initial_whole = val.whole;
  val.whole << 1;
  val.whole += did_overflow;
  if (val.whole > initial_whole) {
    if (val.is_neg = 1) {
      val.is_overflow_neg = 1;
    }
    else {
      val.is_overflow_pos = 1;
    }
  }
  
  return val;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  if (left.is_neg == 1 && right.is_neg == 0) {
    return -1;
  }
  if (right.is_neg == 1 && left.is_neg == 0) {
    return 1;
  }
  if (left.whole > right.whole) {
    return 1;
  }
  if (right.whole > left.whole) {
    return -1;
  }
  if (left.frac > right.frac) {
    return 1;
  }
  if (right.frac > left.frac) {
    return -1;
  }
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
  if (fixedpoint_is_err(val) || fixedpoint_is_overflow_neg(val) 
  || fixedpoint_is_overflow_pos(val) || fixedpoint_is_underflow_neg(val) 
  || fixedpoint_is_underflow_pos(val)) {
    return 0;
  }
  return 1;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  char *s = malloc(35);
  int offset = 0;
  if(fixedpoint_is_neg(val)){
    s[0] = '-';
    offset++;
  }
  int chars = sprintf(s + offset, "%lx", fixedpoint_whole_part(val));
  offset = offset + chars;

  uint64_t frac = fixedpoint_frac_part(val);
  if (frac != 0){
    *(s + offset) = '.';
    offset++;
    
    // CONCEPT 2:
    // convert to hex string with left 0 padding. 
    // Then truncate trailing 0s
    chars = sprintf(s + offset, "%016lx", frac);
    while(*(s+offset+chars - 1) == '0') {
      *(s+offset+chars - 1) = '\0';
      chars--;
    }
  }
  return s;
}
