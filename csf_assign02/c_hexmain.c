// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  unsigned offset = 0U;
  char data_buf[16];
  char sbuf[9];
  unsigned count = hex_read(data_buf);

  while(count != 0U){
    hex_format_offset(offset, sbuf);
    hex_write_string(sbuf);
    hex_write_string(": "); 
    offset += 16U;
    
    for (unsigned i=0; i<count; i++){
      hex_format_byte_as_hex(data_buf[i], sbuf);
      hex_write_string(sbuf);
      hex_write_string(" ");
      data_buf[i] = hex_to_printable(data_buf[i]);
    }
    for (int i=0; i < (16U - count); i++){
      hex_write_string("   ");
    }

    hex_write_string(" ");
    hex_write_string(data_buf);

    // reset buffer
    for (int i=0; i < 16; i++) {
      data_buf[i] = '\0';
    }
    
    hex_write_string("\n");
    if (count < 16) {
      count = 0;
    }
    else {
      count = hex_read(data_buf);
    }
  }
} 
