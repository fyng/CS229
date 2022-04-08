// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#include "cfuncs.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
using namespace std;


int main (int argc, char* argv[]) {
  // parameter error checking
  if (parameterCheck(argc, argv) == 1) {
    return 1;
  }
  int num_set = stoi(argv[1]);
  int blocks_per_set = stoi(argv[2]);
  int bytes_per_block = stoi(argv[3]);

  // Declare Cache instance
  Cache* cache = new Cache(num_set, blocks_per_set);
  if (cacheSetUp(argv, cache) == 1) {
    return 1;
  }
  
  // read from input
  string trace_line;
  while (getline(cin, trace_line)){
    stringstream ss(trace_line);
    string action, addr;
    ss >> action;
    ss >> addr;
    uint32_t address, index, tag;
    
    address = stoul(addr, 0 , 16);
    index = getIndex(address, bytes_per_block, num_set);
    tag = address >> (logTwo(bytes_per_block) + logTwo(num_set));

    // Load
    if (action.compare("l") == 0) {
      load(cache, index, tag, bytes_per_block);
    }
    // Store
    else if (action.compare("s") == 0) {
      store(cache, index, tag, bytes_per_block);      
    }
  }

  printParameters(cache);
  // printTestParameters(cache);
  
  delete cache;
  return 0;
};
