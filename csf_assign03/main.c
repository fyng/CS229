// Feiyang Huang;
// Tae Wan Kim; tkim104
#include <stdio.h>
#include <stdlib.h>

/*
 *
 */
// Here argc gets the number of arguments received, argv holds the string array of each argument
int main(int argc, char* argv[]) {
  if (argc == 6) {
    int checkPowerTwo = atoi(argv[0]);
    if (checkPowerTwo % 2) {
      return 0;
    }
    return 1;
  }
  return 0;
}
