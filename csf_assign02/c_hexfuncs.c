// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here
unsigned hex_read(char data_buf[]) {
    // filedes == 0 defined to be stdin
    ssize_t n = read(0, data_buf, 16);
    if (n==0){
        // error handling
    }
    return n;
}

void hex_write_string(const char s[]) {
    int i = 0;
    while (*(s+i) != '\0'){
        // filedes == 1 defined to be stdout
        write(1, s+i, 1);
        i++;
    }
}

void hex_format_offset(unsigned offset, char sbuf[]) {
    for (int i=0; i<4; i++){
        // *(sbuf+i) = (char) ((offset << (i*8)) >> (7*8));
        hex_format_byte_as_hex((offset << (i*8) >> (3*8)), sbuf+(i*2));
    }
    *(sbuf+8) = '\0';
 }

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    *(sbuf) = '0' + (byteval / 16U) ;
    *(sbuf+1) = '0' + (byteval % 16U);
    *(sbuf+2) = '\0';
}

char hex_to_printable(unsigned char byteval) {
    if (byteval < 32 || byteval > 126){
        return '.';
    }
    return (char) byteval;
}