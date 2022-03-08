// Feiyang Huang;
// Tae Wan Kim; tkim104
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *
 */
// Here argc gets the number of arguments received, argv holds the string array of each argument
int main(int argc, char* argv[]) {
  if (argc == 6) {
    // This for loop checks whether the first 3 arguments are valid
    if (atoi(argv[2]) < 4) {
      return 0;
    }
    for (int i = 0; i < 3; i++) {
      int checkPowerTwo = atoi(argv[i]);
      if (checkPowerTwo % 2) {
	return 0;
      }
    }
    return 1;
  }
  return 0;
}
