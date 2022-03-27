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

int logTwo(int n){
    int log = 0;
    while(n >>= 1){
        log++;
    }
    return log;
};

Cache load_create_set_block(Cache cache, Block new_block, int index){
    cache.insert(pair<int, Set> (index, Set() ));
    new_block.access_ts = cache.cur_ts;
    cache.cache.at(index).set.push_back(new_block);
    cache.stats.load_miss++;

    cache.stats.total_cycles += 1 + (100 * (bytes_per_block / 4));
};

