// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#include "cfuncs.h"
#include <iostream>
using namespace std;

bool isPowTwo(int n){
    if(n == 0) {
        return false;
    }
    // a power of 2 has a single 1-bit
    if ((n & (~ (n-1))) == n) {
        return true;
    } 
    return false;
};
