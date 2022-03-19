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
  };
  
  // Cache set up
  map<int, vector<block>> cache;
  string trace_line;

  while (getline(cin, trace_line)) {
    stringstream ss(trace_line);
    string action; 
    string addr;
    ss >> action;
    ss >> addr;
    int address = stoi(addr, 0 , 16);
    int offset = (address << (32 - logTwo(bytes_per_block))) >> (32 - logTwo(bytes_per_block));
    int index = (address << (32 - logTwo(bytes_per_block) - logTwo(num_set))) >> (32 - logTwo(num_set));
    int tag = address >> (logTwo(bytes_per_block) + logTwo(num_set));

    // Create the Block struct
    block new_block;
    new_block.tag = tag;
    new_block.num_accesses = 0;
    new_block.dirty = false;
    // If loading
    if (action.compare("l")) {
      total_loads++;
      // If there is no set existent yet
      if (cache.find(index) == cache.end()) {
        cache.insert(pair<int, vector<block>> (index, vector<block>() ));
        cache.at(index).push_back(new_block);
        load_miss++;
        total_cycles += 1 + (100 * (bytes_per_block / 4));
      }
      // If the set exists
      else if (cache.find(index) != cache.end()) {
        bool hit = false;
        for (vector<block>::iterator it = cache.at(index).begin(); it != cache.at(index).end(); ++it) {
          if ((*it).tag == new_block.tag) {
            hit = true;
            (*it).num_accesses++;
          }
	      }
      // If the block exists
      if (hit) {
        load_hits++;
        total_cycles++;
      }
	    // If the block does not exist
	    else {
	    // If there is space
	      if ((int) cache.at(index).size() < blocks_per_set) {
		cache.at(index).push_back(new_block);
		load_miss++;
		total_cycles += 1 + (100 * (bytes_per_block / 4));
	      }
	      // If there is no space
	      else {
		// The case for lru
		if (evic == 1) {
		  int lowest_accesses = cache.at(index).at(0).num_accesses;
		  int block_index_low_acc = 0;
		  int count = 0;
		  for (vector<block>::iterator it = cache[index].begin(); it != cache[index].end(); ++ it) {
		    if ((*it).num_accesses < lowest_accesses) {
		      lowest_accesses = (*it).num_accesses;
		      block_index_low_acc = count;
		    }
		    count++;
		  }
		  cache.at(index).at(block_index_low_acc) = new_block;
		  load_miss++;
		  total_cycles += 1 + (100 * (bytes_per_block / 4));
		}
		// The case for FIFO
		else {
		}
	      }
	    }
      } else {
	// Was not given a valid block
      }
    } else if (action.compare("s")) {

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
