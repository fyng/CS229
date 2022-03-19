// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#include "cfuncs.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
using namespace std;

int main (int argc, char* argv[]) {
  // parameter error checking
  if (argc != 7) {
    std::cerr << "Invalid number of input parameters.\n";
    return 1;
  }
  
  int num_set = stoi(argv[1]);
  int blocks_per_set = stoi(argv[2]);
  int bytes_per_block = stoi(argv[3]);
  
  if (num_set < 0 || !isPowTwo(num_set)) {
    std::cerr << "Invalid number of sets in the cache.\n";
    return 1;
  }
  if (blocks_per_set < 0 || !isPowTwo(blocks_per_set)) {
    std::cerr << "Invalid number blocks per set.\n";
    return 1;
  }
  if (bytes_per_block < 4 || !isPowTwo(bytes_per_block)) {
    std::cerr << "Invalid number of bytes per block.\n";
    return 1;
  }
  
  int write_alloc; // 1: write-allocate; 0: no-write-allocate
  int write_mode;  // 1: write-through; 0: write-back
  int evic;       // 1: lru; 0: fifo
  
  string write_miss = argv[4];
  if (write_miss.compare("write-allocate") == 0){
    write_alloc = 1;
  } else if (write_miss.compare("no-write-allocate") == 0) {
    write_alloc = 0;
  } else {
    std::cerr << "Invalid write-miss mode.\n";
    return 1;
  }
  
  string write_hit = argv[5];
  if (write_hit.compare("write-through") == 0){
    write_mode = 1;
  } else if (write_hit.compare("write-back") == 0) {
    write_mode = 0;
  } else {
    std::cerr << "Invalid write-hit mode.\n";
    return 1;
  }
  
  string eviction = argv[6];
  if (eviction.compare("lru") == 0){
    evic = 1;
  } else if (eviction.compare("fifo") == 0) {
    evic = 0;
    } else {
    std::cerr << "Invalid eviction mode.\n";
    return 1;
  }
  
  cout << num_set << endl;
  cout << blocks_per_set << endl;
  cout << bytes_per_block << endl;
  cout << write_alloc << endl;
  cout << write_mode << endl;
  cout << evic << endl;

  // Parameters to keep track of and print
  int total_loads = 0;
  int total_stores = 0;
  int load_hits = 0;
  int load_miss = 0;
  int store_hits = 0;
  int store_miss = 0;
  int total_cycles = 0;

  // Block Struct
  struct block {
    int tag;
    int num_accesses;
    bool dirty;
  }
  
  // Cache set up
  map<int, vector<block>> cache;
  string trace_line = get_line();
  while (trace_line != NULL) {
    // Get primary parameters
    char action = get_action(trace_line);
    int address = get_address(trace_line);
    int offset = get_offset(address);
    int index = get_index(address);
    int tag = get_tag(address);
    // Create the Block struct
    block new_block;
    new_block.tag = tag;
    new_block.num_accesses = 0;
    new_block.dirty = false;
    // If loading
    if (action.equals('l')) {
      if (cache[index] == NULL) {
	load_miss++;
	cache.insert(pair<int, vector<block>> (index, vector<block>() ));
	total_cycles += 1 + (100 * (bytes_per_block / 4));
      } else if (cache[index] != NULL) {
	
      } else {
	// Was not given a valid block
      }
    } else if (action.equals('s')) {

    } else {
      // Did not read 'l' or 's'
    }
  }

  // Print out parameter values asked for
  cout << "Total loads: " << total_loads << endl;
  cout << "Total stores: " << total_stores << endl;
  cout << "Load hits: " << load_hits << endl;
  cout << "Load misses: " << load_miss << endl;
  cout << "Store hits: " << store_hits << endl;
  cout << "Store misses: " << store_miss << endl;
  cout << "Total cycles: " << total_cycles << endl;
  
  return 0;
}
